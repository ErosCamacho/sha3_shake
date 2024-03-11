#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pynq_api.h"
#include "Funciones_HW.h"
#include "Funcion_Test.h"
#include "../hw/sha3_hw.h"
#include "../hw/params.h"

void reset_in(unsigned char* INPUT) {
	for (int i = 0; i < SIZE_INPUT; i++) INPUT[i] = 0x00;
}

void input_nist(unsigned char* INPUT, unsigned char* OUTPUT, unsigned long long int* LEN, int sel, int b, int DBG) {

	FILE* rsp;
	int selection; 

	char file_byte_short[100];
	char file_byte_long[100];
	char file_bit_short[100];
	char file_bit_long[100];
	char sha[4];

	sprintf(file_byte_short, "%s", "NIST_TestVector/sha-3bytetestvectors/SHA3_");
	sprintf(file_byte_long, "%s", "NIST_TestVector/sha-3bytetestvectors/SHA3_");
	sprintf(file_bit_short, "%s", "NIST_TestVector/sha-3bittestvectors/SHA3_");
	sprintf(file_bit_long, "%s", "NIST_TestVector/sha-3bittestvectors/SHA3_");

	sprintf(sha, "%d", SIZE_SHA3);
	strcat(file_byte_short, sha);
	strcat(file_byte_long, sha);
	strcat(file_bit_short, sha);
	strcat(file_bit_long, sha);

	strcat(file_byte_short, "ShortMsg.rsp");
	strcat(file_byte_long, "LongMsg.rsp");
	strcat(file_bit_short, "ShortMsg.rsp");
	strcat(file_bit_long, "LongMsg.rsp");



	if (b == 1) {
		if (sel < ((SIZE_BLOCK / 8) + 1)) {
			rsp = fopen(file_byte_short, "r");
			selection = sel;
		}
		else {
			rsp = fopen(file_byte_long, "r");
			selection = sel - ((SIZE_BLOCK / 8) + 1);
		}
	}
	else {
		if (sel < (SIZE_BLOCK + 1)) {
			rsp = fopen(file_bit_short, "r");
			selection = sel;
		}
		else {
			rsp = fopen(file_bit_long, "r");
			selection = sel - (SIZE_BLOCK + 1);
		}
	}

	/*
	char line[MAX_LINE_LENGTH] = { 0 };
	char line_buffer[MAX_LINE_LENGTH] = { 0 };
	unsigned char data_in[SIZE_INPUT];
	*/

	unsigned char* line;
	line = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	unsigned char* line_buffer;
	line_buffer = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line_buffer, 0, MAX_LINE_LENGTH);

	unsigned char* data_in;
	data_in = malloc(sizeof(unsigned char) * SIZE_INPUT);
	memset(data_in, 0, SIZE_INPUT);

	unsigned char data_out[SIZE_OUTPUT];
	int line_count = -8; // Necesario para eliminar las cabeceras de los archivos
	int ind1, ind2;

	memset(data_in, 0, SIZE_INPUT);
	memset(data_out, 0, SIZE_OUTPUT);

	while (fgets(line, MAX_LINE_LENGTH, rsp))
	{
		/* Print each line */
		if (DBG == 4) printf("line[%06d]: %s", ++line_count, line);
		else ++line_count;

		//printf("\n %d %d %d %d", line_count, line_count % 4, line_count / 4, sel);

		// Obtener LENGTH
		if (((line_count % 4) == 0) && ((line_count / 4) == selection)) {
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 5) line_buffer[i - 6] = line[i];
			}
			*LEN = atoll(line_buffer);
			if (DBG == 3) printf("\n LEN: %lld", *LEN);
		}

		// Obtener DATA_INPUT
		else if (((line_count % 4) == 1) && ((line_count / 4) == selection)) {
			if (DBG == 3) printf("\n data_in: ");
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 5) line_buffer[i - 6] = line[i];
			}
			int ce = ceil((float)*LEN / 8);
			char buf;
			for (int i = 0; i < ce; i++) {
					ind1 = 2 * i;
					ind2 = ind1 + 1;
					char_to_hex(line_buffer[ind1], line_buffer[ind2], &buf);
					data_in[i] = buf;
					if (DBG == 3) printf("%02x", data_in[i]);
			}
			memcpy(INPUT, data_in, sizeof(unsigned char) * ce);

		}

		// Obtener DATA_OUTPUT
		else if (((line_count % 4) == 2) && ((line_count / 4) == selection)) {
			if (DBG == 3) printf("\n data_out: ");
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 4) line_buffer[i - 5] = line[i];
			}
			for (int i = 0; i < SIZE_OUTPUT; i++) {
				ind1 = 2 * i;
				ind2 = ind1 + 1;
				char buf;
				char_to_hex(line_buffer[ind1], line_buffer[ind2], &buf);
				data_out[i] = buf;
				if (DBG == 3) printf("%02x", data_out[i]);
			}

			memcpy(OUTPUT, data_out, sizeof(data_out));


		}

		/* Add a trailing newline to lines that don't already have one */
		if (DBG == 3) {
			if (line[strlen(line) - 1] != '\n')
				printf("\n");
		}
	}
	fclose(rsp);
}

