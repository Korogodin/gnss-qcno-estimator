#include "PowerMeasure.h"


#if (RECEIVER_TYPE == RECEIVER_ALPACA)
	#include <math.h>
	#ifndef POME_TESTBENCH
		#include "FPGA.h"
	#endif
#elif (RECEIVER_TYPE == RECEIVER_NIIKP_ARM)
	#ifndef POME_TESTBENCH
		#include "std.h"
		#include <intrinsics.h>
	#else
		int __CLZ(int x);
	#endif
#endif


quint32 sqrt_PoMe(quint32 x);
quint32 NearestPower2(quint32 x); // Ближайщее большее 2^n, возвращаем n
void RoughCalc_qcno_dBHz_PowerMeasure(PowerMeasure_struct *PoMe);

/**
Инициализация структуры данных блока оценки с/ш, сброс счетчиков.
@param PoMe - указатель на структуру данных блока оценки с/ш
@param Init_qcno - инициализационное значение отношения с/ш, в НИИ КП не используется
*/
void InitPowerMeasure(PowerMeasure_struct * PoMe, quint32 Init_qcno)
{
	PoMe->x_A2_est[0] = (1<<PoMe_NoiseU2Bit_shifted<<5);

	PoMe->x_stdn2_est = (1<<PoMe_NoiseU2Bit_shifted); // Для первого порядка совпадает с экстраполяцией
	PoMe->x_stdn2_est_shifted = (PoMe->x_stdn2_est << PoMe_x_stdn2_shift);
	PoMe->allow_stnd2_est = 1; // Разрешить оценивать и фильтровать дисперсию квадратур

	PoMe->R2 = 0;
	PoMe->R4 = 0;

	PoMe->acum_counter = 0; // Счетчик второго уровня накопителя корреляционных сумм
	PoMe->sum_counter = 0; // Счетчик первого уровня накопителя корреляционных сумм

	PoMe->fail_counter = 0; // Счетчик подряд идущих фейлов измерений

	PoMe->start_counter = 0; // Счетчик тиков фильтра СКО от разрешения, доходит до (obr_Kf_stdn_0+задержка) и застывает

	PoMe->acum_threshold_lock = 0;

#if (RECEIVER_TYPE == RECEIVER_ALPACA)
	if (Init_qcno > 0)
	  PoMe->qcno = Init_qcno;
	else
	  PoMe->qcno = (quint32)( PoMe->x_A2_est[0] / (2.0 * PoMe->x_stdn2_est * PoMe_Tcorr) );
	PoMe->qcno_dBHz =  10.0*log10(PoMe->qcno);
	PoMe->qcno_dBHz_extr = PoMe->qcno_dBHz;
#endif
}


