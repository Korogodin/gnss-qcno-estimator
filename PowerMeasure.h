/** @file PowerMeasure.h */
#ifndef POWERMEASURE_H_
#define POWERMEASURE_H_ 

#include "PowerMeasureTB.h"

#include "types.h" // Определяет типы переменных q*

/** Структура данных блока измерения эквивалентного отношения сигнал/шум
для одного корреляционного канала. */
typedef struct {

	quint32 x_A2_est[1]; ///< Вектор состояния Ф СС за квадратом амплитуды КК, оценка=экстраполяция
	quint32 A_IQ_est; ///< Оценка амплитуды 1мс квадратур (уже с учетом U2_SHIFT)
	quint32 A_IQ_2_est; ///< Оценка квадрата амплитуды 1мс квадратур (уже с учетом U2_SHIFT)
	quint32 stdn_IQ_2_est; ///< Оценка квадрата амплитуды 1мс квадратур (уже с учетом U2_SHIFT)

	quint32 x_stdn2_est; ///< Оценка дисперсии КК
	quint32 x_stdn2_est_shifted; ///< Вектор состояния Ф СС за дисперсией КК, оценка=экстраполяция, сдвинут влево

	/** @name Аккумуляторы и их триггеры */
	quint32 R2; ///< Первичный акк.  \f$ I^2 + Q^2 \f$
	quint64 R4; ///< Первичный акк.  \f$ (I^2 + Q^2)^2 \f$

	quint32 R2_acum; ///< Вторичный аккумулятор R2
	quint64 R4_acum; ///< Вторичный аккумулятор R4

	quint32 R2_acum_copy; ///< Триггер R2_acum
	quint64 R4_acum_copy; ///< Триггер R4_acum
	//@}

	/** @name Счетчики для аккумуляторов */
	quint16 sum_counter; ///< для R2, R4
	quint16 acum_counter; ///< для R2_acum, R4_acum
	//@}

	/** @name Пороги накоплений R2_acum, R4_acum */
	quint16 acum_threshold;
	quint16 acum_threshold_lock;
	qint16 acum_threshold_level;
	qint16 acum_threshold_level_first;
	//@}

	quint16 fail_counter; ///< Число подряд идущих срывов измерения амплитуды (комплексные корни)

	quint32 start_counter; ///< Счетчик тактов от начала включения блока

	quint8 allow_stnd2_est; ///< Разрешить оценивать и фильтровать дисперсию квадратур

	int Accumulators_are_ready; ///< Флаг готовности триггеров R2_acum_copy, R4_acum_copy и копия счетчика в одном лице

	qint8 rough_qcno_dBHz; ///< Грубая оценка отношения сигнал шум с погрешностью "округления" в интервале [0 +3] дБГц

	#if (RECEIVER_TYPE == RECEIVER_ALPACA)
		/** @name Сохраненные измерения, поступающие на вход фильтров */
		quint32 SQ_A_izm; ///< Квадрата амплитуды
		quint32 SQ_stdn_izm; ///< Квадрата СКО
		//@}
		quint32 qcno; ///< Оценка отношения с/ш, в абсолютных единицах
		double qcno_dBHz; ///< Оценка отношения с/ш, в дБГц
		double qcno_dBHz_extr; ///< Экстраполяция на следующий такт отношения с/ш, в дБГц
	#endif

	quint32 IQ_Power; ///< Оценка \f$ E[I^2 + Q^2] \f$

	quint8 N_Coher; ///< Число мс когерентных накоплений

	quint16 sum_counter_max; ///< Число суммирований в первичных аккумуляторах R2, R4
} PowerMeasure_struct; 


/** @name Перечисление констант */
enum {
	PoMe_obr_Kf_stdn_0 = 256, 			///< Обратный коэффициент Ф СС за дисперсией КК

	#if (RECEIVER_TYPE == RECEIVER_ALPACA)
		U2_SHIFT = 6, 						///< Сдвиг I^2+Q^2 перед аккумулированием (вызовом AccumPowerMeasure)
	#elif (RECEIVER_TYPE == RECEIVER_NIIKP_ARM)
		U2_SHIFT = 0, 						///< Сдвиг I^2+Q^2 перед аккумулированием (вызовом AccumPowerMeasure)
	#endif
	  /**
	    I,Q in signed short =>
	    - log2((2^15)^2 * 2 / 2^6 * 128) = 32
	    - log2((2^15)^4 * 2 / 2^6 * 128) = 64

	    U2_SHIFT <-> :
	    U = sqrt(I^2 + Q^2):
	    - 1) log2(max U^2 >> U2_SHIFT) < 32
	    - 2) log2(max U^4 >> U2_SHIFT) < 64
	    - 3) PoMe_sum_counter_max < max_acum_counter (if hasn't reserve in 1) and 2) )
	    - 4) min U^2 >> U2_SHIFT > 0
	    - 5) max(U^2) ~ 2^(PoMe_NoiseU2Bit + 8) - for 51 dBHz and Tc = 0.001

	    Estimates of stdn2 and A2 are shifted by U2_SHIFT relative real values.
	  */