void input_nist_shake(unsigned char* INPUT, unsigned char* OUTPUT, unsigned long long int* LEN, int sel, int b, int DBG) {

	FILE* rsp;
	int selection;

	char file_byte_short[100];
	char file_byte_long[100];
	char file_bit_short[100];
	char file_bit_long[100];
	char sha[4];

	sprintf(file_byte_short, "%s", "NIST_TestVector/shakebytetestvectors/SHAKE");
	sprintf(file_byte_long, "%s", "NIST_TestVector/shakebytetestvectors/SHAKE");
	sprintf(file_bit_short, "%s", "NIST_TestVector/shakebittestvectors/SHAKE");
	sprintf(file_bit_long, "%s", "NIST_TestVector/shakebittestvectors/SHAKE");

	sprintf(sha, "%d", SIZE_SHA3);
	strcat(file_byte_short, sha);
	strcat(file_byte_long, sha);
	strcat(file_bit_short, sha);
	strcat(file_bit_long, sha);

	strcat(file_byte_short, "ShortMsg.rsp");
	strcat(file_byte_long, "LongMsg.rsp");
	strcat(file_bit_short, "ShortMsg.rsp");
	strcat(file_bit_long, "LongMsg.rsp");



	if (b == 1) {
		if (sel < ((SIZE_BLOCK / 8) + 1)) {
			rsp = fopen(file_byte_short, "r");
			selection = sel;
		}
		else {
			rsp = fopen(file_byte_long, "r");
			selection = sel - ((SIZE_BLOCK / 8) + 1);
		}
	}
	else {
		if (sel < (SIZE_BLOCK + 1)) {
			rsp = fopen(file_bit_short, "r");
			selection = sel;
		}
		else {
			rsp = fopen(file_bit_long, "r");
			selection = sel - (SIZE_BLOCK + 1);
		}
	}

	unsigned char* line;
	line = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	unsigned char* line_buffer;
	line_buffer = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line_buffer, 0, MAX_LINE_LENGTH);

	unsigned char* data_in;
	data_in = malloc(sizeof(unsigned char) * SIZE_INPUT);
	memset(data_in, 0, SIZE_INPUT);

	unsigned char data_out[SIZE_OUTPUT];
	int line_count = -8; // Necesario para eliminar las cabeceras de los archivos
	int ind1, ind2;

	memset(data_in, 0, SIZE_INPUT);
	memset(data_out, 0, SIZE_OUTPUT);

	while (fgets(line, MAX_LINE_LENGTH, rsp))
	{
		/* Print each line */
		if (DBG == 4) printf("line[%06d]: %s", ++line_count, line);
		else ++line_count;

		//printf("\n %d %d %d %d", line_count, line_count % 4, line_count / 4, sel);

		// Obtener LENGTH
		if (((line_count % 4) == 0) && ((line_count / 4) == selection)) {
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 5) line_buffer[i - 6] = line[i];
			}
			*LEN = atoll(line_buffer);
			if (DBG == 3) printf("\n LEN: %lld", *LEN);
		}

		// Obtener DATA_INPUT
		else if (((line_count % 4) == 1) && ((line_count / 4) == selection)) {
			if (DBG == 3) printf("\n data_in: ");
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 5) line_buffer[i - 6] = line[i];
			}
			int ce = ceil((float)*LEN / 8);
			char buf;
			for (int i = 0; i < ce; i++) {
				ind1 = 2 * i;
				ind2 = ind1 + 1;
				char_to_hex(line_buffer[ind1], line_buffer[ind2], &buf);
				data_in[i] = buf;
				if (DBG == 3) printf("%02x", data_in[i]);
			}
			memcpy(INPUT, data_in, sizeof(unsigned char) * ce);

		}

		// Obtener DATA_OUTPUT
		else if (((line_count % 4) == 2) && ((line_count / 4) == selection)) {
			if (DBG == 3) printf("\n data_out: ");
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 8) line_buffer[i - 9] = line[i];
			}
			for (int i = 0; i < SIZE_OUTPUT; i++) {
				ind1 = 2 * i;
				ind2 = ind1 + 1;
				char buf;
				char_to_hex(line_buffer[ind1], line_buffer[ind2], &buf);
				data_out[i] = buf;
				if (DBG == 3) printf("%02x", data_out[i]);
			}

			memcpy(OUTPUT, data_out, sizeof(data_out));


		}

		/* Add a trailing newline to lines that don't already have one */
		if (DBG == 3) {
			if (line[strlen(line) - 1] != '\n')
				printf("\n");
		}
	}
	fclose(rsp);
}