/**
По накопленным \f$\sum I_k^2+Q_k^2\f$ и \f$\sum (I_k^2+Q_k^2)^2\f$ производим
оценки дисперсии, амплитуды и квадрата амплитуды квадратур.
Основная нить вычислений:
\f[ M = \frac{R2}{K} \f]
\f[ Diskrimi = 2M^2 - \frac{R4}{K} \f]
\f[ A^2_{izm} = M - 2 \sigma^2_{filt} \f]
\f[ \sigma^2_{izm} = \frac{M - \sqrt{Diskrimi}}{2} \f]
Измерения \f$ \sigma^2_{izm} \f$ хорошенько фильтруем, чтобы не испортить \f$ A^2_{izm} \f$
@param PoMe - указатель на структуру данных блока оценки с/ш
*/
void DoPowerMeasure(PowerMeasure_struct *PoMe)
{
	quint32 M;
	quint32 Diskrimi;
	quint32 SQ_stdn_izm;
	quint32 SQ_A_izm;
	//qint32 Ud_A;
	qint32 Ud_stdn;
	qint64 tmp64_1, tmp64_2, tmp64_3;
	quint32 tmpu32_2, Diskrimi_shift;

	/* Division by acum_counter */
	int clz=31-__CLZ(PoMe->Accumulators_are_ready);
	if((1<<clz)==PoMe->Accumulators_are_ready)
	{
		tmp64_1 = PoMe->R2_acum_copy >> clz; // M
		tmp64_3 = PoMe->R4_acum_copy >> clz;
	}
	else
	{
		tmp64_1 = PoMe->R2_acum_copy / PoMe->Accumulators_are_ready; // M
		tmp64_3 = PoMe->R4_acum_copy / PoMe->Accumulators_are_ready;
	}
	/***/

	PoMe->acum_threshold_lock = 0;

	tmp64_2 = 2*tmp64_1*tmp64_1;
	tmp64_2 = tmp64_2 - tmp64_3; // Diskrimi
	if (tmp64_2 < 0)tmp64_2 = 0;

	tmpu32_2 = ((tmp64_2 >> 32)&0xFFFFFFFF);
	Diskrimi_shift = 32 - __CLZ(tmpu32_2);
	if (Diskrimi_shift & 0x1) Diskrimi_shift++; // Чётное must be
	M = (tmp64_1&0xFFFFFFFF);
	Diskrimi =  ((tmp64_2 >> Diskrimi_shift)&0xFFFFFFFF);
	PoMe->Accumulators_are_ready = 0;
	PoMe->IQ_Power = M;

	/* Измерение квадрата амплитуды */
	if (M > 2*PoMe->x_stdn2_est){
		SQ_A_izm =  M - 2*PoMe->x_stdn2_est;
		PoMe->fail_counter = 0;
	}
	else {
		PoMe->fail_counter++;
		if (PoMe->x_A2_est[0] > (1<<PoMe_NoiseU2Bit_shifted) ) SQ_A_izm = (1<<PoMe_NoiseU2Bit_shifted);
		else SQ_A_izm =  PoMe->x_A2_est[0] - PoMe->x_A2_est[0]/2; //0.05*(0 - PoMe->x_A2_est[0]);
	}
	/***/


	/* Фильтр амплитуды и её квадрата */
	PoMe->x_A2_est[0] = SQ_A_izm;
	PoMe->x_A_est = sqrt_PoMe( PoMe->x_A2_est[0] );
	/***/

	if (PoMe->allow_stnd2_est == 1){
		quint32 tmp;
		/* Измерение квадрата СКО квадратур */
		if ( Diskrimi > 0 ) {
			tmp = sqrt_PoMe(Diskrimi);
			if (tmp != 0){ // Одна итерация по Герону для увеличения точности до требуемой
				tmp += ((int)(Diskrimi) - (int)(tmp*tmp))/2 / (int)(tmp); // Обратное смещение с учетом смещения в корне
			}
			tmp <<= (Diskrimi_shift/2);
			if (M > tmp)
			  SQ_stdn_izm = (M - tmp) / 2 ; //
			else
			  SQ_stdn_izm = 0;
		} else
			SQ_stdn_izm = M / 2;
		/***/

		if (SQ_stdn_izm > 2*PoMe->x_stdn2_est) // При резком изменении амплитуды на интервале оценивания
			SQ_stdn_izm = 2*PoMe->x_stdn2_est; // происходит аномальный выброс, т.к. процесс не соответствует модели постоянства параметра


		/* Фильтр диспресии квадратурных компонент */
		Ud_stdn = (SQ_stdn_izm<<PoMe_x_stdn2_shift) - PoMe->x_stdn2_est_shifted; // Дискриминатор дисперсии квадратур
		quint32 start_delay = 10; // First PoMe_obr_Kf_stdn_0 steps Kalman is used
		if (PoMe->start_counter >= (PoMe_obr_Kf_stdn_0 + start_delay)) // После подстройки опять затягиваем фильтр
			PoMe->x_stdn2_est_shifted += Ud_stdn / PoMe_obr_Kf_stdn_0;
		else if (PoMe->start_counter < start_delay) { // На начальном этапе фильтр затянут
			PoMe->start_counter++;
			PoMe->x_stdn2_est_shifted += Ud_stdn / PoMe_obr_Kf_stdn_0;
		}
		else { // После выжидания start_delay расширяем фильтр и быстро подстраиваемся
			PoMe->start_counter++;
			PoMe->x_stdn2_est_shifted += Ud_stdn >> NearestPower2(PoMe->start_counter - start_delay);
		}
		PoMe->x_stdn2_est = PoMe->x_stdn2_est_shifted>>PoMe_x_stdn2_shift;
		/***/

	} // if (PoMe->allow_stnd2_est == 1)

	RoughCalc_qcno_dBHz_PowerMeasure(PoMe);
#if (RECEIVER_TYPE == RECEIVER_ALPACA)
	PoMe->SQ_A_izm = SQ_A_izm;
	PoMe->SQ_stdn_izm = SQ_stdn_izm;
	PoMe->qcno = (quint32)( PoMe->x_A2_est[0] / (2.0 * PoMe->x_stdn2_est * PoMe_Tcorr) );
	PoMe->qcno_dBHz =  10.0*log10(PoMe->qcno);
	PoMe->qcno_dBHz_extr = PoMe->qcno_dBHz;
#endif

	//#ifdef __Model
	//	static FILE* fid_ampout=0;
	//	if (!fid_ampout)
	//	{
	//		fid_ampout = fopen("amp.csv", "wt");
	// 		fprintf(fid_ampout,"tmp64_1;tmp64_2;tmp64_3;SQ_stdn;Diskrimi;M;SQ_A_izm;x_stdn2_est;x_A2_est[0];qcno_dBHz; rough_qcno_dBHz;\n");
	//	}
	//	fprintf(fid_ampout,"%I64u;%I64u;%I64u;%d;%u;%u;%u;%u;%u;%f;%u\n",
	//						tmp64_1,tmp64_2,tmp64_3,
	//						SQ_stdn_izm,
	//						Diskrimi,
	//						M,
	//						SQ_A_izm,
	//						PoMe->x_stdn2_est,
	//						PoMe->x_A2_est[0],
	//						10.0*log10( PoMe->x_A2_est[0] / (2.0 * PoMe->x_stdn2_est * 0.001) ),
	//                        PoMe->rough_qcno_dBHz);

	//	fflush(fid_ampout);
	//#endif
}

