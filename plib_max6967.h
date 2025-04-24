#ifndef PLIB_MAX6967_H
#define PLIB_MAX6967_H

/**
 * @file plib_max6967.h
 * @brief Pilote du driver PWM MAX6967
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

#include "libs/common_c_libs/plib_comm_struct.h"

/** @brief Nombre de ports sur module MAX6967 */
#define MAX6967_NUM_PORTS       10

/** 
 * @defgroup MAX6967_Registers Registres du MAX6967
 * @brief Définitions des registres du composant MAX6967 utilisés pour la communication SPI.
 * @{
 */
/** @brief Registre de niveau de sortie sur le port 0 */
#define MAX6967_REG_PORT0           0x00
/** @brief Registre de niveau de sortie sur le port 1 */
#define MAX6967_REG_PORT1           0x01
/** @brief Registre de niveau de sortie sur le port 2 */
#define MAX6967_REG_PORT2           0x02
/** @brief Registre de niveau de sortie sur le port 3 */
#define MAX6967_REG_PORT3           0x03
/** @brief Registre de niveau de sortie sur le port 4 */
#define MAX6967_REG_PORT4           0x04
/** @brief Registre de niveau de sortie sur le port 5 */
#define MAX6967_REG_PORT5           0x05
/** @brief Registre de niveau de sortie sur le port 6 */
#define MAX6967_REG_PORT6           0x06
/** @brief Registre de niveau de sortie sur le port 7 */
#define MAX6967_REG_PORT7           0x07
/** @brief Registre de niveau de sortie sur le port 8 */
#define MAX6967_REG_PORT8           0x08
/** @brief Registre de niveau de sortie sur le port 9 */
#define MAX6967_REG_PORT9           0x09
/** @brief Registre de configuration */
#define MAX6967_REG_CONF            0x10
/** @brief Registre de configuration pour courant continu (hald ou full) des ports 0 à 7 */
#define MAX6967_REG_IOUT70          0x13    // 0 half CC, 1 full CC
/** @brief Registre de configuration pour courant continu (hald ou full) des ports 8 à 9 */
#define MAX6967_REG_IOUT98          0x14    // 0 half CC, 1 full CC
/** @brief Registre de courant global pour courant max global */
#define MAX6967_REG_GLOBAL_CURRENT  0x15    // 0b000 2.5 mA for full and 1.25 for half (same value as step)
/** @} */ // Fin du groupe MAX6967_Registers

/** 
 * @defgroup MAX6967_PortValues Valeurs du registre port du MAX6967
 * @brief Définitions des valeurs du registre port du MAX6967
 * @{
 */
#define MAX6967_PORT_LOGIC_LOW      0x00
#define MAX6967_PORT_LOGIC_HIGH     0x01
#define MAX6967_PORT_CC_STATIC      0x02
#define MAX6967_PORT_CC_PWM_MAX     0x03    // 100 %
#define MAX6967_PORT_CC_PWM_MIN     0xFE    // 0 %
#define MAX6967_PORT_CC_OFF         0xFF
/** @} */ // Fin du groupe MAX6967_PortValues

/**
 * @union MAX6967ConfReg_t
 * @brief Registre de direction des broches (IODIR) du MCP23S17
 */
typedef union
{
    /** @brief Accès individuel aux bits du registre. */
    struct {
        /** @brief Mode (shutdown ou run) */
        unsigned char mode : 1;
        /** @brief Chip Select run conversion */
        unsigned char cs_run : 1;
        /** @brief Ramp up configuration */
        unsigned char ramp_up : 1;
        /** @brief Fade off configuration */
        unsigned char fade_off : 1;
        /** @brief Hold off configuration */
        unsigned char hold_off : 1;
        /** @brief Stagger configuration (phased or stagged) */
        unsigned char stagger : 1;
        /** @brief Bit 6 */
        unsigned char b6 : 1;
        /** @brief Oscillator configuration */
        unsigned char osc : 1;
    } bits;
    /** @brief Accès complet au registre sous forme d'un octet. */
    unsigned char reg; /**< Accès complet au registre sous forme d'un octet. */
} MAX6967ConfReg_t;