void input_nist_shake_varout(unsigned char* INPUT, unsigned char* OUTPUT, unsigned long long int* LEN, int sel, int b, int DBG) {

	FILE* rsp;
	int selection;

	char file_byte[100];
	char file_bit[100];
	char sha[4];

	sprintf(file_byte, "%s", "NIST_TestVector/shakebytetestvectors/");
	sprintf(file_bit, "%s", "NIST_TestVector/shakebittestvectors/");

	sprintf(sha, "%d", SIZE_SHA3);
	strcat(file_byte, sha);
	strcat(file_bit, sha);

	strcat(file_byte, "VariableOut.rsp");
	strcat(file_bit, "VariableOut.rsp");

	rsp = fopen(file_byte, "r");

	if (b == 1) {
		rsp = fopen(file_byte, "r");
	}
	else {
		rsp = fopen(file_bit, "r");
	}

	selection = sel;

	unsigned char* line;
	line = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	unsigned char* line_buffer;
	line_buffer = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line_buffer, 0, MAX_LINE_LENGTH);

	unsigned char* data_in;
	data_in = malloc(sizeof(unsigned char) * SIZE_INPUT);
	memset(data_in, 0, SIZE_INPUT);

	unsigned char data_out[SIZE_OUTPUT];
	int line_count = -10; // Necesario para eliminar las cabeceras de los archivos
	int ind1, ind2;

	memset(data_in, 0, SIZE_INPUT);
	memset(data_out, 0, SIZE_OUTPUT);

	while (fgets(line, MAX_LINE_LENGTH, rsp))
	{
		/* Print each line */
		if (DBG == 4) printf("line[%06d]: %s", ++line_count, line);
		else ++line_count;

		//printf("\n %d %d %d %d", line_count, line_count % 4, line_count / 4, sel);

		// Obtener LENGTH
		if (((line_count % 5) == 1) && ((line_count / 5) == selection)) {
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 11) line_buffer[i - 12] = line[i];
			}
			*LEN = atoll(line_buffer);
			if (DBG == 3) printf("\n LEN: %lld", *LEN);
		}

		// Obtener DATA_INPUT
		else if (((line_count % 5) == 2) && ((line_count / 5) == selection)) {
			if (DBG == 3) printf("\n data_in: ");
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 5) line_buffer[i - 6] = line[i];
			}
			int ce = SIZE_SHA3 / 8;
			char buf;
			for (int i = 0; i < ce; i++) {
				ind1 = 2 * i;
				ind2 = ind1 + 1;
				char_to_hex(line_buffer[ind1], line_buffer[ind2], &buf);
				data_in[i] = buf;
				if (DBG == 3) printf("%02x", data_in[i]);
			}
			memcpy(INPUT, data_in, sizeof(unsigned char) * ce);

		}

		// Obtener DATA_OUTPUT
		else if (((line_count % 5) == 3) && ((line_count / 5) == selection)) {
			if (DBG == 3) printf("\n data_out: ");
			for (int i = 0; i < MAX_LINE_LENGTH; i++) {
				if (i > 8) line_buffer[i - 9] = line[i];
			}
			for (int i = 0; i < SIZE_OUTPUT; i++) {
				ind1 = 2 * i;
				ind2 = ind1 + 1;
				char buf;
				char_to_hex(line_buffer[ind1], line_buffer[ind2], &buf);
				data_out[i] = buf;
				if (DBG == 3) printf("%02x", data_out[i]);
			}

			memcpy(OUTPUT, data_out, sizeof(data_out));


		}

		/* Add a trailing newline to lines that don't already have one */
		if (DBG == 3) {
			if (line[strlen(line) - 1] != '\n')
				printf("\n");
		}
	}
	fclose(rsp);
}

