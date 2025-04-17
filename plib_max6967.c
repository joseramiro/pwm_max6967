#include "plib_max6967.h"
#include "plib_max6967_spi.h"

void MAX6967_StartTranmission(SPIConfiguration_t *spi)
{
    spi->cs_clear();
}

void MAX6967_EndTranmission(SPIConfiguration_t *spi)
{
    spi->cs_set();
}

void MAX6967_Init(MAX6967Conf_t* conf)
{
    MAX6967_EndTranmission(&conf->spi);
    MAX6967_Configure(conf);
    MAX6967_WritePortPWMReg(conf, MAX6967_PORT_CC_OFF);
}

void MAX6967_Configure(MAX6967Conf_t* conf)
{   
    // Configuration to be done in high level function
    conf->config.bits.shutdown = MAX6967_MODE_RUN;
    conf->config.bits.cs_run = MAX6967_CS_RUN_DISABLED;
    conf->config.bits.ramp_up = MAX6967_RAMP_UP_DISABLED;
    conf->config.bits.fade_off = MAX6967_FADE_OFF_DISABLED;
    conf->config.bits.hold_off = MAX6967_HOLD_OFF_ENABLED;
    conf->config.bits.pwm_stagger = MAX6967_PWM_STAGGED;
    conf->config.bits.osc = MAX6967_OSC_INTERNAL;
}

void MAX6967_WriteRegister(MAX6967Conf_t* conf, unsigned char reg, unsigned char data)
{
    MAX6967_StartTranmission(&conf->spi);
    MAX6967_Write(&conf->spi, reg, data);
    MAX6967_EndTranmission(&conf->spi);
}

unsigned char MAX6967_ReadRegister(MAX6967Conf_t* conf, unsigned char reg)
{
    unsigned char readData = 0;
    MAX6967_StartTranmission(&conf->spi);
    readData = MAX6967_Read(&conf->spi, reg);
    MAX6967_EndTranmission(&conf->spi);
    return readData;
}

/* ==== Fonctions Write ==== */

void MAX6967_WriteConfigurationReg(MAX6967Conf_t* conf, unsigned char data)
{
    MAX6967_WriteRegister(conf, MAX6967_REG_CONF, data);
}

void MAX6967_WritePortReg(MAX6967Conf_t* conf, unsigned char data)
{
    MAX6967_WriteRegister(conf, (MAX6967_REG_PORT0 + conf->id), data);
}

void MAX6967_WriteOutputCurrentReg(MAX6967Conf_t* conf, unsigned char* data)
{
    MAX6967_WriteRegister(conf, (MAX6967_REG_IOUT70 + conf->id), data[0]);
    MAX6967_WriteRegister(conf, (MAX6967_REG_IOUT98 + conf->id), data[1]);
}

void MAX6967_WriteGlobalCurrentReg(MAX6967Conf_t* conf, unsigned char data)
{
    MAX6967_WriteRegister(conf, (MAX6967_REG_GLOBAL_CURRENT + conf->id), data);
}

void MAX6967_WritePortPWMReg(MAX6967Conf_t* conf, unsigned char data)
{
    MAX6967_WritePortReg(conf, (MAX6967_PORT_CC_PWM_MAX | (data << 2)));
}

/* ==== Fonctions Read ==== */

unsigned char MAX6967_ReadConfigurationReg(MAX6967Conf_t* conf)
{
    return MAX6967_ReadRegister(conf, MAX6967_REG_CONF);
}

unsigned char MAX6967_ReadPortReg(MAX6967Conf_t* conf)
{
    return MAX6967_ReadRegister(conf, (MAX6967_REG_PORT0 + conf->id));
}


/*
#define MAX6967_PORT_LOGIC_LOW      0x00
#define MAX6967_PORT_LOGIC_HIGH     0x01
#define MAX6967_PORT_CC_STATIC      0x02
#define MAX6967_PORT_CC_PWM_MAX     0x03
#define MAX6967_PORT_CC_PWM_MIN     0xFE
#define MAX6967_PORT_CC_OFF         0xFF
*/