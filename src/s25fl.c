#include "s25fl.h"

/****************************************************************************************/
//  S25FLxxx Commands
/****************************************************************************************/
//--- Read Device Identification --------------------------------------------------------/
#define FLASH_CMD_READ_ID      0x90    //RDID   (Read Manufacturer & Device ID)
#define FLASH_CMD_RDID         0x9F    //RDID   (Read Identification)
#define FLASH_CMD_RES          0xAB    //RDID   (Read Electronic Signature)
//--- Register Access -------------------------------------------------------------------/
#define FLASH_CMD_RDSR1        0x05    //RDSR1  (Read Status Register 1)
#define   SR1_WIP              0x01    //WIP    (Write In Progress)
#define   SR1_WEL              0x02    //WEL    (Write Enable Latch)
#define   SR1_BP0              0x04    //BP0    (Block Protection)
#define   SR1_BP1              0x08    //BP1
#define   SR1_BP2              0x10    //BP2
#define   SR1_E_ERR            0x20    //E_ERR  (Erase Error Occured)
#define   SR1_P_ERR            0x40    //P_ERR  (Program Error Occured)
#define   SR1_SRWD             0x80    //SRWD   (Status Register Write Disable)
#define FLASH_CMD_RDSR2        0x07    //RDSR2  (Read Status Register 2)
#define   SR2_PS               0x01    //PS     (Program Suspend)
#define   SR2_ES               0x02    //ES     (Erase Suspend)
#define FLASH_CMD_RDCR         0x35    //RDCR   (Read Configuration Register 1)
#define FLASH_CMD_WRR          0x01    //WRR    (Write Status & Configuration Registers)
#define FLASH_CMD_WRDI         0x04    //WRDI   (Write Disable)
#define FLASH_CMD_WREN         0x06    //WREN   (Write Enable)
#define FLASH_CMD_CLSR         0x30    //CLSR   (Reset Erise & Program Fail Flags)
#define FLASH_CMD_ABRD         0x14    //ABRD   (AutoBoot Register Read)
#define FLASH_CMD_ABWR         0x15    //ABWR   (AutoBoot Register Write)
#define FLASH_CMD_BRRD         0x16    //BRRD   (Bank Register Read)
#define FLASH_CMD_BRWR         0x17    //BRWR   (Bank Register Write)
#define FLASH_CMD_BRAC         0xB9    //BRAC   (Bank Register Access)
#define FLASH_CMD_DLPRD        0x41    //DLPRD  (Data Learning Pattern Read)
#define FLASH_CMD_PNVDLR       0x43    //PNVDLR (Program NV Data Learning Register)
#define FLASH_CMD_WVDLR        0x4A    //WVDLR  (Write Volatile Data Learning Register)
//--- Read Flash Array ------------------------------------------------------------------/
#define FLASH_CMD_READ         0x03    //READ       (Read (3- or 4-byte address))
#define FLASH_CMD_4READ        0x13    //4READ      (Read (4-byte address))
#define FLASH_CMD_FAST_READ    0x0B    //FAST_READ  (Fast Read (3- or 4-byte address))
#define FLASH_CMD_4FAST_READ   0x0C    //4FAST_READ (Fast Read (4-byte address))
#define FLASH_CMD_DDRFR        0x0D    //DDRFR      (DDR Fast Read (3- or 4-byte address))
#define FLASH_CMD_4DDRFR       0x0E    //4DDRFR     (DDR Fast Read (4-byte address))
#define FLASH_CMD_DOR          0x3B    //DOR        (Read Dual Out (3- or 4-byte address))
#define FLASH_CMD_4DOR         0x3C    //4DOR       (Read Dual Out (4-byte address))
#define FLASH_CMD_QOR          0x6B    //QOR        (Read Quad Out (3- or 4-byte address))
#define FLASH_CMD_4QOR         0x6C    //4QOR       (Read Quad Out (4-byte address))
#define FLASH_CMD_DIOR         0xBB    //DIOR       (Dual I/O Read (3- or 4-byte address))
#define FLASH_CMD_4DIOR        0xBC    //4DIOR      (Dual I/O Read (4-byte address))
#define FLASH_CMD_DDRDIOR      0xBD    //DDRDIOR    (DDR Dual I/O Read (3- or 4-byte address))
#define FLASH_CMD_4DDRDIOR     0xBE    //4DDRDIOR   (DDR Dual I/O Read (4-byte address))
#define FLASH_CMD_QIOR         0xEB    //QIOR       (Quad I/O Read (3- or 4-byte address))
#define FLASH_CMD_4QIOR        0xEC    //4QIOR      (Quad I/O Read (4-byte address))
#define FLASH_CMD_DDRQIOR      0xED    //DDRQIOR    (DDR Quad I/O Read (3- or 4-byte address))
#define FLASH_CMD_4DDRQIOR     0xEE    //4DDRQIOR   (DDR Quad I/O Read (4-byte address))
//--- Program Flash Array ---------------------------------------------------------------/
#define FLASH_CMD_PP           0x02    //PP    (Page Program (3- or 4-byte address))
#define FLASH_CMD_4PP          0x12    //4PP   (Page Program (4-byte address))
#define FLASH_CMD_QPP          0x32    //QPP   (Quad Page Program (3- or 4-byte address))
#define FLASH_CMD_QPPA         0x38    //QPP   (Quad Page Program - Alternate instruction)
#define FLASH_CMD_4QPP         0x34    //4QPP  (Quad Page Program (4-byte address))
#define FLASH_CMD_PGSP         0x85    //PGSP  (Program Suspend)
#define FLASH_CMD_PGRS         0x8A    //PGRS  (Program Resume)
//--- Erase Flash Array -----------------------------------------------------------------/
#define FLASH_CMD_P4E          0x20    //P4E   (Parameter 4-kB, sector Erase (3- or 4-byte address))
#define FLASH_CMD_4PE4         0x21    //4P4E  (Parameter 4-kB, sector Erase (4-byte address))
#define FLASH_CMD_BE           0x60    //BE    (Bulk Erase)
#define FLASH_CMD_BEA          0xC7    //BE    (Bulk Erase (Alternate command))
#define FLASH_CMD_SE           0xD8    //SE    (Erase 64 kB or 256 kB (3- or 4-byte address))
#define FLASH_CMD_4SE          0xDC    //4SE   (Erase 64 kB or 256 kB (4-byte address))
#define FLASH_CMD_ERSP         0x75    //ERSP  (Erase Suspend)
#define FLASH_CMD_ERRS         0x7A    //ERRS  (Erase Resume)
//--- One Time Program Array ------------------------------------------------------------/
#define FLASH_CMD_OTPP         0x42    //OTPP  (OTP Program)
#define FLASH_CMD_OTPR         0x4B    //OTPR  (OTP Read)
//--- Advanced Sector Protection --------------------------------------------------------/
#define FLASH_CMD_DYBRD        0xE0    //DYBRD   (DYB Read)
#define FLASH_CMD_DYBWR        0xE1    //DYBWR   (DYB Write)
#define FLASH_CMD_PPBRD        0xE2    //PPBRD   (PPB Read)
#define FLASH_CMD_PPBP         0xE3    //PPBP    (PPB Program)
#define FLASH_CMD_PPBE         0xE4    //PPBE    (PPB Erase)
#define FLASH_CMD_ASPRD        0x2B    //ASPRD   (ASP Read)
#define FLASH_CMD_ASPP         0x2F    //ASPP    (ASP Program)
#define FLASH_CMD_PLBRD        0xA7    //PLBRD   (PPB Lock Bit Read)
#define FLASH_CMD_PLBWR        0xA6    //PLBWR   (PPB Lock Bit Write)
#define FLASH_CMD_PASSRD       0xE7    //PASSRD  (Password Read)
#define FLASH_CMD_PASSP        0xE8    //PASSP   (Password Program)
#define FLASH_CMD_PASSU        0xE9    //PASSU   (Password Unlock)
//--- Reset -----------------------------------------------------------------------------/
#define FLASH_CMD_RESET        0xF0    //RESET   (Software Reset)
#define FLASH_CMD_MBR          0xFF    //MBR     (Mode Bit Reset)
//---------------------------------------------------------------------------------------/

