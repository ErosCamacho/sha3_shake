#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pynq_api.h"
#include "params.h"

void load_bitstream(int DBG, int version) {

	unsigned long long tic = 0, toc;
	int Status;
	char bitstream_file[80];

	/*
	if(version == 32)	sprintf(bitstream_file, "%s", "BD_wrapper_32.bit");
	else				sprintf(bitstream_file, "%s", "BD_wrapper_64.bit");
	*/

	sprintf(bitstream_file, "%s", "BD_wrapper.bit");

	FILE* file;

	if (!(file = fopen(bitstream_file, "r"))) {
		printf("\n   Bitstream not available. Bye ...\n\n");
	}
	fclose(file);

	tic = PYNQ_Wtime();
		Status = PYNQ_loadBitstream(bitstream_file);
		if (Status != PYNQ_SUCCESS) {
			printf(" Failed Loading !!!\n");
		}
	toc = PYNQ_Wtime() - tic;
	if(DBG == 1) printf("\n Load Bitsream (HW) \t \t ... \t (%6llu us.) \n ", toc);
}


void msg_padding(unsigned char* in) {
	int len_input = SIZE_INPUT;
	int cero = 1;

	for (int i = SIZE_INPUT - 1; i >= 0; i = i - 1) {
		if (i - 1 >= 0) {
			if (in[i] == 0 && in[i - 1] != 0) {
				len_input = i - 1;
				cero = 0;
			}
		}
		else if (cero == 1) {
			if (in[i] != 0) len_input = i;
			else len_input = -1;
		}
	}
	in[len_input + 1] = 0x80; // El siguiente a la ultima posicion requiere un '1000 0000' -> Habria que modificarlo para distinto a 8 bits
	if (len_input * 8 < 256) in[SIZE_INPUT - 1] = (len_input + 1) * 8;  // La longitud de la cadena (len+1) por 8 bits de cada elemento genera el total de bits usados.
	else {
		in[SIZE_INPUT - 1] = (len_input + 1) * 8;
		in[SIZE_INPUT - 2] = ((len_input + 1) * 8) >> 8;
	}
}
