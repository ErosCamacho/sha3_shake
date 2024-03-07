#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shake_hw.h"
#include "params.h"
#include "math.h"

#include <pynq_api.h>

void shake_ms2xl_init(PYNQ_MMIO_WINDOW ms2xl) {
	unsigned long long int op;

	op = RESET; // RESET ON
	PYNQ_writeMMIO(&ms2xl, &op, CONTROL, sizeof(unsigned int));

	op = 0; // RESET OFF
	PYNQ_writeMMIO(&ms2xl, &op, CONTROL, sizeof(unsigned int));

}

void shake_ms2xl(unsigned long long int* a, unsigned long long int* b, PYNQ_MMIO_WINDOW ms2xl, unsigned long long int pos_pad, int pad, int read, int DBG) {

	unsigned long long int op;
	unsigned long long int end_op = 0;
	unsigned long long int reg_addr;
	unsigned long long int reg_data_in;
	unsigned long long int reg_data_out;
	unsigned long long tic = 0, toc;

	if (pad == 1 | pad == 2) {
		// ----------- LOAD PADDING ---------- //
		if (DBG == 2) {
			printf("  -- sha2_ms2xl - Loading data padding ...................... \n");
			tic = PYNQ_Wtime();
		}

		op = LOAD_PADDING; // LOAD
		if (DBG == 5) printf("\n CONTROL: %lld", op);
		PYNQ_writeMMIO(&ms2xl, &op, CONTROL, sizeof(unsigned long long int));

		reg_addr = (unsigned long long int)(0);
		reg_data_in = (unsigned long long int)(pos_pad);
		PYNQ_writeMMIO(&ms2xl, &reg_addr, ADDRESS, sizeof(unsigned long long int));
		PYNQ_writeMMIO(&ms2xl, &reg_data_in, DATA_IN, sizeof(unsigned long long int));
		if (DBG == 3) printf(" pos_pad: %lld\n\r", reg_data_in);
	}

	if (pad == 1)		op = LOAD + START_PADDING;
	else if (pad == 2)	op = LOAD + START_PADDING_DOUBLE;
	else if (pad == 3)	op = START_SHAKE;
	else				op = LOAD;

	if (DBG == 5) printf("\n CONTROL: %lld", op);
	PYNQ_writeMMIO(&ms2xl, &op, CONTROL, sizeof(unsigned long long int));

	if (pad != 3) {
		for (int i = 0; i < (SIZE_BLOCK / 64); i++) {
			reg_addr = (unsigned long long int)(i);
			reg_data_in = (unsigned long long int)(a[i]);
			PYNQ_writeMMIO(&ms2xl, &reg_addr, ADDRESS, sizeof(unsigned long long int));
			PYNQ_writeMMIO(&ms2xl, &reg_data_in, DATA_IN, sizeof(unsigned long long int));
			if (DBG == 3) printf(" a(%d): %02llx\n\r", i, a[i]);
		}

		if (DBG == 2) {
			toc = PYNQ_Wtime() - tic;
			printf("(%3llu us.)\n", toc);
		}
	
	}

	// ----------- OPERATING ------------- //
	if (DBG == 2) {
		printf("  -- sha2_ms2xl - Operating .............. \n");
		tic = PYNQ_Wtime();
	}

	op = START; // START
	if (DBG == 5) printf("\n CONTROL: %lld", op);
	PYNQ_writeMMIO(&ms2xl, &op, CONTROL, sizeof(unsigned long long int));

	// wait END_OP
	while (!end_op) PYNQ_readMMIO(&ms2xl, &end_op, END_OP, sizeof(unsigned long long int));

	if (DBG == 2) {
		toc = PYNQ_Wtime() - tic;
		printf("(%3llu us.)\n", toc);
	}

	// ----------- READ ------------- //
	if (pad & read) {
		if (DBG == 2) {
			printf("  -- sha2_ms2xl - Reading output .............................. \n");
			tic = PYNQ_Wtime();
		}

		op = READ; // READ
		if (DBG == 5) printf("\n CONTROL: %lld", op);
		PYNQ_writeMMIO(&ms2xl, &op, CONTROL, sizeof(unsigned long long int));

		if (DBG == 3) printf(" i_max: %f %f %d\n\r", ((double)SHAKE_OUT / (double)64), ceil((double)SHAKE_OUT / (double)64), (int)ceil((double)SHAKE_OUT / (double)64));

		for (int i = 0; i < (int)ceil((double)SHAKE_OUT / (double)64); i++) {
			reg_addr = (unsigned long long int)(i);
			PYNQ_writeMMIO(&ms2xl, &reg_addr, ADDRESS, sizeof(unsigned long long int));
			PYNQ_readMMIO(&ms2xl, &reg_data_out, DATA_OUT, sizeof(unsigned long long int));
			b[i] = reg_data_out;
			if (DBG == 3) printf(" b(%d): %02llx\n\r", i, b[i]);
		}

		if (DBG == 2) {
			toc = PYNQ_Wtime() - tic;
			printf("(%3llu us.)\n", toc);
		}
	}
}

