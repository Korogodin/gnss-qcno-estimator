#include "PowerMeasure.h"


#ifndef POME_TESTBENCH
	#include "std.h"
	#include <intrinsics.h>
#else
	int __CLZ(int x);
#endif

quint32 sqrt_PoMe(quint32 x);

/**
Производит необходимые когерентные накопления отсчетов, после чего вызывает
некогерентное накопление
@param PoMe - указатель на структуру данных блока оценки с/ш
@param I, Q - квадратурные суммы
*/
void CohAccumPowerMeasure(PowerMeasure_struct *PoMe, qint32 I, qint32 Q, qint8 SyncFirstFlag){

	if (PoMe->ModeNow != PoMeMode_no_SS)
		if (SyncFirstFlag){ // Копить нужно синхронно тактам ЦИ
			PoMe->First_sample_of_bit = 1;
			PoMe->Icoh = 0; PoMe->Qcoh = 0;
			PoMe->n_Coher = 1;
		}

	PoMe->Icoh += I;
	PoMe->Qcoh += Q;

	if (PoMe->n_Coher == PoMe->N_Coher){
		NocohAccumPowerMeasure(PoMe);
		PoMe->First_sample_of_bit = 0;
		PoMe->Icoh = 0;
		PoMe->Qcoh = 0;
    	PoMe->n_Coher = 0;
	}
	PoMe->n_Coher++;
}
/**
Производит необходимые накопления отсчетов и расчет квадрата амплитуды,
устанавливает пороги накопления по детектируемой мощности на входе.
@param PoMe - указатель на структуру данных блока оценки с/ш
*/
void NocohAccumPowerMeasure(PowerMeasure_struct *PoMe){


	if (PoMe->First_sample_of_bit == 0){
		PoMe->sum_counter++;
		// В 31 разряд входят 55 дБ (при 10мс ког) с десятикратным запасом при СКО_1мс = 8;
		PoMe->R2 += PoMe->Icoh*PoMe->Iold + PoMe->Qcoh*PoMe->Qold;
	}

	PoMe->Iold = PoMe->Icoh;
	PoMe->Qold = PoMe->Qcoh;

	if ((PoMe_abs(PoMe->R2) > PoMe->R2_accumulation_threashold)||(PoMe->sum_counter >= PoMe->sum_counter_max))
		if ( !((PoMe->sum_counter)&(PoMe->sum_counter - 1))) // Проверка является ли степенью двойки
			if ((PoMe->sum_counter<<3)>= PoMe->sum_counter_max){ // Всё-таки чаще чем нормы более чем в 8 раз не стоит делать

				quint32 tempA2;
				if (PoMe->R2 > 0){
					tempA2 = PoMe->R2;
					// Деление на N_Coher
					int tmp = (PoMe->N_Coher_shift - PoMe->x_A2_shift);
					if (tempA2 < 0x0000FFFF){ // Если число маленьное
						tempA2 *=  PoMe->N_Coher_mult;
						if (tmp < 0) // Всего-навсего сдвиг, но почему-то неадекватно работает с отрицательными
							tempA2 <<= (-tmp);
						else
							tempA2 >>= (tmp);
					}else{  // Если число большое
						if (tmp < 0)
							tempA2 <<= (-tmp);
						else
							tempA2 >>= (tmp);
						tempA2 *= PoMe->N_Coher_mult;
					}

					// Делим на число некогерентных накоплений
					int clz = 31-__CLZ(PoMe->sum_counter); // Степень при двойке
					tempA2 >>= clz;

				}else
					tempA2 = 0;

				if (tempA2>0){
					PoMe->x_A2_est = tempA2; // Данный функция может попасть в низкоприоритетные задачи
					PoMe->fail_counter = 0;
				}else{
					if ((PoMe->x_A2_est>>PoMe->x_A2_shift) > (PoMe->stdn_IQ_2_est>>2))
						PoMe->x_A2_est = PoMe->stdn_IQ_2_est<<(PoMe->x_A2_shift-3);
					else
						PoMe->x_A2_est /= 2;
					PoMe->fail_counter++;
				}
//				PoMe->A_IQ_2_est = PoMe->x_A2_est >> PoMe->x_A2_shift;

				PoMe->New_measurements_are_ready = 1; // Флаг, что есть неучтенные новые измерения

				PoMe->R2 = 0;
				PoMe->sum_counter = 0;
			}
}


