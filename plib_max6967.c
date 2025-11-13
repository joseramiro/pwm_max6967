/**
 * @file plib_max6967.c
 * @brief Pilote du driver PWM MAX6967
 * @author Ramiro Najera
 * @version 1.0.5
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

#include <stddef.h>
#include "plib_max6967.h"
#include "plib_max6967_spi.h"
#include "libs/common_c_libs/plib_data_struct.h"

/* ==== Fonctions SPI ==== */

void MAX6967_StartTranmission(SPI_t *spi)
{
    if(spi->pinEN.Clear != NULL)
        spi->pinEN.Clear();
    if(spi->pinCS.Clear != NULL) 
        spi->pinCS.Clear();
}

void MAX6967_EndTranmission(SPI_t *spi)
{
    if(spi->pinCS.Set != NULL)
        spi->pinCS.Set();
    if(spi->pinEN.Set != NULL)
        spi->pinEN.Set();
}

unsigned char MAX6967_InitChip(MAX6967_t* obj)
{
    MAX6967_EndTranmission(&obj->spi);
    // Set global current to max
    MAX6967_WriteGlobalCurrentReg(obj, MAX6967_GLOBAL_CURRENT_20_MA);
    // Set configuration reg externally: run, stagged enbled
    MAX6967_WriteConfigurationReg(obj, obj->config.reg);
    // Set all ports to min pwm value
    // todo: Add a checkwrite register(see mcp23s17 as example)
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
    //Return error code
    return 0;
}

unsigned int MAX6967_InitList(MAX6967_t *objList, unsigned char size)
{
    unsigned int errorCode = 0;
    // Check max size: return error code
    if(size > 16)
        return 0xFF;
    // Init each MAX6967 module
    for(unsigned char i = 0; i < size; i++)
    {
        if(MAX6967_InitChip(&objList[i]))
            SET_FLAG_BIT(errorCode, i);
    }
    // Return error code
    return errorCode;
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
    unsigned int rawPwm;
    // Calculate PWM register according to PWM value
    if(pwm == 0)
        rawPwm = MAX6967_PORT_LOGIC_LOW;
    else if(pwm == 100)
        rawPwm = MAX6967_PORT_LOGIC_HIGH;
    else 
        rawPwm = MAX6967_PORT_CC_PWM_MAX - (pwm * (MAX6967_PORT_CC_PWM_MAX - MAX6967_PORT_CC_PWM_MIN) / 100);
    // Write register
    MAX6967_WritePortReg(obj, port, (unsigned char)rawPwm);
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

/* ==== Fonctions Read ==== */

unsigned char MAX6967_ReadConfigurationReg(MAX6967_t* obj)
{
    return MAX6967_ReadRegister(obj, MAX6967_REG_CONF);
}

unsigned char MAX6967_ReadPortReg(MAX6967_t* obj, unsigned char port)
{
    return MAX6967_ReadRegister(obj, (MAX6967_REG_PORT0 + port));
}