//Device ID and Common Flash Interface (ID-CFI)
typedef __packed struct s25fl_idcfi_s
{
  uint8_t   ManufacturerID;
  uint16_t  DeviceID;
  uint8_t   Length;
  uint8_t   SectorArchitecture;
  uint8_t   FamilyID;
  uint8_t   Model[10];
  uint8_t   QueryStringQRY[3];
  uint16_t  PrimaryOEMCommandSet;
  uint16_t  PrimaryExtTableAddr;
  uint16_t  AlternateOEMCommandSet;
  uint16_t  AlternateExtTableAddr;
  uint8_t   VccMin;
  uint8_t   VccMax;
  uint8_t   VppMin;
  uint8_t   VppMax;
  uint8_t   TypicalTimeOutProgramOneByte;
  uint8_t   TypicalTimeOutProgramPage;
  uint8_t   TypicalTimeOutSectorErase;
  uint8_t   TypicalTimeOutFullChipErase;
  uint8_t   MaxTimeOutProgramOneByte;
  uint8_t   MaxTimeOutProgramPage;
  uint8_t   MaxTimeOutSectorErase;
  uint8_t   MaxTimeOutFullChipErase;
  uint8_t   DeviceSize;
  uint16_t  FlashDeviceInterface;
  uint16_t  PageSize;
  uint8_t   UniformOrBootDevice;
  uint8_t   EraseBlockRegion1Info[4];
  uint8_t   EraseBlockRegion2Info[4];
  uint8_t   RFU1[10];
  uint8_t   QueryStringPRI[3];
  uint8_t   Version[2];
  uint8_t   Technology;
  uint8_t   EraseSuspend;
  uint8_t   SectorProtect;
  uint8_t   TempSectorUnprotect;
  uint8_t   SectorProtectUnprotectScheme;
  uint8_t   SimultaneousOperation;
  uint8_t   BurstMode;
  uint8_t   PageModeType;
  uint8_t   ACCSupplyMin;
  uint8_t   ACCSupplyMax;
  uint8_t   WP;
  uint8_t   ProgramSuspend;
} s25fl_idcfi_t, * s25fl_idcfi_p;