void input_file(unsigned char* INPUT, unsigned long long int* LEN, unsigned char* FILE_NAME, int hex, int DBG) {

	FILE* rsp;
	int selection;
	rsp = fopen(FILE_NAME, "r");
	if (rsp == NULL) {
		printf("\n\n ERROR: No file found with this name ... Exit.\n\n"); return;
	}

	/*
	unsigned char line[MAX_LINE_LENGTH] = { 0 };
	unsigned char line_buffer[MAX_LINE_LENGTH] = { 0 };
	unsigned char data_in[SIZE_INPUT];
	*/
	unsigned char* line;
	line = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	unsigned char* line_buffer;
	line_buffer = malloc(sizeof(unsigned char) * MAX_LINE_LENGTH);
	memset(line_buffer, 0, MAX_LINE_LENGTH);

	unsigned char* data_in;
	data_in = malloc(sizeof(unsigned char) * SIZE_INPUT);
	memset(data_in, 0, SIZE_INPUT);

	unsigned char data_out[SIZE_OUTPUT];
	unsigned char character;
	int line_count = 0;
	int ind1, ind2;

	memset(data_out, 0, SIZE_OUTPUT);

	while (fgets(line, MAX_LINE_LENGTH, rsp))
	{
		/* Print each line */
		if (DBG == 4) printf("line[%06d]: %s", ++line_count, line);
		else ++line_count;

		unsigned long long length = (unsigned long long)strlen(line);
		//printf("\n length: %lld", length);

		// Obtener DATA_INPUT
		if (DBG == 3) printf("\n data_in: \n");
		for (int i = 0; i < length; i++) {
			line_buffer[i] = line[i];
		}

		if (hex) {

			for (int i = 0; i < length; i++) {
				int check = check_hex(line_buffer[i]);
				if (check) {
					printf("\n\n ERROR: The message introduced is not in HEX format ... Exit.\n\n"); return;
				}
			}

			int mod = 0;

			for (int i = 0; i < (int)ceil((float)length / 2); i++) {
				ind1 = 2 * i - mod;
				ind2 = ind1 + 1;
				if (i == 0 & ((length % 2) != 0)) {
					char_to_hex('0', line_buffer[ind1], &character);
					mod = 1;
				}
				else char_to_hex(line_buffer[ind1], line_buffer[ind2], &character);
				data_in[i] = character;
				if (DBG == 3) {
					printf("%c %c %02x %02x", line_buffer[ind1], line_buffer[ind2], character, data_in[i]);
					getchar();
				}
			}

			*LEN = 8 * (int)ceil((float)length / 2);
			memcpy(INPUT, data_in, sizeof(unsigned char) * (int)ceil((float)length / 2));
		}
		else {
			*LEN = 8 * length;
			memcpy(INPUT, line_buffer, sizeof(line_buffer));
		}

		/* Add a trailing newline to lines that don't already have one */
		if (DBG == 3) {
			if (line[strlen(line) - 1] != '\n')
				printf("\n");
		}
	}
	fclose(rsp);
}

