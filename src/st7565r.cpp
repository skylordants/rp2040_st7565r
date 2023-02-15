#include <stdio.h>

#include "hardware/gpio.h"

#include "st7565r.h"

ST7565R::ST7565R(spi_inst_t *spi, uint8_t pBL, uint8_t pCS, uint8_t pSCK, uint8_t pTX, uint8_t pCD, uint8_t pRST)
	: _spi(spi)
	, _pBL(pBL)
	, _pCS(pCS)
	, _pSCK(pSCK)
	, _pTX(pTX)
	, _pCD(pCD)
	, _pRST(pRST)
{
	gpio_init(_pCS);
  	gpio_set_dir(_pCS, GPIO_OUT);
	gpio_put(_pCS, 1);

	// Initialize SPI port at 20 MHz
	spi_init(_spi, 20*1000*1000);

	// Set SPI format
	spi_set_format( _spi,   // SPI instance
									8,      // Number of bits per transfer
									SPI_CPOL_0,      // Polarity (CPOL)
									SPI_CPHA_1,      // Phase (CPHA)
									SPI_MSB_FIRST);

	// Initialize SPI pins
	gpio_set_function(_pSCK, GPIO_FUNC_SPI);
	gpio_set_function(_pTX, GPIO_FUNC_SPI);


	// CD
	gpio_init(_pCD);
	gpio_set_dir(_pCD, GPIO_OUT);

	// Backlight
	gpio_init(_pBL);
	gpio_set_dir(_pBL, GPIO_OUT);

	// Reset
	gpio_init(_pRST);
	gpio_set_dir(_pRST, GPIO_OUT);

	reset();

}

ST7565R::ST7565R() {

}

bool ST7565R::reset() {
	gpio_put(_pCS, 0);
	gpio_put(_pRST, 0);
	sleep_ms(500);
	gpio_put(_pRST, 1);
	gpio_put(_pCS, 1);


	uint8_t data[1];

	// LCD bias select
	data[0] = 0xA3;
	send_command(data, 1, 0);

	// ADC select
	data[0] = 0xA0;
	send_command(data, 1, 0);

	// COM select
	data[0] = 0xC0;
	send_command(data, 1, 0);

	// Start line select
	data[0] = 0x40;
	send_command(data, 1, 0);

	// Voltage converter
	data[0] = 0x2C;
	send_command(data, 1, 0);

	sleep_ms(50);

	// Voltage regulator
	data[0] = 0x2E;
	send_command(data, 1, 0);

	sleep_ms(50);

	// Voltage follower
	data[0] = 0x2F;
	send_command(data, 1, 0);

	sleep_ms(10);

	// LCD operating voltage
	data[0] = 0x24;
	send_command(data, 1, 0);

	ram_clear();

	display_on(true);
	return true;
}

bool ST7565R::send_command(uint8_t *buffer, uint8_t len, bool write) {
	gpio_put(_pCD, write);

	gpio_put(_pCS, 0);
	spi_write_blocking(_spi, buffer, len);
	gpio_put(_pCS, 1);
	return true;
}

void ST7565R::backlight(bool on) {
	gpio_put(_pBL, on);
}

void ST7565R::display_on(bool on) {
	uint8_t data[1] = {(uint8_t)(0xae | on)};
	send_command(data, 1, 0);
}

void ST7565R::DBG_all_points(bool on) {
	uint8_t data[1] = {(uint8_t)(0xa4 | on)};
	printf("%x\n", data[0]);
	send_command(data, 1, 0);
}

bool ST7565R::ram_select_address(uint8_t page, uint8_t column) {
	if (page > 7) {
		printf("ERROR: Wrong Page Address\n");
		return false;
	}

	if (column > 127) {
		printf("ERROR: Wrong Column Address\n");
		return false;
	}

	uint8_t data[3] = {(uint8_t)(0xB0 | page), (uint8_t)( 0x10 | (column>>4)),(uint8_t)(0x00 | (column&0x0F))};
	return send_command(data, 3, 0);
}

bool ST7565R::ram_write(uint8_t *data, uint8_t len) {
	return send_command(data, len, 1);
}

bool ST7565R::ram_clear() {
	uint8_t buf [128];
	for (int i = 0; i < 128; i++) {
		buf[i] = 0x00;
	}
	for (int page = 0; page < 8; page++) {

		ST7565R::ram_select_address(page, 0);
		ST7565R::ram_write(buf, 128);
	}

	return true;
}

bool ST7565R::write_chars(char *buffer, uint8_t len, uint8_t line, uint8_t pos) {
	ram_select_address(line, pos*5);

	for (int i = 0; i < len; i++) {
		uint8_t data[5];
		for (int j = 0; j < 5; j++) {
			data[j] = font[(uint8_t)buffer[i]*5 + j];
		}
		ram_write(data, 5);
	}
	return true;
}