void shake_hw(unsigned char* in, unsigned char* out, unsigned long long int length, unsigned long long int length_out, PYNQ_MMIO_WINDOW ms2xl, int DBG) {

	unsigned long long int hb_num;
	unsigned long long int pos_pad;
	unsigned long long int ind;
	int last_hb = 0;
	int read = 0;
	int ind_matrix = 0;

	unsigned long long int buffer_in[(SIZE_BLOCK / 64)];
	unsigned long long int buffer_out[(int)ceil((double)SHAKE_OUT / (double)64)];
	unsigned char prev_out[SIZE_OUTPUT];
	unsigned char prev_out_2[18000];
	unsigned long long int buf_1, buf_2;

	unsigned long long tic = 0, toc;

	//unsigned char OUT_MATRIX[1024][SIZE_OUTPUT]; // Bits max = 1024*SHAKE_OUT
	//int** OUT_MATRIX = (int**)malloc(1024 * sizeof(int*));
	//for (int i = 0; i < 1024; i++) OUT_MATRIX[i] = (int*)malloc(SHAKE_OUT * sizeof(unsigned char));


	// ------- Number of hash blocks ----- //
	hb_num = ((length+5) / SIZE_BLOCK) + 1; //3 bits for padding
	pos_pad = length % SIZE_BLOCK;

	if (DBG == 1) {
		printf("\n hb_num = %lld \n", hb_num);
		printf("\n length = %lld \n", length);
		printf("\n pos_pad = %lld \n", pos_pad);
	}

	// ------- SHAKE Initialization --------//

	shake_ms2xl_init(ms2xl);

	// ------- Operation ---------------- //
	for (unsigned long long int hb = 1; hb <= hb_num; hb++) {
		ind = (hb - 1) * (SIZE_BLOCK / 8);
		for (int i = 0; i < (SIZE_BLOCK / 64); i++) {
			if ((ind + 0) * 8 >= length) in[ind + 0] = 0x00;
			if ((ind + 1) * 8 >= length) in[ind + 1] = 0x00;
			if ((ind + 2) * 8 >= length) in[ind + 2] = 0x00;
			if ((ind + 3) * 8 >= length) in[ind + 3] = 0x00;
			if ((ind + 4) * 8 >= length) in[ind + 4] = 0x00;
			if ((ind + 5) * 8 >= length) in[ind + 5] = 0x00;
			if ((ind + 6) * 8 >= length) in[ind + 6] = 0x00;
			if ((ind + 7) * 8 >= length) in[ind + 7] = 0x00;

			buf_1 = ((unsigned long long int)in[ind + 7] << 56) + ((unsigned long long int)in[ind + 6] << 48) + ((unsigned long long int)in[ind + 5] << 40) + ((unsigned long long int)in[ind + 4] << 32);
			buf_2 = ((unsigned long long int)in[ind + 3] << 24) + ((unsigned long long int)in[ind + 2] << 16) + ((unsigned long long int)in[ind + 1] << 8) + ((unsigned long long int)in[ind + 0]);
			buffer_in[i] = buf_1 + buf_2;
			if (DBG == 1) printf("buf_1 = %02llx \n", buf_1);
			if (DBG == 1) printf("buf_2 = %02llx \n", buf_2);
			if (DBG == 1) printf("in[%lld] = %02x \t in[%lld] = %02x \t in[%lld] = %02x \t in[%lld] = %02x \n", ind, in[ind], ind + 1, in[ind + 1], ind + 2, in[ind + 2], ind + 3, in[ind + 3]);
			if (DBG == 1) printf("buffer_in[%d] = %02llx \n", i, buffer_in[i]);
			ind = ind + 8;
		}

		if (hb == hb_num)						last_hb = 1;
		else if (pos_pad > (SIZE_BLOCK - 4))	last_hb = 2; // Add a new block with mix padding
		else									last_hb = 0;

		if (DBG == 1) {
			printf("\n last_hb = %d \n", last_hb);
		}

		if (last_hb)	read = 1;
		else			read = 0;
		shake_ms2xl(buffer_in, buffer_out, ms2xl, pos_pad, last_hb, read, DBG);
	}

	// ------- Change Out Format --------- //
	if (read) {
		for (int i = 0; i < (int)ceil((double)SHAKE_OUT / (double)64); i++) {
			ind = i * 8;
			prev_out[ind + 7] = buffer_out[i] >> 56;
			prev_out[ind + 6] = buffer_out[i] >> 48 - (out[ind + 0] << 8);
			prev_out[ind + 5] = buffer_out[i] >> 40 - (out[ind + 0] << 16) - (out[ind + 1] << 8);
			prev_out[ind + 4] = buffer_out[i] >> 32 - (out[ind + 0] << 24) - (out[ind + 1] << 16) - (out[ind + 2] << 8);

			prev_out[ind + 3] = buffer_out[i] >> 24 - ((unsigned long long int)out[ind + 0] << 32) - (out[ind + 1] << 24) - (out[ind + 2] << 16) - (out[ind + 3] << 8);
			prev_out[ind + 2] = buffer_out[i] >> 16 - ((unsigned long long int)out[ind + 0] << 40) - ((unsigned long long int)out[ind + 1] << 32) - (out[ind + 2] << 24) - (out[ind + 3] << 16) - (out[ind + 4] << 8);
			prev_out[ind + 1] = buffer_out[i] >> 8 - ((unsigned long long int)out[ind + 0] << 48) - ((unsigned long long int)out[ind + 1] << 40) - ((unsigned long long int)out[ind + 2] << 32) - (out[ind + 3] << 24) - (out[ind + 4] << 16) - (out[ind + 5] << 8);
			prev_out[ind + 0] = buffer_out[i] - ((unsigned long long int)out[ind + 0] << 56) - ((unsigned long long int)out[ind + 1] << 48) - ((unsigned long long int)out[ind + 2] << 40) - ((unsigned long long int)out[ind + 3] << 32) - (out[ind + 4] << 24) - (out[ind + 5] << 16) - (out[ind + 6] << 8);
		}

		for (int i = 0; i < SIZE_OUTPUT; i++) {
			//OUT_MATRIX[ind_matrix][i] = prev_out[i];	
			prev_out_2[ind_matrix*SIZE_OUTPUT + i] = prev_out[i];
		}

		ind_matrix++;

	}



	// SHAKE Expansion
	if (length_out > SHAKE_OUT) {
		last_hb = 3;
		read = 1;
		unsigned long long int len;
		len = SHAKE_OUT;

		while (len < length_out) {

			shake_ms2xl(buffer_in, buffer_out, ms2xl, pos_pad, last_hb, read, DBG);

			for (int i = 0; i < (int)ceil((double)SHAKE_OUT / (double)64); i++) {
				ind = i * 8;
				prev_out[ind + 7] = buffer_out[i] >> 56;
				prev_out[ind + 6] = buffer_out[i] >> 48 - (out[ind + 0] << 8);
				prev_out[ind + 5] = buffer_out[i] >> 40 - (out[ind + 0] << 16) - (out[ind + 1] << 8);
				prev_out[ind + 4] = buffer_out[i] >> 32 - (out[ind + 0] << 24) - (out[ind + 1] << 16) - (out[ind + 2] << 8);

				prev_out[ind + 3] = buffer_out[i] >> 24 - ((unsigned long long int)out[ind + 0] << 32) - (out[ind + 1] << 24) - (out[ind + 2] << 16) - (out[ind + 3] << 8);
				prev_out[ind + 2] = buffer_out[i] >> 16 - ((unsigned long long int)out[ind + 0] << 40) - ((unsigned long long int)out[ind + 1] << 32) - (out[ind + 2] << 24) - (out[ind + 3] << 16) - (out[ind + 4] << 8);
				prev_out[ind + 1] = buffer_out[i] >> 8 - ((unsigned long long int)out[ind + 0] << 48) - ((unsigned long long int)out[ind + 1] << 40) - ((unsigned long long int)out[ind + 2] << 32) - (out[ind + 3] << 24) - (out[ind + 4] << 16) - (out[ind + 5] << 8);
				prev_out[ind + 0] = buffer_out[i] - ((unsigned long long int)out[ind + 0] << 56) - ((unsigned long long int)out[ind + 1] << 48) - ((unsigned long long int)out[ind + 2] << 40) - ((unsigned long long int)out[ind + 3] << 32) - (out[ind + 4] << 24) - (out[ind + 5] << 16) - (out[ind + 6] << 8);
			}

			for (int i = 0; i < SIZE_OUTPUT; i++) {
				//OUT_MATRIX[ind_matrix][i] = prev_out[i];
				prev_out_2[ind_matrix * SIZE_OUTPUT + i] = prev_out[i];
			}

			ind_matrix++;

			len = len + SHAKE_OUT;
		}


	}

	// SHAKE output
	ind_matrix = 0;
	for (int j = 0; j < (int)ceil((double)length_out / (double)8); j = j + 1) {
		//if (j == (ind_matrix + 1) * SIZE_OUTPUT) ind_matrix++;

		//out[j] = OUT_MATRIX[ind_matrix][j - (ind_matrix*SIZE_OUTPUT)];
		out[j] = prev_out_2[j];

		if (DBG == 4) printf("\n ind_matrix: %d \t j: %d", ind_matrix, j);

		//printf("\n %d %d %d", j, SIZE_OUTPUT, (j % SIZE_OUTPUT));
		//printf("\n ind_matrix: %d %d %02x %02x", ind_matrix, j, out[j], OUT_MATRIX[ind_matrix][j]);
	}

	if (DBG == 4) {
		for (int j = 0; j < SIZE_OUTPUT; j = j + 1)
			printf("\n %d: %x", j, prev_out_2[j]);
	}
	


}