#ifndef PLIB_MAX6967_H
#define PLIB_MAX6967_H

#include "libs/util/plibs_utils.h"  // used for SPI struct

#define MAX6967_REG_PORT0           0x00
#define MAX6967_REG_PORT1           0x01
#define MAX6967_REG_PORT2           0x02
#define MAX6967_REG_PORT3           0x03
#define MAX6967_REG_PORT4           0x04
#define MAX6967_REG_PORT5           0x05
#define MAX6967_REG_PORT6           0x06
#define MAX6967_REG_PORT7           0x07
#define MAX6967_REG_PORT8           0x08
#define MAX6967_REG_PORT9           0x09

#define MAX6967_REG_CONF            0x10
#define MAX6967_REG_IOUT70          0x13    // 0 half CC, 1 full CC
#define MAX6967_REG_IOUT98          0x14    // 0 half CC, 1 full CC
#define MAX6967_REG_GLOBAL_CURRENT  0x15    // 0b000 2.5 mA for full and 1.25 for half (same value as step)

#define MAX6967_PORT_LOGIC_LOW      0x00
#define MAX6967_PORT_LOGIC_HIGH     0x01
#define MAX6967_PORT_CC_STATIC      0x02
#define MAX6967_PORT_CC_PWM_MAX     0x03    // 100 %
#define MAX6967_PORT_CC_PWM_MIN     0xFE    // 0 %
#define MAX6967_PORT_CC_OFF         0xFF

// D7 : DOUT/OSC, OSC
typedef enum
{
    MAX6967_OSC_INTERNAL,
    MAX6967_OSC_EXTERNAL
}MAX6967_OSC_Conf;

// D6 : not used

// D5 : PWM stagger, stagger (whether all PWM outputs in same phase, or stagged)
typedef enum
{
    MAX6967_PWM_PHASED,
    MAX6967_PWM_STAGGED
}MAX6967_PWMStagger_Conf;;

// D4 : Hold-off status, hold-off
typedef enum
{
    MAX6967_HOLD_OFF_DISABLED,
    MAX6967_HOLD_OFF_ENABLED
}MAX6967_HoldOff_Conf;

// D3 : Fade-off status, fade-off ()
typedef enum
{
    MAX6967_FADE_OFF_DISABLED,
    MAX6967_FADE_OFF_ENABLED
}MAX6967_FadeOff_Conf;

// D2 : Ramp-up disabled, ramp-up enabled
typedef enum
{
    MAX6967_RAMP_UP_DISABLED,
    MAX6967_RAMP_UP_ENABLED
}MAX6967_RampUp_Conf;

// D1 : CS disabled, CS enabled
typedef enum
{
    MAX6967_CS_RUN_DISABLED,
    MAX6967_CS_RUN_ENABLED
}MAX6967_CSRun_Conf;

// D0 : Shutdown, run 
typedef enum
{
    MAX6967_MODE_SHUTDOWN,
    MAX6967_MODE_RUN
}MAX6967_Mode_Conf;

typedef union
{
    struct {
        unsigned char shutdown      : 1;    // Bit 0
        unsigned char cs_run        : 1;    // Bit 1
        unsigned char ramp_up       : 1;    // Bit 2
        unsigned char fade_off      : 1;    // Bit 3
        unsigned char hold_off      : 1;    // Bit 4
        unsigned char pwm_stagger   : 1;    // Bit 5
        unsigned char b6            : 1;    // Bit 6 (reserved)
        unsigned char osc           : 1;    // Bit 7
    } bits;
    unsigned char reg; // Full register as a byte
} MAX6967ConfReg_t;

typedef struct
{
    unsigned char id;                   /**< Identifiant du module */
    MAX6967ConfReg_t config;
    SPIConfiguration_t spi;               /**< Configuration SPI associée au module */
}MAX6967Conf_t;

void MAX6967_StartTranmission(SPIConfiguration_t *spi);
void MAX6967_EndTranmission(SPIConfiguration_t *spi);

void MAX6967_Init(MAX6967Conf_t* conf);
void MAX6967_Configure(MAX6967Conf_t* conf);

void MAX6967_WriteRegister(MAX6967Conf_t* conf, unsigned char reg, unsigned char data);
unsigned char MAX6967_ReadRegister(MAX6967Conf_t* conf, unsigned char reg);

/* ==== Fonctions Write ==== */

void MAX6967_WriteConfigurationReg(MAX6967Conf_t* conf, unsigned char data);
void MAX6967_WritePortReg(MAX6967Conf_t* conf, unsigned char data);
void MAX6967_WriteOutputCurrentReg(MAX6967Conf_t* conf, unsigned char* data);
void MAX6967_WriteGlobalCurrentReg(MAX6967Conf_t* conf, unsigned char data);
void MAX6967_WritePortPWMReg(MAX6967Conf_t* conf, unsigned char data);

/* ==== Fonctions Read ==== */

unsigned char MAX6967_ReadConfigurationReg(MAX6967Conf_t* conf);
unsigned char MAX6967_ReadPortReg(MAX6967Conf_t* conf);

#endif  // PLIB_MAX6967_H
