
/**
 * @file plib_max6967_spi.c
 * @brief Pilote SPI pour le MAX6967 (driver de PWM)
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

#include "plib_max6967_spi.h"
#include "peripheral/spi/spi_master/plib_spi1_master.h"
#include "peripheral/spi/spi_master/plib_spi2_master.h"

void MAX6967_Write(SPI_t *spi, unsigned char reg, unsigned char val)
{
    unsigned char TXBuffer[2] =
    {
        (reg),
        val
    };

    // Disable interrupts in critical part
    __builtin_disable_interrupts();
    
    // Send buffer, return error code
    if(spi->channel == SPI_CH1)
        SPI1_Write(TXBuffer, 2);
    else if(spi->channel == SPI_CH2)
        SPI2_Write(TXBuffer, 2);
    
    // Enable interrupts after critical part
    __builtin_enable_interrupts();
}

unsigned char MAX6967_Read(SPI_t *spi, unsigned char reg)
{
    unsigned char TXBuffer[2] =
    {
        (reg | MAX6967_READ_MODE),
        0xFF
    };

    // Disable interrupts in critical part
    __builtin_disable_interrupts();

    unsigned char RXBuffer[1];

    // Send buffer and read data according to SPI channel
    if(spi->channel == SPI_CH1)
    {
        SPI1_Write(TXBuffer, 1);
        SPI1_Read(RXBuffer, 1);
    }
    else if(spi->channel == SPI_CH2)
    {
        SPI2_Write(TXBuffer, 1);
        SPI2_Read(RXBuffer, 1);
    }

    // Enable interrupts after critical part
    __builtin_enable_interrupts();
    return RXBuffer[0];
}