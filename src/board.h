#ifndef BOARD_H
#define BOARD_H

//#define SPI_MASTER_0_ENABLE

#define SPIM0_SCK_PIN       19    /**< SPI clock GPIO pin number. */
#define SPIM0_MOSI_PIN      17    /**< SPI Master Out Slave In GPIO pin number. */
#define SPIM0_MISO_PIN      9     /**< SPI Master In Slave Out GPIO pin number. */
#define SPIM0_SS_PIN        7     /**< SPI Slave Select GPIO pin number. */

// Yuriy PCB
//#define SPIM0_SCK_PIN       15    /**< SPI clock GPIO pin number. */
//#define SPIM0_MOSI_PIN      16    /**< SPI Master Out Slave In GPIO pin number. */
//#define SPIM0_MISO_PIN      14     /**< SPI Master In Slave Out GPIO pin number. */
//#define SPIM0_SS_PIN        13     /**< SPI Slave Select GPIO pin number. */

#define SPIM1_SCK_PIN       19     /**< SPI clock GPIO pin number. */
#define SPIM1_MOSI_PIN      17     /**< SPI Master Out Slave In GPIO pin number. */
#define SPIM1_MISO_PIN      7      /**< SPI Master In Slave Out GPIO pin number. */
#define SPIM1_SS_PIN        9      /**< SPI Slave Select GPIO pin number. */

//#define TWI0_SCL_PIN        7
//#define TWI0_SDA_PIN        9

//#define TWI1_SCL_PIN        7
//#define TWI1_SDA_PIN        9

#endif // BOARD_H