//Flash Structure
typedef struct s25fl_s
{
  uint32_t  ID;
  uint32_t  FlashSize;
  uint32_t  SectorSize;
  uint32_t  PageSize;
  uint8_t   TxBuf[S25FL_TX_RX_BUF_LENGTH];
  uint8_t   RxBuf[S25FL_TX_RX_BUF_LENGTH];
} s25fl_t, * s25fl_p;

static s25fl_t         s25fl;
static volatile bool   m_transfer_completed = true;

/* Callback function for SPI events (called if registered) */
//static void S25FL_SPIM_Event_Handler(spi_master_evt_t spi_master_evt)
//{
//  switch(spi_master_evt.evt_type)
//  {
//    case SPI_MASTER_EVT_TRANSFER_COMPLETED:
      // Inform application that transfer is completed.
//      m_transfer_completed = true;
//      break;

//    default:
      // No implementation needed.
//      break;
//  }
//}


/* Flash SoftWare Reset Function */
void S25FL_SoftWareReset(void)
{
  s25fl.TxBuf[0] = FLASH_CMD_RESET;

  spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, 1, s25fl.RxBuf, 1);
}


/* SPI Init Function - Initializes SPI to required mode*/
bool S25FL_SPI_Init(void)
{
  spi_master_config_t SPIC = SPI_MASTER_INIT_DEFAULT;
  
  // Initializing a SPI master driver.
  SPIC.SPI_Freq = SPI_FREQUENCY_FREQUENCY_M8;
  SPIC.SPI_CONFIG_ORDER = SPI_CONFIG_ORDER_MsbFirst;
#if defined(SPI_MASTER_0_ENABLE)
  SPIC.SPI_Pin_SCK  = SPIM0_SCK_PIN;
  SPIC.SPI_Pin_MISO = SPIM0_MISO_PIN;
  SPIC.SPI_Pin_MOSI = SPIM0_MOSI_PIN;
  SPIC.SPI_Pin_SS   = SPIM0_SS_PIN;
#elif defined(SPI_MASTER_1_ENABLE)
  SPIC.SPI_Pin_SCK  = SPIM1_SCK_PIN;
  SPIC.SPI_Pin_MISO = SPIM1_MISO_PIN;
  SPIC.SPI_Pin_MOSI = SPIM1_MOSI_PIN;
  SPIC.SPI_Pin_SS   = SPIM1_SS_PIN;
#endif /* SPI_MASTER_ENABLE */ 
    
  if(spi_master_open(SPI_MASTER_HW, &SPIC) != NRF_SUCCESS) return false;
  
  // Register SPI master event callback
  //spi_master_evt_handler_reg(SPI_MASTER_HW, S25FL_SPIM_Event_Handler);
  
  S25FL_SoftWareReset();
  
  return true;
}


