 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "PowerMeasure.h"

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

int main(void)
{
	char *fname_in = "file_in.csv";
	FILE * fid_in = fopen(fname_in, "r");
    if (fid_in == 0)
    {
            printf("Не могу открыть файл '%s'", fname_in);
            return 1;
    }

    // Чтение K из файла
    int K;
    fscanf(fid_in, "%d", &K);

    // Чтение I,Q из файла

    int* I = (int *)malloc(sizeof(int) * K);
    int* Q = (int *)malloc(sizeof(int) * K);
    int* SyncTemp = (int *)malloc(sizeof(int) * K);
    int* SyncLast = (int *)malloc(sizeof(int) * K);
    int* SyncFirst = (int *)malloc(sizeof(int) * K);

    int i;
    for (i=0; i<K; i++){
    	fscanf(fid_in, "%d", &(I[i]));
//    	printf("I[%d] = %d\n", i, I[i]);
    }
    for (i=0; i<K; i++){
    	fscanf(fid_in, "%d", &(Q[i]));
//    	printf("Q[%d] = %d\n", i, Q[i]);
    }
//    for (i=0; i<K; i++){
//    	fscanf(fid_in, "%d", &(SyncTemp[i]));
////    	printf("SyncTemp[%d] = %d\n", i, SyncTemp[i]);
//    }
//    for (i=0; i<K; i++){
//    	fscanf(fid_in, "%d", &(SyncLast[i]));
////    	printf("SyncLast[%d] = %d\n", i, SyncLast[i]);
//    }

    for (i=0; i<K; i++){
    	fscanf(fid_in, "%d", &(SyncFirst[i]));
    }


    fclose(fid_in);


    unsigned int* x_A2_est = (unsigned int *)malloc(sizeof(unsigned int) * K);
    unsigned int* x_A_est = (unsigned int *)malloc(sizeof(unsigned int) * K);
    unsigned int* x_stdn2_est = (unsigned int *)malloc(sizeof(unsigned int) * K);
    unsigned int* x_stdn2_est_shifted = (unsigned int *)malloc(sizeof(unsigned int) * K);
    int* rough_qcno_dBHz = (int *)malloc(sizeof(int) * K);

    int* sum_counter = (int *)malloc(sizeof(int) * K);
    int* acum_counter = (int *)malloc(sizeof(int) * K);
    int* fail_counter = (int *)malloc(sizeof(int) * K);
    int* allow_stnd2_est = (int *)malloc(sizeof(int) * K);

    int* R2 = (int *)malloc(sizeof(int) * K);
    unsigned long int* R4 = (unsigned long int *)malloc(sizeof(unsigned long int) * K);
    unsigned int* R2_acum = (unsigned int *)malloc(sizeof(unsigned int) * K);
    unsigned long int* R4_acum = (unsigned long int *)malloc(sizeof(unsigned long int) * K);
    unsigned int* start_counter = (unsigned int *)malloc(sizeof(unsigned int) * K);
    unsigned int* IQ_Power = (unsigned int *)malloc(sizeof(unsigned int) * K);
    unsigned char* N_Coher = (unsigned char *)malloc(sizeof(unsigned char) * K);

    // Имитация потока исполнения
    int k;
    PowerMeasure_struct PoMe;

//    SetModePowerMeasure(&(PoMe), PoMeMode_20ms);
    SetModePowerMeasure(&(PoMe), PoMeMode_no_SS);
//    SetModePowerMeasure(&(PoMe), PoMeMode_20ms_4ms);
    SetVariancePowerMeasure(&(PoMe), 64);

    PoMe.x_A2_est = 0;
//	PoMe.A_IQ_est = 0;
    for (k=0; k<K; k++){

    	CohAccumPowerMeasure(&(PoMe), I[k], Q[k], SyncFirst[k]);

		if (PoMe.New_measurements_are_ready != 0){
			PoMe.New_measurements_are_ready = 0;

//			Calc_A_PowerMeasure(&(PoMe));
			Calc_Q_PowerMeasure(&(PoMe));
		}
//

		x_A2_est[k] = PoMe.x_A2_est>>PoMe.x_A2_shift; // PoMe.A_IQ_2_est;
//		A_IQ_2_est[k] = PoMe.A_IQ_2_est;
//		x_A_est[k] = PoMe.A_IQ_est;
		x_stdn2_est[k] = PoMe.stdn_IQ_2_est;
		//		x_stdn2_est_shifted[k] = PoMe.x_stdn2_est_shifted;
		rough_qcno_dBHz[k] = (int)PoMe.rough_qcno_dBHz;
		sum_counter[k] = (int)(PoMe.sum_counter);
		//		acum_counter[k] = (int)(PoMe.acum_counter);
		//		fail_counter[k] = (int)(PoMe.fail_counter);
		//		allow_stnd2_est[k] = (int)(PoMe.allow_stnd2_est);
		R2[k] = PoMe.R2;
		//		R4[k] = PoMe.R4;
		//		R2_acum[k] = PoMe.R2_acum;
		//		R4_acum[k] = PoMe.R4_acum;
		//		start_counter[k] = PoMe.start_counter;
		//		IQ_Power[k] = PoMe.IQ_Power;
		N_Coher[k] = PoMe.N_Coher;
    }

    // Запись результатов в файл
	char *fname_out = "file_out.csv";
	FILE * fid_out = fopen(fname_out, "w");
    if (fid_out == 0)
    {
            printf("Не могу открыть файл '%s'", fname_out);
            return 2;
    }

    // Запись
//    fprintf(fid_out, "%d\n", K);
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", x_A2_est[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", x_A_est[i]);
    }
    for (i=0; i<K; i++)
    	fprintf(fid_out, "%u\n", x_stdn2_est[i]);

    for (i=0; i<K; i++)
    	fprintf(fid_out, "%u\n", x_stdn2_est_shifted[i]);

    for (i=0; i<K; i++)
    	fprintf(fid_out, "%d\n", rough_qcno_dBHz[i]);

    for (i=0; i<K; i++){
    	fprintf(fid_out, "%d\n", sum_counter[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%d\n", acum_counter[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%d\n", fail_counter[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%d\n", allow_stnd2_est[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%d\n", R2[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%ul\n", R4[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", R2_acum[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%ul\n", R4_acum[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", start_counter[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", IQ_Power[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", N_Coher[i]);
    }

    free( I );
    free( Q );
    free( SyncTemp );
    free( SyncLast );

	free(x_A2_est);
	free(x_A_est);
	free(x_stdn2_est);

    free(x_stdn2_est_shifted);
    free(rough_qcno_dBHz);

    free(acum_counter);
    free(sum_counter);
    free(fail_counter);
    free(allow_stnd2_est);

    free(R2);
    free(R4);
    free(R2_acum);
    free(R4_acum);

    free(start_counter);
    free(IQ_Power);
    free(N_Coher);

    fclose(fid_out);

    printf("Runtime finished\n");
return 0;
}