	#if (RECEIVER_TYPE == RECEIVER_ALPACA)
		PoMe_NoiseU2Bit = 20,				///< Typical \f$ \log_2(Noise^2) \f$, exponent of two
	#elif (RECEIVER_TYPE == RECEIVER_NIIKP_ARM)
		PoMe_NoiseU2Bit = 6,				///< Typical \f$ \log_2(Noise^2) \f$, exponent of two
	#endif
	PoMe_NoiseU2Bit_shifted = PoMe_NoiseU2Bit - U2_SHIFT, ///< PoMe_NoiseU2Bit-U2_SHIFT
	PoMe_x_stdn2_shift = 8,					///< Сдвиг дисперсии квадратур во внутренней структуре фильтра

	#if (RECEIVER_TYPE == RECEIVER_ALPACA)
		/** @name Делитель вторичных аккумуляторов для каждого времени накопления, сдвиги */
		PoMe_Threshold_5_sh = 2, 			// Период: 	2*PoMe_sum_counter_max
		PoMe_Threshold_4_sh = 3,			// 		4*PoMe_sum_counter_max
		PoMe_Threshold_3_sh = 3,			// 		8*PoMe_sum_counter_max
		PoMe_Threshold_2_sh = 3,			// 		32*PoMe_sum_counter_max
		PoMe_Threshold_1_sh = 4,			// 		64*PoMe_sum_counter_max
		PoMe_Threshold_def_sh = 4,			// 		32*PoMe_sum_counter_max
		//@}
	#elif (RECEIVER_TYPE == RECEIVER_NIIKP_ARM)
		/** @name Делитель вторичных аккумуляторов для каждого времени накопления, сдвиги */
		PoMe_Threshold_5_sh = 2, 			// Период: 	2*PoMe_sum_counter_max
		PoMe_Threshold_4_sh = 2,			// 		4*PoMe_sum_counter_max
		PoMe_Threshold_3_sh = 2,			// 		8*PoMe_sum_counter_max
		PoMe_Threshold_2_sh = 2,			// 		32*PoMe_sum_counter_max
		PoMe_Threshold_1_sh = 2,			// 		64*PoMe_sum_counter_max
		PoMe_Threshold_def_sh = 2,			// 		32*PoMe_sum_counter_max
		//@}
	#endif

	/** @name Делитель вторичных аккумуляторов для каждого времени накопления */
	PoMe_Threshold_5 = (1<<PoMe_Threshold_5_sh),
	PoMe_Threshold_4 = (1<<PoMe_Threshold_4_sh),
	PoMe_Threshold_3 = (1<<PoMe_Threshold_3_sh),
	PoMe_Threshold_2 = (1<<PoMe_Threshold_2_sh),
	PoMe_Threshold_1 = (1<<PoMe_Threshold_1_sh),
	PoMe_Threshold_def = (1<<PoMe_Threshold_def_sh),
	//@}
};
//@}

#if (RECEIVER_TYPE == RECEIVER_ALPACA)

  #define PoMe_Tcorr 0.001 ///< For qcno_dBHz calculation - длительность одного такта

  /** "Число лидирующих нулей" - число первых нулей бинарного представления.
  Данная функция реализована рекурсивно для совместимости с ARM НИИ КП
  @param x Входное 32-разрядное число
  @return Число первых нулей бинарного представления переменной */
  int __CLZ(int x);
  
#elif (RECEIVER_TYPE == RECEIVER_NIIKP_ARM) /*#ifndef NIIKP_ARM_F_*/
	#define exp_kARU 12		///< Экспонента для переменной kARU ~ обратной крутизны
	#define max_fail_counter 2 		///< Число разрешенных fail_counter до снятия канала
#endif /* if (RECEIVER_TYPE == RECEIVER_NIIKP_ARM) */

void InitPowerMeasure(PowerMeasure_struct *PoMe, quint32 Init_qcno);

void DoPowerMeasure(PowerMeasure_struct *PoMe); 

void AccumPowerMeasure(PowerMeasure_struct *PoMe, quint32 U2);

int SetVariancePowerMeasure(PowerMeasure_struct *PoMe, quint32 stdn2_IQ);

void AllowVariancePowerMeasure(PowerMeasure_struct *PoMe);

void CalcTrueValues_PowerMeasure(PowerMeasure_struct *PoMe);

#endif // POWERMEASURE_H_