/* Read ID Function - Reads Flash ID and fills Flash Parameters Structure*/
uint32_t S25FL_ReadID(void)
{
  uint32_t ID;
  s25fl_idcfi_p idcfi;

  s25fl.TxBuf[0] = FLASH_CMD_READ_ID; //0x90XXXXXXXXXX - Command
  s25fl.TxBuf[1] = 0;                 //0xXX000000XXXX - Address
  s25fl.TxBuf[2] = 0;
  s25fl.TxBuf[3] = 0;
  s25fl.TxBuf[4] = 0;                 //0xXXXXXXXX01XX - Manufacture ID
  s25fl.TxBuf[5] = 0;                 //0xXXXXXXXXXXYY - Device ID
  
  s25fl.ID = 0;
  
  if(spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, 6, s25fl.RxBuf, 6) != NRF_SUCCESS)
    return 0;
  
  ID = (s25fl.RxBuf[4] << 8) | s25fl.RxBuf[5];
  
  if((ID == 0) || (ID == 0xFFFF)) return 0;
  
  s25fl.TxBuf[0] = FLASH_CMD_RDID;
  if(spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, 82, s25fl.RxBuf, 82) != NRF_SUCCESS)
    return 0;

  idcfi = (s25fl_idcfi_p)&s25fl.RxBuf[1];
  
  if((idcfi->ManufacturerID == 0) || (idcfi->ManufacturerID == 0xFF)) return 0;

  s25fl.ID        = ID;
  s25fl.FlashSize = (1 << idcfi->DeviceSize);
  if(idcfi->SectorArchitecture == 0x00)
    s25fl.SectorSize = 262144;
  else if(idcfi->SectorArchitecture == 0x01)
    s25fl.SectorSize = 65536;
  else
   s25fl.SectorSize = 65536;
  s25fl.PageSize = (1 << idcfi->PageSize);
  
  return ID;
}


/* Init Function - Initializes SPI and some internal variables */
bool S25FL_Init(void)
{
  return((S25FL_SPI_Init()) && (S25FL_ReadID() == S25FL_ID));
}


/* Read Flash Status Function */
uint8_t S25FL_ReadStatus(void)
{
  s25fl.TxBuf[0] = FLASH_CMD_RDSR1;
  s25fl.TxBuf[1] = 0x00;
  
  if(spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, 2, s25fl.RxBuf, 2) != NRF_SUCCESS)
    return 0;

  return s25fl.RxBuf[1];
}


/* Flash Is Busy Function */
bool S25FL_IsBusy(void)
{
  return((S25FL_ReadStatus() & SR1_WIP) != 0);
}


