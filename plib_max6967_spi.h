#ifndef PLIB_MAX6967_SPI_H
#define PLIB_MAX6967_SPI_H


#include "libs/common_c_libs/plib_comm_struct.h"

#define MAX6967_WRITE_MODE          0x00
#define MAX6967_READ_MODE           0x80

/* ==== Fonctions SPI ==== */

void MAX6967_Write(SPI_t *spi, unsigned char reg, unsigned char val);
unsigned char MAX6967_Read(SPI_t *spi, unsigned char reg);

#endif  // PLIB_MAX6967_SPI_H
