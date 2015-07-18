#ifndef S25FL_H
#define S25FL_H

#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "nrf_error.h"
#include "nrf_delay.h"
#include "nrf51_bitfields.h"
#include "app_util_platform.h"
#include "spi_master.h"

//Define memory type (must be S25FL128S or S25FL256S)
#define S25FL256S

#if defined(SPI_MASTER_0_ENABLE)
  #define SPI_MASTER_HW SPI_MASTER_0
#elif defined(SPI_MASTER_1_ENABLE)
  #define SPI_MASTER_HW SPI_MASTER_1
#else
  #error "No SPI enabled"
#endif

//Manufacture ID          == 0x01
//Device ID for S25FL128S == 0x17
//Device ID for S25FL256S == 0x18
#define S25FL128S_ID                    0x00000117
#define S25FL256S_ID                    0x00000118

#if defined(S25FL128S)
  //S25FL128 ID
  #define S25FL_ID    S25FL128S_ID
  //--- S25FL128 Capacity is 16,777,216 Bytes ---/
  //  (256 sectors) * (65,536 bytes per sector)
  //  (65536 pages) * (256 bytes per page) 
  //Sector size 1 << 16 = 65,536
  //Page size 1 << 8 = 256
#elif defined(S25FL256S)
  //S25FL128 ID
  #define S25FL_ID    S25FL256S_ID
  //--- S25FL256 Capacity is 33,554,432 Bytes ---/
  //  (512 sectors) * (65,536 bytes per sector)
  //  (131072 pages) * (256 bytes per page) 
  //Sector size 1 << 16 = 65,536
  //Page size 1 << 8 = 256
#else
  #error "No S25FL Memory Selected"
#endif

#define S25FL_TX_RX_BUF_LENGTH          520

bool     S25FL_Init(void);
bool     S25FL_SectorErase(uint32_t Sector);
uint32_t S25FL_WriteBuffer(uint32_t Address, uint8_t * Buffer, uint32_t Size);
uint32_t S25FL_ReadBuffer(uint32_t Address, uint8_t * Buffer, uint32_t Size);


#endif //S25FL_H