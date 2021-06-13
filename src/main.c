#include <inttypes.h>
#include <stdbool.h>
#include <stm32f10x.h>

#include "debug.h"
#include "display.h"
#include "io.h"
#include "sram.h"

// Display - Working
// init - broken
// sending commands - broken

// SRAM - Working
// Reading and writing
// Sequential writing
// Mode set
// Sequential reading broken

void test(uint8_t testnr)
{
	size_t i;

	switch (testnr) {
	case 0:
		// check if reading and writing one byte is working
		for (i = 0; i < 0xff; i++) {
			sram_write_byte(0x0000, i);
			assert(sram_read_byte(0x0000) == i);
		}
		break;

	case 1:
		// check if read and write to status has effect
		sram_write_status(SRAM_MODE_SEQUENTIAL);
		assert(sram_read_status() == SRAM_MODE_SEQUENTIAL);
		sram_write_status(SRAM_MODE_BYTE);
		break;

	case 2:
		// check if sram_clear resets all data
		{
			sram_clear();

			for (i = 0; i < SRAM_SIZE; i++) {
				assert(sram_read_byte(i) == 0x00);
			}
		}
		break;

	case 3:
		// test if the display can show simple random data
		{
			const size_t buffersize = (200 * 200) / 8;
			uint8_t framebuffer[buffersize];

			// send frame
			send_display(D_DTM1, framebuffer, buffersize);

			// send data stop
			send_display(D_DSP, NULL, 0);

			// send refresh
			send_display(D_DRF, NULL, 0);
		}
		break;
	}

	// show that the test was passed
	while (1) {
		DC = !DC;
	}
}

int main()
{
	// run config functions
	__disable_irq();
	spi1_init();
	sram_init();
	//init_display();
	__enable_irq();

	test(2);
	return 0;
}
