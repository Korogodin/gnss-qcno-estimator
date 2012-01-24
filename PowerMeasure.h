/** @file PowerMeasure.h */
#ifndef POWERMEASURE_H_
#define POWERMEASURE_H_ 

#include "PowerMeasureTB.h"

#include "types.h" // Определяет типы переменных q*

/** Структура данных блока измерения эквивалентного отношения сигнал/шум
для одного корреляционного канала. */
typedef struct {

	qint32 Icoh, Qcoh; ///< Аккумуляторы для когерентного накопления квадратур
	qint32 Iold, Qold; ///< Значения с прошлого такта

	quint32 A_IQ_est; ///< Оценка амплитуды 1мс квадратур (уже с учетом U2_SHIFT)
	quint32 A_IQ_2_est; ///< Оценка квадрата амплитуды 1мс квадратур (уже с учетом U2_SHIFT)
	quint32 stdn_IQ_2_est; ///< Оценка квадрата амплитуды 1мс квадратур (уже с учетом U2_SHIFT)

	/** @name Аккумуляторы и их триггеры */
	qint32 R2; ///< Первичный акк.  \f$ I*Iold + Q*Qold \f$
	//@}

	qint32 R2_accumulation_threashold; ///< Если докопились до такой величины, считаем, что хватит

	/** @name Счетчики для аккумуляторов */
	quint16 sum_counter; ///< счетчик некогерентных накоплений для R2
	quint16 sum_counter_max; ///< Максимальное число некогерентных накоплений
	//@}

	quint8 New_measurements_are_ready; ///< Флаг готовности новых измерений
	quint8 n_Coher; ///< Счетчик произведенных внешних когерентных накоплений
	quint8 N_Coher; ///< Число мс когерентных накоплений
	quint8 N_Coher_shift; ///< Параметры для организации деления через умножение и сдвиг
	quint16 N_Coher_mult; ///<

	quint8 First_sample_of_bit; ///< Флаг того, что сейчас у нас происходит когерентное накопление на начале бита ЦИ
	quint8 ModeNow; ///< Имя режима
} PowerMeasure_struct; 

//
///** @name Перечисление констант */
//enum {
//    exp_kARU = 12,		///< Экспонента для переменной kARU ~ обратной крутизны
//    max_fail_counter = 2 		///< Число разрешенных fail_counter до снятия канала
//};
////@}

/** @name Перечисление имен режимов */
enum {
    PoMeMode_20ms,		///< Стандартный режим для скорости следования символов 20 мс
    PoMeMode_10ms,	    ///< Стандартный режим для скорости следования символов 10 мс
    PoMeMode_4ms,		///< Стандартный режим для скорости следования символов 4 мс
    PoMeMode_2ms,		///< Стандартный режим для скорости следования символов 2 мс
    PoMeMode_no_SS		///< Работаем по 1мс без символьной синхронизации
};
//@}


void NocohAccumPowerMeasure(PowerMeasure_struct *PoMe);

void SetModePowerMeasure(PowerMeasure_struct *PoMe, quint8 ModeName);

//void InitPowerMeasure(PowerMeasure_struct *PoMe, quint32 Init_qcno);
//
//void DoPowerMeasure(PowerMeasure_struct *PoMe);
//
//void AccumPowerMeasure(PowerMeasure_struct *PoMe, quint32 U2);
//
//int SetVariancePowerMeasure(PowerMeasure_struct *PoMe, quint32 stdn2_IQ);
//
//void AllowVariancePowerMeasure(PowerMeasure_struct *PoMe);
//
//void CalcTrueValues_PowerMeasure(PowerMeasure_struct *PoMe);

#endif // POWERMEASURE_H_
