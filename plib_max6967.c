/**
 * @file plib_max6967.c
 * @brief Pilote du driver PWM MAX6967
 * @author Ramiro Najera
 * @version 1.0.4
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

#include "plib_max6967.h"

/** @brief Masque pour commande en mode lecture */
#define MAX6967_READ_MODE           0x80

// Static functions

static void MAX6967_Write(SPI_t *spi, uint8_t reg, uint8_t data);
static uint8_t MAX6967_Read(SPI_t *spi, uint8_t reg);
static void MAX6967_StartTranmission(SPI_t *spi);
static void MAX6967_EndTranmission(SPI_t *spi);
static void MAX6967_WriteRegister(MAX6967_t* obj, uint8_t reg, uint8_t data);
uint8_t MAX6967_ReadRegister(MAX6967_t* obj, uint8_t reg);

// Public API

void MAX6967_Init(MAX6967_t* obj)
{
    MAX6967_EndTranmission(&obj->spi);
    // Set global current to max
    MAX6967_WriteGlobalCurrentReg(obj, MAX6967_GLOBAL_CURRENT_20_MA);
    // Set configuration reg externally: run, stagged enbled
    MAX6967_WriteConfigurationReg(obj, obj->config.reg);
    // Set all ports to min pwm value
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT0, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT1, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT2, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT3, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT4, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT5, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT6, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT7, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT8, 0);
    MAX6967_WritePortPWMReg(obj, MAX6967_REG_PORT9, 0);
}



/* ==== Fonctions Write ==== */

void MAX6967_WriteConfigurationReg(MAX6967_t* obj, uint8_t data)
{
    MAX6967_WriteRegister(obj, MAX6967_REG_CONF, data);
}

void MAX6967_WritePortReg(MAX6967_t* obj, uint8_t port, uint8_t data)
{
    MAX6967_WriteRegister(obj, (MAX6967_REG_PORT0 + port), data);
}

void MAX6967_WriteOutputCurrentReg(MAX6967_t* obj, uint8_t* data)
{
    MAX6967_WriteRegister(obj, MAX6967_REG_IOUT70, data[0]);
    MAX6967_WriteRegister(obj, MAX6967_REG_IOUT98, data[1]);
}

void MAX6967_WriteGlobalCurrentReg(MAX6967_t* obj, uint8_t data)
{
    MAX6967_WriteRegister(obj, MAX6967_REG_GLOBAL_CURRENT, data);
}

void MAX6967_WritePortPWMReg(MAX6967_t* obj, uint8_t port, uint8_t pwm)
{
    uint16_t rawPwm;
    // Calculate PWM register according to PWM value
    if(pwm == 0)
        rawPwm = MAX6967_PORT_LOGIC_LOW;
    else if(pwm == 100)
        rawPwm = MAX6967_PORT_LOGIC_HIGH;
    else 
        rawPwm = MAX6967_PORT_CC_PWM_MAX - (pwm * (MAX6967_PORT_CC_PWM_MAX - MAX6967_PORT_CC_PWM_MIN) / 100);
    // Write register
    MAX6967_WritePortReg(obj, port, (uint8_t)rawPwm);
}

void MAX6967_SetRunMode(MAX6967_t* obj, uint8_t state)
{
    obj->config.bits.mode = state;
    MAX6967_WriteConfigurationReg(obj, obj->config.reg);
}

void MAX6967_DisablePort(MAX6967_t* obj, uint8_t port)
{
    MAX6967_WritePortReg(obj, (MAX6967_REG_PORT0 + port), MAX6967_PORT_CC_OFF);
}

/* ==== Fonctions Read ==== */

uint8_t MAX6967_ReadConfigurationReg(MAX6967_t* obj)
{
    return MAX6967_ReadRegister(obj, MAX6967_REG_CONF);
}

uint8_t MAX6967_ReadPortReg(MAX6967_t* obj, uint8_t port)
{
    return MAX6967_ReadRegister(obj, (MAX6967_REG_PORT0 + port));
}

// Static functions

static void MAX6967_Write(SPI_t *spi, uint8_t reg, uint8_t val)
{
    uint8_t TXBuffer[2] = {reg, val};

    // Disable interrupts in critical part
    __builtin_disable_interrupts();
    
    // Send buffer, return error code
    if(spi->Write)
        spi->Write(TXBuffer, 2);
    
    // Enable interrupts after critical part
    __builtin_enable_interrupts();
}

static uint8_t MAX6967_Read(SPI_t *spi, uint8_t reg)
{
    uint8_t TXBuffer[2] = {(reg | MAX6967_READ_MODE), 0xFF};

    // Disable interrupts in critical part
    __builtin_disable_interrupts();

    uint8_t RXBuffer[1];

    // Send buffer and read data according to SPI channel
    if(spi->Write)
        spi->Write(TXBuffer, 1);
    if(spi->Read)
        spi->Read(RXBuffer, 1);

    // Enable interrupts after critical part
    __builtin_enable_interrupts();
    return RXBuffer[0];
}

static void MAX6967_StartTranmission(SPI_t *spi)
{
    if(spi->pinEN.Clear)
        spi->pinEN.Clear();
    if(spi->pinCS.Clear) 
        spi->pinCS.Clear();
}

static void MAX6967_EndTranmission(SPI_t *spi)
{
    if(spi->pinCS.Set)
        spi->pinCS.Set();
    if(spi->pinEN.Set)
        spi->pinEN.Set();
}

static void MAX6967_WriteRegister(MAX6967_t* obj, uint8_t reg, uint8_t data)
{
    MAX6967_StartTranmission(&obj->spi);
    MAX6967_Write(&obj->spi, reg, data);
    MAX6967_EndTranmission(&obj->spi);
}

static uint8_t MAX6967_ReadRegister(MAX6967_t* obj, uint8_t reg)
{
    uint8_t readData = 0;
    MAX6967_StartTranmission(&obj->spi);
    readData = MAX6967_Read(&obj->spi, reg);
    MAX6967_EndTranmission(&obj->spi);
    return readData;
}