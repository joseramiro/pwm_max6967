#ifndef PLIB_MAX6967_SPI_H
#define PLIB_MAX6967_SPI_H

/**
 * @file plib_max6967_spi.h
 * @brief Pilote SPI pour le MAX6967 (driver de PWM)
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

#include "libs/common_c_libs/plib_comm_struct.h"

/** @brief Masque pour commande en mode lecture */
#define MAX6967_READ_MODE           0x80

/**
 * @brief Écrit des données dans le MAX6967 via SPI.
 * @param spi Pointeur vers la configuration SPI utilisée pour la communication.
 * @param reg Registre à ecrire
 * @param data Donnée à écrire
 */
void MAX6967_Write(SPI_t *spi, unsigned char reg, unsigned char val);

/**
 * @brief Lit des données depuis le MAX6967 via SPI.
 * @param spi Pointeur vers la configuration SPI utilisée pour la communication
 * @param reg Regitre à lire
 * @return unsigned char Donnée lue
 */
unsigned char MAX6967_Read(SPI_t *spi, unsigned char reg);

#endif  // PLIB_MAX6967_SPI_H