/**
Сопоставляет выбранному номеру порога число требуемых накоплений второго уровня.
@param PoMe - указатель на структуру данных блока оценки с/ш

@todo Что-то слишком громозко и сложно, нужно переделать систему порогов
*/
void SetAccumThreshold(PowerMeasure_struct *PoMe){

  switch (PoMe->acum_threshold_level){
    case 5:
      PoMe->acum_threshold =  PoMe_Threshold_5;
      break;
    case 4:
      PoMe->acum_threshold =  PoMe_Threshold_4;
      break;
    case 3:
      PoMe->acum_threshold =  PoMe_Threshold_3;
      break;
    case 2:
      PoMe->acum_threshold =  PoMe_Threshold_2;
      break;
    case 1:
      PoMe->acum_threshold =  PoMe_Threshold_1;
      break;
    default:
      PoMe->acum_threshold =  PoMe_Threshold_def;
  }
}

/**
Производит необходимые накопления суммы квадратов корреляционных сумм,
устанавливает пороги накопления по детектируемой мощности на входе.
Различаются накопления двух уровней.
 - первый: накопление PoMe_sum_counter_max величин U2, результат - детектор мощности
 - второй: накопление acum_threshold сумм первого уровня
@param PoMe - указатель на структуру данных блока оценки с/ш
@param U2 - сумма квадратов корреляционных сумм \f$U2 = I_k^2 + Q_k^2\f$
*/
void AccumPowerMeasure(PowerMeasure_struct *PoMe, quint32 U2){

  quint64 tmpu64;
  quint32 tmpu32;
  int overflow_F_ = 0;
  PoMe->sum_counter++;  if (PoMe->sum_counter == PoMe_sum_counter_max) PoMe->sum_counter = 0;

  tmpu32 = PoMe->R2 + U2;
  if (PoMe->R2 <= tmpu32) // overflow defense
    PoMe->R2 = tmpu32; // sum I^2 + Q^2
  else{ // impossible, if PoMe_sum_counter_max and U2_SHIFT are correct
    PoMe->R2 = U2;
    overflow_F_ = 1;
  }

  tmpu64 = PoMe->R4 + (quint64)(U2) * (quint64)(U2);
  if ( (PoMe->R4 <= tmpu64) & (overflow_F_ == 0) ) // overflow defense
    PoMe->R4 = tmpu64; //  sum (I^2 + Q^2)^2
  else{ // impossible, if PoMe_sum_counter_max and U2_SHIFT are correct
    PoMe->R4 = (quint64)(U2) * (quint64)(U2);
  }

  if (PoMe->sum_counter == (PoMe_sum_counter_max - 1) )
  {
    quint64 tmpu64_1, tmpu64_2;
    quint32 tmpu32_1, tmpu32_2;
    if (overflow_F_ == 0){
		tmpu64_1 = PoMe->R4 / PoMe_sum_counter_max;
		tmpu32_1 = PoMe->R2 / PoMe_sum_counter_max;
    } else {
		tmpu64_1 = PoMe->R4;
		tmpu32_1 = PoMe->R2;
    }
    PoMe->R2 = 0;
    PoMe->R4 = 0;

    tmpu64_2 = PoMe->R4_acum + tmpu64_1;
    tmpu32_2 = PoMe->R2_acum + tmpu32_1;

    if ( (PoMe->R4_acum <= tmpu64_2)&(PoMe->R2_acum <= tmpu32_2)&(overflow_F_ == 0) ){ // Переполнения нет
      PoMe->R4_acum = tmpu64_2;
      PoMe->R2_acum = tmpu32_2;
      PoMe->acum_counter++;
    }else if ( (tmpu64_1 > PoMe->R4_acum)||(tmpu32_1 > PoMe->R2_acum)||(overflow_F_ == 0) ){
      // impossible, if PoMe_sum_counter_max, U2_SHIFT and max[threshold] are correct
      overflow_F_ = 1;
      PoMe->R4_acum = tmpu64_1; // А раз оно такое большое, то будем по нему и работать
      PoMe->R2_acum = tmpu32_1;
      PoMe->acum_counter = 1;
    }else {  // Штатное переполнение по капле: где-то что-то мы проворонили
      overflow_F_ = 1; // Надеяться и ждать, что сейчас всё пучком пройдет)
    }

    // *********** Threshold setting **********************
    tmpu32_2 = 32 - __CLZ(tmpu32_1); // Num of ones in E[U2]
    if ( tmpu32_2 > (PoMe_NoiseU2Bit_shifted + 8) ){  // Power >> Noise Power
      PoMe->acum_threshold_level = 5;
    }else if ( tmpu32_2 > (PoMe_NoiseU2Bit_shifted + 4) ){
      PoMe->acum_threshold_level = 4;
    }else if ( tmpu32_2 > (PoMe_NoiseU2Bit_shifted+2) ){
      PoMe->acum_threshold_level = 3;
    }else
      if ( (PoMe->x_A2_est[0])>>(PoMe_NoiseU2Bit_shifted-1)){
      PoMe->acum_threshold_level = 2;
      }else{
	PoMe->acum_threshold_level = 1;
      }
    if (PoMe->acum_counter == 1) PoMe->acum_threshold_level_first = PoMe->acum_threshold_level;


    if ((PoMe->acum_threshold_level - PoMe->acum_threshold_level_first) > 1){ // Up
      PoMe->acum_threshold_lock = 1;
      SetAccumThreshold(PoMe);
      PoMe->R4_acum = tmpu64_1; // Old data so old
      PoMe->R2_acum = tmpu32_1;
      PoMe->acum_counter = 1;
      PoMe->acum_threshold_level_first = PoMe->acum_threshold_level; // One second or more
      if (PoMe->acum_threshold_level_first > 3)
	PoMe->acum_threshold =  1;
      else if (PoMe->acum_threshold_level_first == 3)
	PoMe->acum_threshold =  2;
      else
	PoMe->acum_threshold =  4;
    }else if ( (PoMe->acum_threshold_level_first - PoMe->acum_threshold_level) > 1){// Down
	if (PoMe->acum_threshold_lock == 0){
	  PoMe->R4_acum = tmpu64_1; // Old data so old
	  PoMe->R2_acum = tmpu32_1;
	  PoMe->acum_counter = 1;
	  PoMe->acum_threshold_level_first = PoMe->acum_threshold_level;
	  PoMe->acum_threshold_lock = 1;
	  PoMe->acum_threshold =  (1024 / PoMe_sum_counter_max); 	// One second
	}
    }else
      if ( (PoMe->acum_threshold_lock == 0) & (PoMe->acum_counter == 1) )
	SetAccumThreshold(PoMe);
      //PoMe->acum_threshold=8;

    if ( (PoMe->acum_counter >= PoMe->acum_threshold) // Превысили порог накопления для текущего qcno
              || (overflow_F_) // Overflow in R2_acum or R4_acum
       ){
      PoMe->Accumulators_are_ready = PoMe->acum_counter; 	PoMe->acum_counter = 0;
      PoMe->R4_acum_copy = PoMe->R4_acum; 			PoMe->R4_acum = 0;
      PoMe->R2_acum_copy = PoMe->R2_acum;			PoMe->R2_acum = 0;
    }

  }

}