/**
 * @enum MAX6967ConfMode_t
 * @brief Active ou désactive complètement le périphérique.
 */
typedef enum
{
    /** @brief Tous les ports sont en haute impédance (désactivés). */
    MAX6967_CONF_MODE_SHUTDOWN,
    /** @brief Le périphérique fonctionne normalement, les sorties fonctionnent comme configurées. */
    MAX6967_CONF_MODE_RUN
}MAX6967ConfMode_t;

/**
 * @enum MAX6967ConfCSRun_t
 * @brief Détermine si les modifications SPI sont immédiatement appliquées aux sorties ou non.
 */
typedef enum
{
    /** @brief Les modifications nécessitent une commande spéciale pour être appliquées */
    MAX6967_CONF_CS_RUN_DISABLED,
    /** @brief Des changements immédiatement dès que les données sont envoyées via SPI. */
    MAX6967_CONF_CS_RUN_ENABLED
}MAX6967ConfCSRun_t;

/**
 * @enum MAX6967ConfRampUp_t
 * @brief Contrôle le fade-in (augmentation progressive) des sorties quand elles sont activées.
 */
typedef enum
{
    /** @brief La sortie est activée instantanément*/
    MAX6967_CONF_RAMP_UP_DISABLED,
    /** @brief La sortie augmente progressivement */
    MAX6967_CONF_RAMP_UP_ENABLED
}MAX6967ConfRampUp_t;

/**
 * @enum MAX6967ConfFadeOff_t
 * @brief Contrôle le fade-out (diminution progressive) des sorties lorsqu'elles sont éteintes.
 */
typedef enum
{
    /** @brief La sortie est éteinte immédiatement. */
    MAX6967_CONF_FADE_OFF_DISABLED,
    /** @brief La sortie diminue progressivement jusqu'à zéro.*/
    MAX6967_CONF_FADE_OFF_ENABLED
}MAX6967ConfFadeOff_t;

/**
 * @enum MAX6967ConfHoldOff_t
 * @brief Crée un délai avant de commencer le fade-off.
 */
typedef enum
{
    /** @brief L'effet de fade-off commence immédiatement.*/
    MAX6967_CONF_HOLD_OFF_DISABLED,
    /** @brief  Un délai est ajouté avant de commencer le fade-off.*/
    MAX6967_CONF_HOLD_OFF_ENABLED
}MAX6967ConfHoldOff_t;

/**
 * @enum MAX6967ConfPWMStagger_t
 * @brief Décale les phases de PWM des différentes sorties pour éviter des pics de courant simultanés.
 */
typedef enum
{
    /** @brief Toutes les sorties PWM sont synchronisées.*/
    MAX6967_CONF_STAGGER_DISABLED,
    /** @brief Les phases PWM des sorties sont décalées les unes par rapport aux autres.*/
    MAX6967_CONF_STAGGED_ENABLED
}MAX6967ConfPWMStagger_t;;

/**
 * @enum MAX6967ConfOSC_t
 * @brief Choisit entre l'oscillateur interne ou un horloge externe.
 */
typedef enum
{
    /** @brief Oscillateur interne.*/
    MAX6967_CONF_OSC_INTERNAL,
    /** @brief Horloge externe (via la broche OSC).*/
    MAX6967_CONF_OSC_EXTERNAL
}MAX6967ConfOSC_t;

/**
 * @struct MAX6967_t
 * @brief Configuration du module MAX6967
 */
typedef struct
{
    /** @brief Identifiant du module */
    unsigned char id;
    /** @brief Registre de configuration */
    MAX6967ConfReg_t config;
    /** @brief Configuration SPI associé au module */
    SPI_t spi;
}MAX6967_t;


