#include "plib_max6967.h"
#include "plib_max6967_spi.h"

/**
 * @file plib_max6967.c
 * @brief Pilote du driver PWM MAX6967
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

void MAX6967_StartTranmission(SPI_t *spi)
{
    spi->cs.clear();
}

void MAX6967_EndTranmission(SPI_t *spi)
{
    spi->cs.set();
}

void MAX6967_Init(MAX6967_t* obj)
{
    MAX6967_EndTranmission(&obj->spi);
    // set configuration reg externally: run, stagged enbled
    MAX6967_WriteConfigurationReg(obj, obj->config.reg);
    MAX6967_DisableAllPorts(obj);
}

void MAX6967_WriteRegister(MAX6967_t* obj, unsigned char reg, unsigned char data)
{
    MAX6967_StartTranmission(&obj->spi);
    MAX6967_Write(&obj->spi, reg, data);
    MAX6967_EndTranmission(&obj->spi);
}

unsigned char MAX6967_ReadRegister(MAX6967_t* obj, unsigned char reg)
{
    unsigned char readData = 0;
    MAX6967_StartTranmission(&obj->spi);
    readData = MAX6967_Read(&obj->spi, reg);
    MAX6967_EndTranmission(&obj->spi);
    return readData;
}

/* ==== Fonctions Write ==== */

void MAX6967_WriteConfigurationReg(MAX6967_t* obj, unsigned char data)
{
    MAX6967_WriteRegister(obj, MAX6967_REG_CONF, data);
}

void MAX6967_WritePortReg(MAX6967_t* obj, unsigned char port, unsigned char data)
{
    MAX6967_WriteRegister(obj, (MAX6967_REG_PORT0 + port), data);
}

void MAX6967_WriteOutputCurrentReg(MAX6967_t* obj, unsigned char* data)
{
    MAX6967_WriteRegister(obj, MAX6967_REG_IOUT70, data[0]);
    MAX6967_WriteRegister(obj, MAX6967_REG_IOUT98, data[1]);
}

void MAX6967_WriteGlobalCurrentReg(MAX6967_t* obj, unsigned char data)
{
    MAX6967_WriteRegister(obj, MAX6967_REG_GLOBAL_CURRENT, data);
}

void MAX6967_WritePortPWMReg(MAX6967_t* obj, unsigned char port, unsigned char pwm)
{
    // Convert pwm to raw pwm
    unsigned char rawPwm = MAX6967_PORT_CC_PWM_MIN - (pwm * (MAX6967_PORT_CC_PWM_MIN - MAX6967_PORT_CC_PWM_MAX)) / 100;
    // Write register
    MAX6967_WritePortReg(obj, port, rawPwm);
}

void MAX6967_SetRunMode(MAX6967_t* obj, unsigned char state)
{
    obj->config.bits.mode = state;
    MAX6967_WriteConfigurationReg(obj, obj->config.reg);
}

void MAX6967_DisablePort(MAX6967_t* obj, unsigned char port)
{
    MAX6967_WritePortReg(obj, (MAX6967_REG_PORT0 + port), MAX6967_PORT_CC_OFF);
}

void MAX6967_DisableAllPorts(MAX6967_t* obj)
{
    unsigned char i;

    for(i = 0; i < MAX6967_NUM_PORTS; i++)
    {
        MAX6967_DisablePort(obj, i);
    }
}

/* ==== Fonctions Read ==== */

unsigned char MAX6967_ReadConfigurationReg(MAX6967_t* obj)
{
    return MAX6967_ReadRegister(obj, MAX6967_REG_CONF);
}

unsigned char MAX6967_ReadPortReg(MAX6967_t* obj, unsigned char port)
{
    return MAX6967_ReadRegister(obj, (MAX6967_REG_PORT0 + port));
}