/**
Позволяет установить оценку дисперсии (мощности шумовой составляющей) корреляционных сумм и
зафиксировать её.
@param PoMe - указатель на структуру данных блока оценки с/ш
@param stdn2_IQ - устанавливаемое значение оценки дисперсии квадратур
@return 0, если прошло успешно, 1, если предлагаемое число после сдвига не влазиет в 32 разряда
*/
int SetVariancePowerMeasure(PowerMeasure_struct *PoMe, quint32 stdn2_IQ ){
	if (__CLZ(stdn2_IQ) >= PoMe_x_stdn2_shift){ // Если предлагаемое число можно сдвинуть, не переполнив 32 разряда
		PoMe->x_stdn2_est = stdn2_IQ;
		// В случае увеличения порядка фильтра добавить сюда PoMe->x_stdn2_extr = stdn2_IQ;
		PoMe->x_stdn2_est_shifted = stdn2_IQ<<PoMe_x_stdn2_shift;
		return 0;
	}else	return 1;
}


/**
Разрешает работу фильтру дисперсии квадратур, сбрасывает его счетчик.
@param PoMe - указатель на структуру данных блока оценки с/ш
*/
void AllowVariancePowerMeasure(PowerMeasure_struct *PoMe){
	PoMe->allow_stnd2_est = 1;
	PoMe->start_counter = 8;
}