/**
 * @brief Démarre une transmission SPI avec le module MAX6967
 * @param spi Pointeur vers la configuration SPI utilisée pour la communication.
 */
void MAX6967_StartTranmission(SPI_t *spi);

/**
 * @brief Termine une transmission SPI avec le module MAX6967.
 * @param spi Pointeur vers la configuration SPI utilisée pour la communication.
 */
void MAX6967_EndTranmission(SPI_t *spi);

 /**
 * @brief Initialise le module MAX6967 avec la configuration fournie.
 * @param obj Pointeur vers la structure de configuration du module.
 */
void MAX6967_Init(MAX6967_t* obj);

/**
 * @brief Ecrit une donnée dans un registre dans le module MAX6967
 * @param obj Module MAX6967
 * @param reg Regitre à écrire
 * @param data Donnée à écrire
 */
void MAX6967_WriteRegister(MAX6967_t* obj, unsigned char reg, unsigned char data);

/**
 * @brief Lire une donnée dans un registre dans le module MAX6967
 * @param obj Module MAX6967
 * @param reg Registre à lire
 * @return unsigned char Donnnée à lire
 */
unsigned char MAX6967_ReadRegister(MAX6967_t* obj, unsigned char reg);

/* ==== Fonctions Write ==== */

/**
 * @brief Ecrire une donnée dans le registre de configuration du module MAX6967
 * @param obj Module MAX6967
 * @param data Donnée à écrire
 */
void MAX6967_WriteConfigurationReg(MAX6967_t* obj, unsigned char data);

/**
 * @brief Ecrire une donnée dans le registre de port du module MAX6967
 * @param obj Module MAX6967
 * @param port Port
 * @param data Donnée à écrire
 */
void MAX6967_WritePortReg(MAX6967_t* obj, unsigned char port, unsigned char data);

/**
 * @brief Ecrire une donnée dans les registres output current du module MAX6967
 * @param obj Module MAX6967
 * @param data Donnée à écrire
 */
void MAX6967_WriteOutputCurrentReg(MAX6967_t* obj, unsigned char* data);

/**
 * @brief Ecrire une donnée dans le registre global current du module MAX6967
 * @param obj Module MAX6967
 * @param data Donnée à écrire
 */
void MAX6967_WriteGlobalCurrentReg(MAX6967_t* obj, unsigned char data);

/**
 * @brief Ecrire une PWM dans le registre port du module MAX6967
 * @param obj Module MAX6967
 * @param port Port
 * @param pwm Valeur de PWM brute à écrire (entre 0 et 100 %)
 */
void MAX6967_WritePortPWMReg(MAX6967_t* obj, unsigned char port, unsigned char pwm);

/**
 * @brief Met à jour le mode de fonctionnement du module MAX6967 (shutdown ou run)
 * @param obj Module MAX6967
 * @param state Mode de fonctionnement
 */
void MAX6967_SetRunMode(MAX6967_t* obj, unsigned char state);

/**
 * @brief Met le port à 0
 * @param obj Module MAX6967
 * @param port Port à écrire
 */
void MAX6967_DisablePort(MAX6967_t* obj, unsigned char port);

/**
 * @brief Met tous les ports à 0
 * @param obj Module MAX6967
 */
void MAX6967_DisableAllPorts(MAX6967_t* obj);

/* ==== Fonctions Read ==== */

/**
 * @brief Lit le registre configuration du module MAX6967
 * @param obj Module MAX6967
 * @return unsigned char Valeur du registre lue
 */
unsigned char MAX6967_ReadConfigurationReg(MAX6967_t* obj);

/**
 * @brief Lit le registre port du module MAX6967
 * @param obj Module MAX6967
 * @param port Port
 * @return unsigned char Valeur du registre lue 
 */
unsigned char MAX6967_ReadPortReg(MAX6967_t* obj, unsigned char port);

#endif  // PLIB_MAX6967_H