/* Flash Write Enable Command Sending Function */
void S25FL_EnableWrite(void)
{
  s25fl.TxBuf[0] = FLASH_CMD_WREN;

  spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, 1, s25fl.RxBuf, 1);
}


/* Flash Program Page Function */
/*   Parameters */
/*    - Address - Page Address !WARNING! Must be aligned to Page Size */
/*    - Data    - Page Data */
/*    - Size    - Page Size */
/*   Result */
/*    true - if Page Programed Successfuly */
bool S25FL_ProgramPage(uint32_t Address, uint8_t * Data, uint32_t Size)
{
  uint8_t  TryCounter;
  uint16_t i;

  //Check Flash Address
  if(Address > (s25fl.FlashSize - 1)) return false;

  //Check Flash Page Size
  if((Size > s25fl.PageSize) || (Size == 0)) return false;

  //Check Flash is Busy or Not
  TryCounter = 200;
  while(S25FL_IsBusy() && TryCounter) TryCounter--;
  if(TryCounter == 0) return false;

  //Setting Write Enable Latch Bit
  S25FL_EnableWrite();
  
  //Fill Buffer
  s25fl.TxBuf[0] = FLASH_CMD_4PP;
  s25fl.TxBuf[1] = (Address >> 24) & 0xFF;
  s25fl.TxBuf[2] = (Address >> 16) & 0xFF;
  s25fl.TxBuf[3] = (Address >>  8) & 0xFF;
  s25fl.TxBuf[4] = (Address      ) & 0xFF;
  for(i = 0; i < s25fl.PageSize; i++) s25fl.TxBuf[i + 5] = *Data++;

  //Write Page Program Command and Load Data Into Flash's Buffer
  if(spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, (Size + 5), s25fl.RxBuf, (Size + 5)) != NRF_SUCCESS)
    return false;

  //Check Flash is Busy or Not
  TryCounter = 200;
  while(S25FL_IsBusy() && TryCounter)
  {
    nrf_delay_us(30);
    TryCounter--;
  }
  if(TryCounter == 0) return false;

  return true;
}


/* Flash Write Buffer Function */
/*   Parameters */
/*    - Address - Address !WARNING! Must be aligned to Page Size */
/*    - Buffer  - Data to Write */
/*    - Size    - Data Size */
/*   Result */
/*    Size of Written Data */
uint32_t S25FL_WriteBuffer(uint32_t Address, uint8_t * Buffer, uint32_t Size)
{
  uint32_t PageCount = 0, RestOfPageSize = 0, Addr = 0;

  Addr = Address % s25fl.PageSize;

  PageCount =  Size / s25fl.PageSize;
  RestOfPageSize = Size % s25fl.PageSize;

  //Write Address is Flash Page Size Aligned
  if(Addr == 0)
  {
    //Bytes To Write < Flash Page Size
    if(PageCount == 0)
    {
      if(!S25FL_ProgramPage(Address, Buffer, Size)) return 0;
    }
    //Bytes To Write > Flash Page Size
    else
    {
      while(PageCount--)
      {
        if(!S25FL_ProgramPage(Address, Buffer, s25fl.PageSize)) return 0;
        Address += s25fl.PageSize;
        Buffer += s25fl.PageSize;
      }
      if(RestOfPageSize > 0)
      {
        if(!S25FL_ProgramPage(Address, Buffer, RestOfPageSize)) return 0;
      }
    }
  }
  else return 0;

  return Size;
}


