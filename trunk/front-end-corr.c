 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "PowerMeasure.h"

#if (RECEIVER_TYPE == RECEIVER_NIIKP_ARM)
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
    int I[K];
    int Q[K];
    int i;
    for (i=0; i<K; i++){
    	fscanf(fid_in, "%d", &(I[i]));
//    	printf("I[%d] = %d\n", i, I[i]);
    }
    for (i=0; i<K; i++){
    	fscanf(fid_in, "%d", &(Q[i]));
//    	printf("Q[%d] = %d\n", i, Q[i]);
    }

    fclose(fid_in);


    int x_A2_est[K], x_A_est[K], x_stdn2_est[K], rough_qcno_dBHz[K];
    int sum_counter[K], acum_counter[K];

    int* fail_counter = (int *)malloc(sizeof(int) * K);
    int* allow_stnd2_est = (int *)malloc(sizeof(int) * K);

    // Имитация потока исполнения
    int k;
    PowerMeasure_struct PoMe;

    InitPowerMeasure(&(PoMe), 0);
    unsigned int U2;
    for (k=0; k<K; k++){
    	U2 = I[k]*I[k] + Q[k]*Q[k];
    	AccumPowerMeasure(&(PoMe), U2);
		if (PoMe.Accumulators_are_ready != 0)
			DoPowerMeasure(&(PoMe));
		x_A2_est[k] = PoMe.x_A2_est[0];
		x_A_est[k] = PoMe.x_A_est;
		x_stdn2_est[k] = PoMe.x_stdn2_est;
		rough_qcno_dBHz[k] = PoMe.rough_qcno_dBHz;
		sum_counter[k] = (int)(PoMe.sum_counter);
		acum_counter[k] = (int)(PoMe.acum_counter);
		fail_counter[k] = (int)(PoMe.fail_counter);
		allow_stnd2_est[k] = (int)(PoMe.allow_stnd2_est);
//    	printf("PoMe.fail_counter = %d\n", fail_counter[k]);
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
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%u\n", x_stdn2_est[i]);
    }
    for (i=0; i<K; i++){
    	fprintf(fid_out, "%d\n", (int)(rough_qcno_dBHz[i]));
    }
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

    free(fail_counter);
    free(allow_stnd2_est);

    fclose(fid_out);

    printf("Runtime finished\n");
return 0;
}