/**
Грубый расчет отношения сигнал/шум с точностью порядка 3 дБ.
Работает по сформированным оценкам квадрата амплитуды и дисперсии
шума квадратурных компонент. Формирует оценку rough_qcno_dBHz с
погрешностью "округления" [0 +3] дБГц.
@param PoMe - указатель на структуру данных блока оценки с/ш
*/
void RoughCalc_qcno_dBHz_PowerMeasure(PowerMeasure_struct *PoMe){
	int tmp;
	tmp = __CLZ(PoMe->x_stdn2_est) - __CLZ(PoMe->x_A2_est[0]); // "На сколько по 3дБ амплитуда2 больше СКО2"
	PoMe->rough_qcno_dBHz = 27 + 3*tmp; // При 27 дБГц амплитуда и СКО квадратур практически равны
}



#if (RECEIVER_TYPE == RECEIVER_ALPACA)
/**
Возвращает число подряд идущих нулевых разрядов слева в бинарном представлении 32-разрядного числа.
Например, для x=b'00000111 вернет 29.
@param x
*/
int __CLZ(int x){

  int i;

  for (i=31; i>=0; i--){
    if (x >> i)
      return (31 - i);
  }
  return 32;
}
#endif


#if (RECEIVER_TYPE == RECEIVER_ALPACA)
  #define sh_sqrt 10