/* Flash Read Buffer Function */
/*   Parameters */
/*    - Address - Address, can be any */
/*    - Buffer  - Buffer for Data */
/*    - Size    - Data Size */
/*   Result */
/*    Size of data which have been read */
uint32_t S25FL_ReadBuffer(uint32_t Address, uint8_t * Buffer, uint32_t Size)
{
  uint32_t PageCount = 0, RestOfPageSize = 0, i;

  PageCount =  Size / s25fl.PageSize;
  RestOfPageSize = Size % s25fl.PageSize;
  
  //Bytes To Read < Flash Page Size
  if(PageCount == 0)
  {
    s25fl.TxBuf[0] = FLASH_CMD_4READ;
    s25fl.TxBuf[1] = (Address >> 24) & 0xFF;
    s25fl.TxBuf[2] = (Address >> 16) & 0xFF;
    s25fl.TxBuf[3] = (Address >>  8) & 0xFF;
    s25fl.TxBuf[4] = (Address      ) & 0xFF;
    
    if(spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, (Size + 5), s25fl.RxBuf, (Size + 5)) != NRF_SUCCESS)
      return 0;
    
    for(i = 0; i < Size; i++) Buffer[i] = s25fl.RxBuf[i + 5];
  }
  //Bytes To Read > Flash Page Size
  else
  {
    while(PageCount--)
    {
      s25fl.TxBuf[0] = FLASH_CMD_4READ;
      s25fl.TxBuf[1] = (Address >> 24) & 0xFF;
      s25fl.TxBuf[2] = (Address >> 16) & 0xFF;
      s25fl.TxBuf[3] = (Address >>  8) & 0xFF;
      s25fl.TxBuf[4] = (Address      ) & 0xFF;
    
      if(spi_master_send_recv(SPI_MASTER_HW, 
                              s25fl.TxBuf, (s25fl.PageSize + 5), 
                              s25fl.RxBuf, (s25fl.PageSize + 5)) != NRF_SUCCESS)
        return 0;
      
      for(i = 0; i < s25fl.PageSize; i++) Buffer[i] = s25fl.RxBuf[i + 5];
      
      Address += s25fl.PageSize;
      Buffer += s25fl.PageSize;
    }
    
    if(RestOfPageSize > 0)
    {
      s25fl.TxBuf[0] = FLASH_CMD_4READ;
      s25fl.TxBuf[1] = (Address >> 24) & 0xFF;
      s25fl.TxBuf[2] = (Address >> 16) & 0xFF;
      s25fl.TxBuf[3] = (Address >>  8) & 0xFF;
      s25fl.TxBuf[4] = (Address      ) & 0xFF;
      
      if(spi_master_send_recv(SPI_MASTER_HW, 
                              s25fl.TxBuf, (RestOfPageSize + 5), 
                              s25fl.RxBuf, (RestOfPageSize + 5)) != NRF_SUCCESS)
        return 0;
      
      for(i = 0; i < RestOfPageSize; i++) Buffer[i] = s25fl.RxBuf[i + 5];
    }
  }  

  return Size;
}


/* Flash Sector Erase Function */
/*   Parameters */
/*    - Sector  - Sector Address !WARNING! Must be aligned to Sector Size */
/*   Result */
/*    true - if Sector was erased Successfuly */
bool S25FL_SectorErase(uint32_t Sector)
{
  uint8_t TryCounter;

  //Check Sector Address
  if(Sector > (s25fl.FlashSize - 1)) return false;

  //Check Flash is Busy or Not
  TryCounter = 200;
  while(S25FL_IsBusy() && TryCounter) TryCounter--;
  if(TryCounter == 0) return false;
    
  //Setting Write Enable Latch Bit
  S25FL_EnableWrite();

  s25fl.TxBuf[0] = FLASH_CMD_4SE;
  s25fl.TxBuf[1] = (Sector >> 24) & 0xFF;
  s25fl.TxBuf[2] = (Sector >> 16) & 0xFF;
  s25fl.TxBuf[3] = (Sector >>  8) & 0xFF;
  s25fl.TxBuf[4] = (Sector      ) & 0xFF;
    
  //Write Sector Erase Command and Sector Address
  if(spi_master_send_recv(SPI_MASTER_HW, s25fl.TxBuf, 5, s25fl.RxBuf, 5) != NRF_SUCCESS)
    return false;

  //Check Flash is Busy or Not
  TryCounter = 200;
  while(S25FL_IsBusy() && TryCounter)
  {
    nrf_delay_ms(10);
    TryCounter--;
  }
  if(TryCounter == 0) return false;
    
  return true;
}

