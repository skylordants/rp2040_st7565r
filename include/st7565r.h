#ifndef __ST7565R_H__
#define __ST7565R_H__

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "glcdfont.h"

class ST7565R {
public:
	/// @brief Setup ST7565R LCD
	/// @param spi SPI instance
	/// @param pBL backlight anode pin
	/// @param pCS SPI CS
	/// @param pSCK SPI SCK
	/// @param pTX SPI TX
	/// @param pCD Action selection pin
	/// @param pRST reset pin
	ST7565R(spi_inst_t *spi, uint8_t pBL, uint8_t pCS, uint8_t pSCK, uint8_t pTX, uint8_t pCD, uint8_t pRST);
	ST7565R();

	void backlight(bool on);
	void display_on(bool on);

	void DBG_all_points(bool on);

	bool ram_select_address(uint8_t page, uint8_t column);
	bool ram_write(uint8_t *data, uint8_t len);
	bool ram_clear();
	bool write_chars(char *buffer, uint8_t len, uint8_t line, uint8_t pos);

	/// @brief Call to reset or initialize the display, buffers and settings
	bool reset();

protected:
	bool send_command(uint8_t *buffer, uint8_t len, bool write);

	spi_inst_t *_spi;

	uint8_t _pBL;
	uint8_t _pCS;
	uint8_t _pSCK;
	uint8_t _pTX;
	uint8_t _pCD;
	uint8_t _pRST;

	uint8_t _ram_page_address = 0x00;
	uint8_t _ram_column_address = 0x00;
};

#endif