#else
//  static const int sh_sqrt=10; // Смещение вывода функции fix_sqrt
#define sh_sqrt 10
#endif
/**
Нормирует  1.0 <= x <=4.0 * (2** -sh_sqrt) .
@param x - нормируемое число
@param exp - указатель, через который возвращается сдвиг
@return Результат нормировки
*/
quint32 norm_x_PoMe(quint32 x, qint32 * exp){
	*exp =0;
	int razr=32-__CLZ(x);
// 	printf("razr = %d \n", razr);
	if(razr<sh_sqrt){
		x=x<<(sh_sqrt-razr);
		*exp=-(sh_sqrt-razr);
// 		printf("razr < exp = %d; x = %d \n", *exp, x);
	}
	else if(razr > (sh_sqrt+1)){
		x=x>>(razr-sh_sqrt);
		*exp=razr-sh_sqrt;
// 		printf("razr > exp = %d; x = %d \n", *exp, x);
	}
	if( (*exp) & 1){
		x=x<<1;
		(*exp)--;
// 		printf("exp not even  exp = %d; x = %d \n", *exp, x);
	}
	return x;
}

#if (RECEIVER_TYPE == RECEIVER_ALPACA)
#define K_sqrt1  (qint32)(0.09977*(1<<sh_sqrt)+0.5)
#define K_sqrt2  (qint32)(0.71035*(1<<sh_sqrt)+0.5)
#define K_sqrt3  (qint32)(0.3866*(1<<sh_sqrt)+0.5)
#else
//static	const qint32 K_sqrt1 = (qint32)(0.09977*(1<<sh_sqrt)+0.5);
//static	const qint32 K_sqrt2 = (qint32)(0.71035*(1<<sh_sqrt)+0.5);
//static	const qint32 K_sqrt3 = (qint32)(0.3866*(1<<sh_sqrt)+0.5);
#define K_sqrt1  (qint32)(0.09977*(1<<sh_sqrt)+0.5)
#define K_sqrt2  (qint32)(0.71035*(1<<sh_sqrt)+0.5)
#define K_sqrt3  (qint32)(0.3866*(1<<sh_sqrt)+0.5)
#endif
//#define sqrt_27_bit
/**
Целочисленный алгорит вычисления корня \f$ y = \sqrt{x} \f$.
** алгоритм с плав. зап. взят IAR\ARM\src\lib\dlib\xxsqrt.h
** для sh_sqrt=10 погрешность sqrt( 0 < x < 4) не более 0.004
** для sh_sqrt=12 погрешность sqrt( 0 < x < 4) не более 0.001
@param x - целое положительное число, из которого вычисляется корень
@return Результат вычисления корня
*/
quint32 sqrt_PoMe(quint32 x){	// x  - с ц.р. = 2**-sh_sqrt
	int exp;
	x = norm_x_PoMe(x,&exp); // нормировка 1.0<= x <=4.0 для сходимости, exp кратна 2
	int	y1;
	y1 =( ( (K_sqrt2 - ((K_sqrt1*x)>>sh_sqrt)) * x ) >> sh_sqrt ) + K_sqrt3;
	exp=exp/2-sh_sqrt/2;
	if (exp>0)	return y1<<exp;
	else		return y1>>(-exp);
}


const quint16 Power2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
const quint16 SizePower2 = 13;
/**
Функция нахождения ближайшего большего или равного числа, являющегося степенью двойки.
@todo Перейти на __CLZ
@param x
@return Степень, в которую требуется возвести 2, чтоб получить искомое число
*/
quint32 NearestPower2(quint32 x){//// Ближайшее большее или равное x число 2^n  (возвращаем n)
	int i;
	if (x >= Power2[SizePower2-1]) return SizePower2-1; // Граничные условия
	for (i=0; i<SizePower2; i++){
		if (x <= Power2[i]) // Первое число в последовательности, которое привысит или будет равно x
			return i;   // и будет ответом
	}
	return 0;
}
