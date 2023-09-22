#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pynq_api.h"
#include "Funciones_HW.h"
#include "Funcion_Test.h"
#include "shake_hw.h"
#include "sha3.h"
#include "params.h"

void main(int argc, char** argv) {

	int DBG = 0;
	unsigned long long length;
	unsigned long long length_byte;
	unsigned long long length_out = SIZE_SHA3;

	int hex = 0;

	unsigned char *buf;
	unsigned char *file_name;

	buf			= malloc(sizeof(unsigned char) * 1000);
	file_name	= malloc(sizeof(unsigned char) * 1000);

	int f = 0;
	int v = 1;

	for (int arg = 1; arg < argc; arg++) {

		if (argv[arg][0] == '-') {
			if (argv[arg][1] == 'h') {
				printf("\n Usage: ./sha3_XXX_spirs [-h] [-v] [-m] [-f] [-l] \n");
				printf("\n -h: Show the help.");
				printf("\n -v: Verbose Level:");
				printf("\n \t 1: Show the Acceleration (By default).");
				printf("\n \t 2: Show the Acceleration + Output Data.");
				printf("\n \t 3: Show the Acceleration + Input/Output Data.");
				printf("\n -m: Input message (HEX format).");
				printf("\n -t: Input message (TXT format).");
				printf("\n -mf: Input HEX file (file).");
				printf("\n -tf: Input TXT file (file).");
				printf("\n -l: Length output. (By default 128 or 256)");
				printf("\n\n Results can also be checked in: https://emn178.github.io/online-tools/sha3_512.html");
				printf("\n \n");
				return;
			}
			else if (argv[arg][1] == 'm' & argv[arg][2] == 'f') {
				if (arg < argc - 1) memcpy(file_name, argv[arg + 1], sizeof(unsigned char) * 1000);
				f = 1;
				hex = 1;
			}
			else if (argv[arg][1] == 't' & argv[arg][2] == 'f') {
				if (arg < argc - 1) memcpy(file_name, argv[arg + 1], sizeof(unsigned char) * 1000);
				f = 1;
				hex = 0;
			}
			else if (argv[arg][1] == 'm') {
				if (arg < argc - 1) memcpy(buf,argv[arg + 1], sizeof(unsigned char) * 1000);
				f = 0;
				hex = 1;
			}
			else if (argv[arg][1] == 't') {
				if (arg < argc - 1) memcpy(buf, argv[arg + 1], sizeof(unsigned char) * 1000);
				f = 0;
				hex = 0;
			}
			else if (argv[arg][1] == 'l') {
				if (arg < argc - 1) length_out = atoi(argv[arg + 1]);
			}
			else if (argv[arg][1] == 'v') {
				if (arg < argc - 1) v = atoi(argv[arg + 1]);
			}
			else if (argv[arg][1] == 'd') {
				if (arg < argc - 1) DBG = atoi(argv[arg + 1]);
			}
			else {
				printf("\n Unknow option: %s\n", argv[arg]);
			}
		}
	}

	unsigned char *in;
	unsigned char *out_hw;

	in = malloc(sizeof(unsigned char) * SIZE_INPUT);
	out_hw = malloc(sizeof(unsigned char) * SIZE_OUTPUT * 1000);

	memset(in, 0, SIZE_INPUT);

	unsigned long long tic = 0, toc;
	unsigned long long time_hw = 0, time_sw = 0;

	unsigned char* in_sw;
	unsigned char* out_sw;

	in_sw = malloc(sizeof(unsigned char) * SIZE_INPUT);
	out_sw = malloc(sizeof(unsigned char) * SIZE_OUTPUT * 1000);

	sha3_ctx_t ctx;
	int mdlen = MDLEN; // 28 (SHA3-224), 32 (SHA3-256), 48 (SHA3-386), 64 (SHA3-512)

	load_bitstream(0, 64);
	PYNQ_MMIO_WINDOW ms2xl;
	PYNQ_createMMIOWindow(&ms2xl, MS2XL_BASEADDR, MS2XL_LENGTH);


	length = (unsigned long long)strlen(buf);

	int ind1, ind2;
	unsigned char character;

		// Get Data: Length & Input Data
	if (f) {
		input_file(in, &length_byte, file_name, hex, DBG);
	}
	else {
		if (hex) {

			for (int i = 0; i < length; i++) {
				int check = check_hex(buf[i]);
				if (check) {
					printf("\n\n ERROR: The message introduced is not in HEX format ... Exit.\n\n"); return;
				}
			}

			int mod = 0;

			for (int i = 0; i < (int)ceil((float)length / 2); i++) {
				ind1 = 2 * i - mod;
				ind2 = ind1 + 1;
				if (i == 0 & ((length % 2) != 0)) {
					char_to_hex('0', buf[ind1], &character);
					mod = 1;
				}
				else char_to_hex(buf[ind1], buf[ind2], &character);
				in[i] = character;
			}
			length_byte = 8 * (int)ceil((float)length / 2);
		}
		else {
			for (int i = 0; i < length; i++) in[i] = buf[i];
			length_byte = 8 * length;
		}
	}

	// SHA2 HW Execution
	tic = PYNQ_Wtime();
		shake_hw(in, out_hw, length_byte, length_out, ms2xl, DBG);
	time_hw = PYNQ_Wtime() - tic;

	// SHA2 SW Execution
	memcpy(in_sw, in, SIZE_INPUT);

	tic = PYNQ_Wtime();
		sha3_init(&ctx, MDLEN);
		shake_update(&ctx, in_sw, (length_byte / 8));
		shake_xof(&ctx);
		shake_out(&ctx, out_sw, (length_out/8));
	time_sw = PYNQ_Wtime() - tic;

	// Comparison of results 
	int pass_hw = 1;
	for (int i = 0; i < (int)(length_out/8); i++) if (out_hw[i] != out_sw[i]) pass_hw = 0;

	// Print Result
	if (v >= 1) {
		if(pass_hw) printf("\n \t -- Results of the Execution -- \n \n Msg. Length: %lld (bits) %lld (bytes) \t \t Acceleration: %.2f \t \t HW Pass: YES",
			8 * length_byte, length_byte, ((float)(time_sw) / (float)(time_hw)));
		else printf("\n \t -- Results of the Execution -- \n \n Msg. Length: %lld (bits) %lld (bytes) \t \t Acceleration: %.2f \t \t HW Pass: FAIL",
			8 * length_byte, length_byte, ((float)(time_sw) / (float)(time_hw)));
		printf("\n \n Hash Result: \t");	for (int i = 0; i < (length_out/8); i++) printf("%02x", out_hw[i]);
		printf("\n");
	}
	if (v >= 2) {
		printf("\n \t OUT_HW: \t");	for (int i = 0; i < (int)(length_out / 8); i++) printf("%02x", out_hw[i]);		printf("\t (%6llu us.)", time_hw);
		printf("\n \t OUT_SW: \t");	for (int i = 0; i < (int)(length_out / 8); i++) printf("%02x", out_sw[i]);		printf("\t (%6llu us.)", time_sw);
		printf("\n \t CHECK_HW: \t"); if (pass_hw) printf("OK"); else printf("FAIL");
	}
	if (v >= 3) {
		printf("\n \t IN: ");		for (int i = 0; i < length; i++) printf("%02x", in[i]);
		printf("\n");
	}
	
	printf("\n \n");
	
}