/**
Устанавливаем параметры для определенного режима
@param PoMe - указатель на структуру данных блока оценки с/ш
@param ModeName - имя режима
*/
void SetModePowerMeasure(PowerMeasure_struct *PoMe, quint8 ModeName){

	PoMe->x_A2_shift = 8;

	switch (ModeName){
	case PoMeMode_20ms: // Стандартный режим для длительности бита 20 мс по 10 мс отсчетам
		PoMe->N_Coher = 10; // Число когерентных накоплений
		// Надо поделить на N_Coher^2 с помощью сдвига и умножения:
		PoMe->N_Coher_mult = 41; // При таких параметрах дополнительная ошибка -0.1 дБ
		PoMe->N_Coher_shift = 12; // Не стоит сильно приближаться к 31/2
		// Эти параметры позволяют достичь ошибки деления в 0.01 дБ

		PoMe->sum_counter_max = 64; // Чуть больше секунды (должна быть степень двойки!)

		PoMe->R2_accumulation_threashold = 400000; // (20*10)^2*10: На 10 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
		break;
	case PoMeMode_10ms:  // Стандартный режим для длительности бита 10 мс по 5 мс отсчетам
		PoMe->N_Coher = 5;
		PoMe->N_Coher_mult = 41;
		PoMe->N_Coher_shift = 10;
		// Эти параметры позволяют достичь ошибки деления в 0.01 дБ

		PoMe->sum_counter_max = 128; // (должна быть степень двойки!)

		PoMe->R2_accumulation_threashold = 200000; // (20*5)^2*20: На 20 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ

	    break;
	case PoMeMode_4ms:  // Стандартный режим для длительности бита 4 мс по 2 мс отсчетам
		PoMe->N_Coher = 2;
		PoMe->N_Coher_mult = 1;
		PoMe->N_Coher_shift = 2;

		PoMe->sum_counter_max = 256; // (должна быть степень двойки!)

		PoMe->R2_accumulation_threashold = 89600; // (20*2)^2*56: На 56 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
	    break;
	case PoMeMode_2ms:  // Стандартный режим для длительности бита 2 мс по 1 мс отсчетам
		PoMe->N_Coher = 1;
		PoMe->N_Coher_mult = 1;
		PoMe->N_Coher_shift = 0;

		PoMe->sum_counter_max = 512; // (должна быть степень двойки!)

		PoMe->R2_accumulation_threashold = 50800; // (20*1)^2*127: На 127 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
	    break;
	case PoMeMode_20ms_5ms:  // Длительность бита - 20мс, длительность когерентного накопления - 5мс
		PoMe->N_Coher = 5;
		PoMe->N_Coher_mult = 41;
		PoMe->N_Coher_shift = 10;
		// Эти параметры позволяют достичь ошибки деления в 0.01 дБ

		PoMe->sum_counter_max = 128; // (должна быть степень двойки!) (150 некогернетных накоплений на 1 секунде)

		PoMe->R2_accumulation_threashold = 200000; // (20*5)^2*20: На 20 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
	    break;
	case PoMeMode_20ms_4ms:  // Длительность бита - 20мс, длительность когерентного накопления - 4мс
		PoMe->N_Coher = 4;
		PoMe->N_Coher_mult = 1;
		PoMe->N_Coher_shift = 4;
		// Эти параметры позволяют достичь ошибки деления в 0.01 дБ

		PoMe->sum_counter_max = 256; // (должна быть степень двойки!) (200 некогернетных накоплений на 1 секунде)

		PoMe->R2_accumulation_threashold = 192000; // (20*4)^2*30: На 30 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
	    break;
	case PoMeMode_20ms_2ms:  // Длительность бита - 20мс, длительность когерентного накопления - 2мс
		PoMe->N_Coher = 2;
		PoMe->N_Coher_mult = 1;
		PoMe->N_Coher_shift = 2;
		// Эти параметры позволяют достичь ошибки деления в 0.01 дБ

		PoMe->sum_counter_max = 512; // (должна быть степень двойки!) (450 некогернетных накоплений на 1 секунде)

		PoMe->R2_accumulation_threashold = 89600; // (20*2)^2*56: На 56 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
	    break;
	case PoMeMode_no_SS: // Работаем по 1 мс отсчетам без символьной синхронизации
		PoMe->N_Coher = 1;
		PoMe->N_Coher_mult = 1;
		PoMe->N_Coher_shift = 0;
		PoMe->sum_counter_max = 2048; // (должна быть степень двойки!)

		PoMe->R2_accumulation_threashold = 50800; // (20*1)^2*127: На 56 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
		break;
	default:
		PoMe->N_Coher = 1;
		PoMe->N_Coher_mult = 1;
		PoMe->N_Coher_shift = 0;
		PoMe->sum_counter_max = 1024; // (должна быть степень двойки!)

		PoMe->R2_accumulation_threashold = 50800; // (20*1)^2*127: На 56 некогерентных накоплениях и 35 дБГц (A=20) уже имеем 400+-23 - погрешность в 0.5 дБ
	}

	PoMe->ModeNow = ModeName;
	PoMe->New_measurements_are_ready = 0;
	PoMe->sum_counter = 0;
	PoMe->R2 = 0;
	PoMe->First_sample_of_bit = 0;
	PoMe->n_Coher = 1;

	PoMe->Icoh = 0;	PoMe->Qcoh = 0;
	PoMe->Iold = 0;	PoMe->Qold = 0;

}