void char_to_hex(unsigned char in0, unsigned char in1, unsigned char* out) {

	switch (in0) {
	case '0':
		switch (in1) {
		case '0': *out = 0x00; break;
		case '1': *out = 0x01; break;
		case '2': *out = 0x02; break;
		case '3': *out = 0x03; break;
		case '4': *out = 0x04; break;
		case '5': *out = 0x05; break;
		case '6': *out = 0x06; break;
		case '7': *out = 0x07; break;
		case '8': *out = 0x08; break;
		case '9': *out = 0x09; break;
		case 'a': *out = 0x0a; break;
		case 'b': *out = 0x0b; break;
		case 'c': *out = 0x0c; break;
		case 'd': *out = 0x0d; break;
		case 'e': *out = 0x0e; break;
		case 'f': *out = 0x0f; break;
		case 'A': *out = 0x0a; break;
		case 'B': *out = 0x0b; break;
		case 'C': *out = 0x0c; break;
		case 'D': *out = 0x0d; break;
		case 'E': *out = 0x0e; break;
		case 'F': *out = 0x0f; break;
		} break;
	case '1':
		switch (in1) {
		case '0': *out = 0x10; break;
		case '1': *out = 0x11; break;
		case '2': *out = 0x12; break;
		case '3': *out = 0x13; break;
		case '4': *out = 0x14; break;
		case '5': *out = 0x15; break;
		case '6': *out = 0x16; break;
		case '7': *out = 0x17; break;
		case '8': *out = 0x18; break;
		case '9': *out = 0x19; break;
		case 'a': *out = 0x1a; break;
		case 'b': *out = 0x1b; break;
		case 'c': *out = 0x1c; break;
		case 'd': *out = 0x1d; break;
		case 'e': *out = 0x1e; break;
		case 'f': *out = 0x1f; break;
		case 'A': *out = 0x1a; break;
		case 'B': *out = 0x1b; break;
		case 'C': *out = 0x1c; break;
		case 'D': *out = 0x1d; break;
		case 'E': *out = 0x1e; break;
		case 'F': *out = 0x1f; break;
		} break;
	case '2':
		switch (in1) {
		case '0': *out = 0x20; break;
		case '1': *out = 0x21; break;
		case '2': *out = 0x22; break;
		case '3': *out = 0x23; break;
		case '4': *out = 0x24; break;
		case '5': *out = 0x25; break;
		case '6': *out = 0x26; break;
		case '7': *out = 0x27; break;
		case '8': *out = 0x28; break;
		case '9': *out = 0x29; break;
		case 'a': *out = 0x2a; break;
		case 'b': *out = 0x2b; break;
		case 'c': *out = 0x2c; break;
		case 'd': *out = 0x2d; break;
		case 'e': *out = 0x2e; break;
		case 'f': *out = 0x2f; break;
		case 'A': *out = 0x2a; break;
		case 'B': *out = 0x2b; break;
		case 'C': *out = 0x2c; break;
		case 'D': *out = 0x2d; break;
		case 'E': *out = 0x2e; break;
		case 'F': *out = 0x2f; break;
		} break;
	case '3':
		switch (in1) {
		case '0': *out = 0x30; break;
		case '1': *out = 0x31; break;
		case '2': *out = 0x32; break;
		case '3': *out = 0x33; break;
		case '4': *out = 0x34; break;
		case '5': *out = 0x35; break;
		case '6': *out = 0x36; break;
		case '7': *out = 0x37; break;
		case '8': *out = 0x38; break;
		case '9': *out = 0x39; break;
		case 'a': *out = 0x3a; break;
		case 'b': *out = 0x3b; break;
		case 'c': *out = 0x3c; break;
		case 'd': *out = 0x3d; break;
		case 'e': *out = 0x3e; break;
		case 'f': *out = 0x3f; break;
		case 'A': *out = 0x3a; break;
		case 'B': *out = 0x3b; break;
		case 'C': *out = 0x3c; break;
		case 'D': *out = 0x3d; break;
		case 'E': *out = 0x3e; break;
		case 'F': *out = 0x3f; break;
		} break;
	case '4':
		switch (in1) {
		case '0': *out = 0x40; break;
		case '1': *out = 0x41; break;
		case '2': *out = 0x42; break;
		case '3': *out = 0x43; break;
		case '4': *out = 0x44; break;
		case '5': *out = 0x45; break;
		case '6': *out = 0x46; break;
		case '7': *out = 0x47; break;
		case '8': *out = 0x48; break;
		case '9': *out = 0x49; break;
		case 'a': *out = 0x4a; break;
		case 'b': *out = 0x4b; break;
		case 'c': *out = 0x4c; break;
		case 'd': *out = 0x4d; break;
		case 'e': *out = 0x4e; break;
		case 'f': *out = 0x4f; break;
		case 'A': *out = 0x4a; break;
		case 'B': *out = 0x4b; break;
		case 'C': *out = 0x4c; break;
		case 'D': *out = 0x4d; break;
		case 'E': *out = 0x4e; break;
		case 'F': *out = 0x4f; break;
		} break;
	case '5':
		switch (in1) {
		case '0': *out = 0x50; break;
		case '1': *out = 0x51; break;
		case '2': *out = 0x52; break;
		case '3': *out = 0x53; break;
		case '4': *out = 0x54; break;
		case '5': *out = 0x55; break;
		case '6': *out = 0x56; break;
		case '7': *out = 0x57; break;
		case '8': *out = 0x58; break;
		case '9': *out = 0x59; break;
		case 'a': *out = 0x5a; break;
		case 'b': *out = 0x5b; break;
		case 'c': *out = 0x5c; break;
		case 'd': *out = 0x5d; break;
		case 'e': *out = 0x5e; break;
		case 'f': *out = 0x5f; break;
		case 'A': *out = 0x5a; break;
		case 'B': *out = 0x5b; break;
		case 'C': *out = 0x5c; break;
		case 'D': *out = 0x5d; break;
		case 'E': *out = 0x5e; break;
		case 'F': *out = 0x5f; break;
		} break;
	case '6':
		switch (in1) {
		case '0': *out = 0x60; break;
		case '1': *out = 0x61; break;
		case '2': *out = 0x62; break;
		case '3': *out = 0x63; break;
		case '4': *out = 0x64; break;
		case '5': *out = 0x65; break;
		case '6': *out = 0x66; break;
		case '7': *out = 0x67; break;
		case '8': *out = 0x68; break;
		case '9': *out = 0x69; break;
		case 'a': *out = 0x6a; break;
		case 'b': *out = 0x6b; break;
		case 'c': *out = 0x6c; break;
		case 'd': *out = 0x6d; break;
		case 'e': *out = 0x6e; break;
		case 'f': *out = 0x6f; break;
		case 'A': *out = 0x6a; break;
		case 'B': *out = 0x6b; break;
		case 'C': *out = 0x6c; break;
		case 'D': *out = 0x6d; break;
		case 'E': *out = 0x6e; break;
		case 'F': *out = 0x6f; break;
		} break;
	case '7':
		switch (in1) {
		case '0': *out = 0x70; break;
		case '1': *out = 0x71; break;
		case '2': *out = 0x72; break;
		case '3': *out = 0x73; break;
		case '4': *out = 0x74; break;
		case '5': *out = 0x75; break;
		case '6': *out = 0x76; break;
		case '7': *out = 0x77; break;
		case '8': *out = 0x78; break;
		case '9': *out = 0x79; break;
		case 'a': *out = 0x7a; break;
		case 'b': *out = 0x7b; break;
		case 'c': *out = 0x7c; break;
		case 'd': *out = 0x7d; break;
		case 'e': *out = 0x7e; break;
		case 'f': *out = 0x7f; break;
		case 'A': *out = 0x7a; break;
		case 'B': *out = 0x7b; break;
		case 'C': *out = 0x7c; break;
		case 'D': *out = 0x7d; break;
		case 'E': *out = 0x7e; break;
		case 'F': *out = 0x7f; break;
		} break;
	case '8':
		switch (in1) {
		case '0': *out = 0x80; break;
		case '1': *out = 0x81; break;
		case '2': *out = 0x82; break;
		case '3': *out = 0x83; break;
		case '4': *out = 0x84; break;
		case '5': *out = 0x85; break;
		case '6': *out = 0x86; break;
		case '7': *out = 0x87; break;
		case '8': *out = 0x88; break;
		case '9': *out = 0x89; break;
		case 'a': *out = 0x8a; break;
		case 'b': *out = 0x8b; break;
		case 'c': *out = 0x8c; break;
		case 'd': *out = 0x8d; break;
		case 'e': *out = 0x8e; break;
		case 'f': *out = 0x8f; break;
		case 'A': *out = 0x8a; break;
		case 'B': *out = 0x8b; break;
		case 'C': *out = 0x8c; break;
		case 'D': *out = 0x8d; break;
		case 'E': *out = 0x8e; break;
		case 'F': *out = 0x8f; break;
		} break;
	case '9':
		switch (in1) {
		case '0': *out = 0x90; break;
		case '1': *out = 0x91; break;
		case '2': *out = 0x92; break;
		case '3': *out = 0x93; break;
		case '4': *out = 0x94; break;
		case '5': *out = 0x95; break;
		case '6': *out = 0x96; break;
		case '7': *out = 0x97; break;
		case '8': *out = 0x98; break;
		case '9': *out = 0x99; break;
		case 'a': *out = 0x9a; break;
		case 'b': *out = 0x9b; break;
		case 'c': *out = 0x9c; break;
		case 'd': *out = 0x9d; break;
		case 'e': *out = 0x9e; break;
		case 'f': *out = 0x9f; break;
		case 'A': *out = 0x9a; break;
		case 'B': *out = 0x9b; break;
		case 'C': *out = 0x9c; break;
		case 'D': *out = 0x9d; break;
		case 'E': *out = 0x9e; break;
		case 'F': *out = 0x9f; break;
		} break;
	case 'a':
		switch (in1) {
		case '0': *out = 0xa0; break;
		case '1': *out = 0xa1; break;
		case '2': *out = 0xa2; break;
		case '3': *out = 0xa3; break;
		case '4': *out = 0xa4; break;
		case '5': *out = 0xa5; break;
		case '6': *out = 0xa6; break;
		case '7': *out = 0xa7; break;
		case '8': *out = 0xa8; break;
		case '9': *out = 0xa9; break;
		case 'a': *out = 0xaa; break;
		case 'b': *out = 0xab; break;
		case 'c': *out = 0xac; break;
		case 'd': *out = 0xad; break;
		case 'e': *out = 0xae; break;
		case 'f': *out = 0xaf; break;
		case 'A': *out = 0xaa; break;
		case 'B': *out = 0xab; break;
		case 'C': *out = 0xac; break;
		case 'D': *out = 0xad; break;
		case 'E': *out = 0xae; break;
		case 'F': *out = 0xaf; break;
		} break;
	case 'b':
		switch (in1) {
		case '0': *out = 0xb0; break;
		case '1': *out = 0xb1; break;
		case '2': *out = 0xb2; break;
		case '3': *out = 0xb3; break;
		case '4': *out = 0xb4; break;
		case '5': *out = 0xb5; break;
		case '6': *out = 0xb6; break;
		case '7': *out = 0xb7; break;
		case '8': *out = 0xb8; break;
		case '9': *out = 0xb9; break;
		case 'a': *out = 0xba; break;
		case 'b': *out = 0xbb; break;
		case 'c': *out = 0xbc; break;
		case 'd': *out = 0xbd; break;
		case 'e': *out = 0xbe; break;
		case 'f': *out = 0xbf; break;
		case 'A': *out = 0xba; break;
		case 'B': *out = 0xbb; break;
		case 'C': *out = 0xbc; break;
		case 'D': *out = 0xbd; break;
		case 'E': *out = 0xbe; break;
		case 'F': *out = 0xbf; break;
		} break;
	case 'c':
		switch (in1) {
		case '0': *out = 0xc0; break;
		case '1': *out = 0xc1; break;
		case '2': *out = 0xc2; break;
		case '3': *out = 0xc3; break;
		case '4': *out = 0xc4; break;
		case '5': *out = 0xc5; break;
		case '6': *out = 0xc6; break;
		case '7': *out = 0xc7; break;
		case '8': *out = 0xc8; break;
		case '9': *out = 0xc9; break;
		case 'a': *out = 0xca; break;
		case 'b': *out = 0xcb; break;
		case 'c': *out = 0xcc; break;
		case 'd': *out = 0xcd; break;
		case 'e': *out = 0xce; break;
		case 'f': *out = 0xcf; break;
		case 'A': *out = 0xca; break;
		case 'B': *out = 0xcb; break;
		case 'C': *out = 0xcc; break;
		case 'D': *out = 0xcd; break;
		case 'E': *out = 0xce; break;
		case 'F': *out = 0xcf; break;
		} break;
	case 'd':
		switch (in1) {
		case '0': *out = 0xd0; break;
		case '1': *out = 0xd1; break;
		case '2': *out = 0xd2; break;
		case '3': *out = 0xd3; break;
		case '4': *out = 0xd4; break;
		case '5': *out = 0xd5; break;
		case '6': *out = 0xd6; break;
		case '7': *out = 0xd7; break;
		case '8': *out = 0xd8; break;
		case '9': *out = 0xd9; break;
		case 'a': *out = 0xda; break;
		case 'b': *out = 0xdb; break;
		case 'c': *out = 0xdc; break;
		case 'd': *out = 0xdd; break;
		case 'e': *out = 0xde; break;
		case 'f': *out = 0xdf; break;
		case 'A': *out = 0xda; break;
		case 'B': *out = 0xdb; break;
		case 'C': *out = 0xdc; break;
		case 'D': *out = 0xdd; break;
		case 'E': *out = 0xde; break;
		case 'F': *out = 0xdf; break;
		} break;
	case 'e':
		switch (in1) {
		case '0': *out = 0xe0; break;
		case '1': *out = 0xe1; break;
		case '2': *out = 0xe2; break;
		case '3': *out = 0xe3; break;
		case '4': *out = 0xe4; break;
		case '5': *out = 0xe5; break;
		case '6': *out = 0xe6; break;
		case '7': *out = 0xe7; break;
		case '8': *out = 0xe8; break;
		case '9': *out = 0xe9; break;
		case 'a': *out = 0xea; break;
		case 'b': *out = 0xeb; break;
		case 'c': *out = 0xec; break;
		case 'd': *out = 0xed; break;
		case 'e': *out = 0xee; break;
		case 'f': *out = 0xef; break;
		case 'A': *out = 0xea; break;
		case 'B': *out = 0xeb; break;
		case 'C': *out = 0xec; break;
		case 'D': *out = 0xed; break;
		case 'E': *out = 0xee; break;
		case 'F': *out = 0xef; break;
		} break;
	case 'f':
		switch (in1) {
		case '0': *out = 0xf0; break;
		case '1': *out = 0xf1; break;
		case '2': *out = 0xf2; break;
		case '3': *out = 0xf3; break;
		case '4': *out = 0xf4; break;
		case '5': *out = 0xf5; break;
		case '6': *out = 0xf6; break;
		case '7': *out = 0xf7; break;
		case '8': *out = 0xf8; break;
		case '9': *out = 0xf9; break;
		case 'a': *out = 0xfa; break;
		case 'b': *out = 0xfb; break;
		case 'c': *out = 0xfc; break;
		case 'd': *out = 0xfd; break;
		case 'e': *out = 0xfe; break;
		case 'f': *out = 0xff; break;
		case 'A': *out = 0xfa; break;
		case 'B': *out = 0xfb; break;
		case 'C': *out = 0xfc; break;
		case 'D': *out = 0xfd; break;
		case 'E': *out = 0xfe; break;
		case 'F': *out = 0xff; break;
		} break;
	case 'A':
		switch (in1) {
		case '0': *out = 0xa0; break;
		case '1': *out = 0xa1; break;
		case '2': *out = 0xa2; break;
		case '3': *out = 0xa3; break;
		case '4': *out = 0xa4; break;
		case '5': *out = 0xa5; break;
		case '6': *out = 0xa6; break;
		case '7': *out = 0xa7; break;
		case '8': *out = 0xa8; break;
		case '9': *out = 0xa9; break;
		case 'a': *out = 0xaa; break;
		case 'b': *out = 0xab; break;
		case 'c': *out = 0xac; break;
		case 'd': *out = 0xad; break;
		case 'e': *out = 0xae; break;
		case 'f': *out = 0xaf; break;
		case 'A': *out = 0xaa; break;
		case 'B': *out = 0xab; break;
		case 'C': *out = 0xac; break;
		case 'D': *out = 0xad; break;
		case 'E': *out = 0xae; break;
		case 'F': *out = 0xaf; break;
		} break;
	case 'B':
		switch (in1) {
		case '0': *out = 0xb0; break;
		case '1': *out = 0xb1; break;
		case '2': *out = 0xb2; break;
		case '3': *out = 0xb3; break;
		case '4': *out = 0xb4; break;
		case '5': *out = 0xb5; break;
		case '6': *out = 0xb6; break;
		case '7': *out = 0xb7; break;
		case '8': *out = 0xb8; break;
		case '9': *out = 0xb9; break;
		case 'a': *out = 0xba; break;
		case 'b': *out = 0xbb; break;
		case 'c': *out = 0xbc; break;
		case 'd': *out = 0xbd; break;
		case 'e': *out = 0xbe; break;
		case 'f': *out = 0xbf; break;
		case 'A': *out = 0xba; break;
		case 'B': *out = 0xbb; break;
		case 'C': *out = 0xbc; break;
		case 'D': *out = 0xbd; break;
		case 'E': *out = 0xbe; break;
		case 'F': *out = 0xbf; break;
		} break;
	case 'C':
		switch (in1) {
		case '0': *out = 0xc0; break;
		case '1': *out = 0xc1; break;
		case '2': *out = 0xc2; break;
		case '3': *out = 0xc3; break;
		case '4': *out = 0xc4; break;
		case '5': *out = 0xc5; break;
		case '6': *out = 0xc6; break;
		case '7': *out = 0xc7; break;
		case '8': *out = 0xc8; break;
		case '9': *out = 0xc9; break;
		case 'a': *out = 0xca; break;
		case 'b': *out = 0xcb; break;
		case 'c': *out = 0xcc; break;
		case 'd': *out = 0xcd; break;
		case 'e': *out = 0xce; break;
		case 'f': *out = 0xcf; break;
		case 'A': *out = 0xca; break;
		case 'B': *out = 0xcb; break;
		case 'C': *out = 0xcc; break;
		case 'D': *out = 0xcd; break;
		case 'E': *out = 0xce; break;
		case 'F': *out = 0xcf; break;
		} break;
	case 'D':
		switch (in1) {
		case '0': *out = 0xd0; break;
		case '1': *out = 0xd1; break;
		case '2': *out = 0xd2; break;
		case '3': *out = 0xd3; break;
		case '4': *out = 0xd4; break;
		case '5': *out = 0xd5; break;
		case '6': *out = 0xd6; break;
		case '7': *out = 0xd7; break;
		case '8': *out = 0xd8; break;
		case '9': *out = 0xd9; break;
		case 'a': *out = 0xda; break;
		case 'b': *out = 0xdb; break;
		case 'c': *out = 0xdc; break;
		case 'd': *out = 0xdd; break;
		case 'e': *out = 0xde; break;
		case 'f': *out = 0xdf; break;
		case 'A': *out = 0xda; break;
		case 'B': *out = 0xdb; break;
		case 'C': *out = 0xdc; break;
		case 'D': *out = 0xdd; break;
		case 'E': *out = 0xde; break;
		case 'F': *out = 0xdf; break;
		} break;
	case 'E':
		switch (in1) {
		case '0': *out = 0xe0; break;
		case '1': *out = 0xe1; break;
		case '2': *out = 0xe2; break;
		case '3': *out = 0xe3; break;
		case '4': *out = 0xe4; break;
		case '5': *out = 0xe5; break;
		case '6': *out = 0xe6; break;
		case '7': *out = 0xe7; break;
		case '8': *out = 0xe8; break;
		case '9': *out = 0xe9; break;
		case 'a': *out = 0xea; break;
		case 'b': *out = 0xeb; break;
		case 'c': *out = 0xec; break;
		case 'd': *out = 0xed; break;
		case 'e': *out = 0xee; break;
		case 'f': *out = 0xef; break;
		case 'A': *out = 0xea; break;
		case 'B': *out = 0xeb; break;
		case 'C': *out = 0xec; break;
		case 'D': *out = 0xed; break;
		case 'E': *out = 0xee; break;
		case 'F': *out = 0xef; break;
		} break;
	case 'F':
		switch (in1) {
		case '0': *out = 0xf0; break;
		case '1': *out = 0xf1; break;
		case '2': *out = 0xf2; break;
		case '3': *out = 0xf3; break;
		case '4': *out = 0xf4; break;
		case '5': *out = 0xf5; break;
		case '6': *out = 0xf6; break;
		case '7': *out = 0xf7; break;
		case '8': *out = 0xf8; break;
		case '9': *out = 0xf9; break;
		case 'a': *out = 0xfa; break;
		case 'b': *out = 0xfb; break;
		case 'c': *out = 0xfc; break;
		case 'd': *out = 0xfd; break;
		case 'e': *out = 0xfe; break;
		case 'f': *out = 0xff; break;
		case 'A': *out = 0xfa; break;
		case 'B': *out = 0xfb; break;
		case 'C': *out = 0xfc; break;
		case 'D': *out = 0xfd; break;
		case 'E': *out = 0xfe; break;
		case 'F': *out = 0xff; break;
		} break;
	}
	
}

int check_hex(unsigned char in) {
		
	int check = 0;

		switch (in) {
		case '0': check = 0; break;
		case '1': check = 0; break;
		case '2': check = 0; break;
		case '3': check = 0; break;
		case '4': check = 0; break;
		case '5': check = 0; break;
		case '6': check = 0; break;
		case '7': check = 0; break;
		case '8': check = 0; break;
		case '9': check = 0; break;
		case 'a': check = 0; break;
		case 'b': check = 0; break;
		case 'c': check = 0; break;
		case 'd': check = 0; break;
		case 'e': check = 0; break;
		case 'f': check = 0; break;
		case 'A': check = 0; break;
		case 'B': check = 0; break;
		case 'C': check = 0; break;
		case 'D': check = 0; break;
		case 'E': check = 0; break;
		case 'F': check = 0; break;
		default: check = 1; break;
		}

		if(check)
			return 1; // FAIL
		else
			return 0;
}