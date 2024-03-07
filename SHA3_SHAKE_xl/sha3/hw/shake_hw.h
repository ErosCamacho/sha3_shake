#include <pynq_api.h>
#include "params.h"

/************************ MS2XL Constant Definitions **********************/

#define MS2XL_LENGTH   0x40

#define RESET					1
#define LOAD_PADDING			2
#define START_PADDING			4
#define LOAD					8
#define START					16
#define	READ					32
#define	START_PADDING_DOUBLE	64
#define START_SHAKE				128

#define DATA_IN  0x0		/**< data_in */
#define ADDRESS  0x8		/**< address */
#define CONTROL  0x10		/**< control */
#define DATA_OUT 0x18		/**< data_out */
#define END_OP   0x20		/**< end_op */

/************************ MS2XL Function Definitions **********************/

void shake_ms2xl_init(PYNQ_MMIO_WINDOW ms2xl);
void shake_ms2xl(unsigned long long int* a, unsigned long long int* b, PYNQ_MMIO_WINDOW ms2xl, unsigned long long int pos_pad, int pad, int read, int DBG);
void shake_hw(unsigned char* in, unsigned char* out, unsigned long long int length, unsigned long long int length_out, PYNQ_MMIO_WINDOW ms2xl, int DBG);