/**
Позволяет установить оценку дисперсии (мощности шумовой составляющей) корреляционных сумм и
зафиксировать её.
@param PoMe - указатель на структуру данных блока оценки с/ш
@param stdn2_IQ - устанавливаемое значение оценки дисперсии квадратур для базового времени накопления
@return 0, если прошло успешно, 1, если что-то не так
*/
int SetVariancePowerMeasure(PowerMeasure_struct *PoMe, quint32 stdn2_IQ ){
	PoMe->stdn_IQ_2_est = stdn2_IQ;
	return 0;
}

/**
Грубый расчет отношения сигнал/шум с точностью порядка 3 дБ.
Работает по сформированным оценкам квадрата амплитуды и дисперсии
шума квадратурных компонент. Формирует оценку rough_qcno_dBHz с
погрешностью "округления" [0 +3] дБГц.
@param PoMe - указатель на структуру данных блока оценки с/ш
*/
void Calc_Q_PowerMeasure(PowerMeasure_struct *PoMe){

	// Если за базовое накопление будем брать не 1мс, нужно будет переделать

	int log_arr[] = {9, 10, 10, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15};

	int A2 = PoMe->x_A2_est;
	int S2 = PoMe->stdn_IQ_2_est;

	int KA = 31 - __CLZ(PoMe->x_A2_est); // 2^KA <= PoMe->x_A2_est < 2*2^KA
	int KS = 31 - __CLZ(PoMe->stdn_IQ_2_est); // 2^KS <= PoMe->stdn_IQ_2_est < 2*2^KS

	int dA = 4; // Параметр сдвига между делимым и делителем
	int dC = 6; // Общий масштабный сдвиг делимого и делителя от нуля

	if ( (7 + (KA - PoMe->x_A2_shift)) < KS)
		PoMe->rough_qcno_dBHz = 6;
	else{

		int tmp = (dA - KA + dC);
		if (tmp < 0) // Почему-то сдвиг не отрабатывал отрицательные величины
			A2 >>= (-tmp); // Масштабируем так, чтобы отношение было в нужном нам диапазоне
		else
			A2 <<= (tmp);

		tmp = (- KS + dC);
		if (tmp < 0)
			S2 >>= (-tmp); // Масштабируем так, чтобы отношение было в нужном нам диапазоне
		else
			S2 <<= (tmp);


	    int sum_S = (S2 << (dA-1)) + S2;
	    int quotient_fix = 1<<(dA-1);

	    while (sum_S <= A2){
	        sum_S = sum_S + S2;
	        quotient_fix = quotient_fix + 1;
	    }

	    PoMe->rough_qcno_dBHz = 27 + log_arr[quotient_fix - (1<<(dA-1))] + 3*(KA-KS-dA-PoMe->x_A2_shift);
	}
}


//  static const int sh_sqrt=10; // Смещение вывода функции fix_sqrt
#define sh_sqrt 10

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

//static	const qint32 K_sqrt1 = (qint32)(0.09977*(1<<sh_sqrt)+0.5);
//static	const qint32 K_sqrt2 = (qint32)(0.71035*(1<<sh_sqrt)+0.5);
//static	const qint32 K_sqrt3 = (qint32)(0.3866*(1<<sh_sqrt)+0.5);
#define K_sqrt1  (qint32)(0.09977*(1<<sh_sqrt)+0.5)
#define K_sqrt2  (qint32)(0.71035*(1<<sh_sqrt)+0.5)
#define K_sqrt3  (qint32)(0.3866*(1<<sh_sqrt)+0.5)
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


/**
Алгоритмы оперируют сдвинутыми на U2_SHIFT суммами квадратов 1мс квадратур. Это приводит к тому,
что оценки дисперсии и квадрата амплитуды сами оказываются сдвинутыми на U2_SHIFT. Данная
функция производит обратный сдвиг и записывает результат в соответсвующие переменные
@param PoMe - указатель на структуру данных блока оценки с/ш
*/
//void CalcTrueValues_PowerMeasure(PowerMeasure_struct *PoMe){
//
//	unsigned int temp;
//
//	temp = PoMe->x_A2_est[0] << U2_SHIFT;
//	if (temp > PoMe->x_A2_est[0])
//		PoMe->A_IQ_2_est = temp/ PoMe->N_Coher;
//	else
//		PoMe->A_IQ_2_est = (PoMe->x_A2_est[0] / PoMe->N_Coher) << U2_SHIFT;
////	PoMe->A_IQ_2_est = (PoMe->x_A2_est[0] << U2_SHIFT);
//	PoMe->A_IQ_est = sqrt_PoMe( PoMe->A_IQ_2_est );
//	PoMe->stdn_IQ_2_est = PoMe->x_stdn2_est << U2_SHIFT;
//
//}
