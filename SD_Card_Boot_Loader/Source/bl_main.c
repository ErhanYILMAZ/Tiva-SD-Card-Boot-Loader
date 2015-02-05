//*****************************************************************************
// Erhan YILMAZ
// SD Card Bootloader for Tiva C series mcu's
// 28.01.2015 / Version 1.0
// bl_main.c - The file holds the main control loop of the boot loader.
// Note: Requires TivaWare C Series library to compiling project
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/rom.h"
#include "inc/hw_gpio.h"
#include "inc/hw_flash.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_types.h"
#include "bl_config.h"

#include "Petit/pff.h"
#include "Petit/pffconf.h"

// Specifies how many bytes read from file to write flash at one time
// if higher increase ram size decrease program load time
// else decrease ram size increase program load time
#define	WRITE_DATA_PACKET_SIZE	128


//*****************************************************************************
//
// Make sure that the application start address falls on a flash page boundary
//
//*****************************************************************************
#if (APP_START_ADDRESS & (FLASH_PAGE_SIZE - 1))
#error ERROR: APP_START_ADDRESS must be a multiple of FLASH_PAGE_SIZE bytes!
#endif



//*****************************************************************************
//
// A prototype for the function (in the startup code) for calling the
// application.
//
//*****************************************************************************
extern void CallApplication(uint32_t ui32Base);

//*****************************************************************************
//
// A prototype for the function (in the startup code) for a predictable length
// delay.
//
//*****************************************************************************
extern void Delay(uint32_t ui32Count);

//*****************************************************************************
//
//! Configures the microcontroller.
//!
//! This function configures the peripherals and GPIOs of the microcontroller,
//! preparing it for use by the boot loader.  The interface that has been
//! selected as the update port will be configured, and auto-baud will be
//! performed if required.
//!
//! \return None.
//
//*****************************************************************************
void
ConfigureDevice(void)
{

#ifdef CRYSTAL_FREQ
    //
    // Since the crystal frequency was specified, enable the main oscillator
    // and clock the processor from it.
    //
    HWREG(SYSCTL_RCC) &= ~(SYSCTL_RCC_MOSCDIS);
    Delay(524288);
    HWREG(SYSCTL_RCC) = ((HWREG(SYSCTL_RCC) & ~(SYSCTL_RCC_OSCSRC_M)) |
                         SYSCTL_RCC_OSCSRC_MAIN);
#endif

}

	
BYTE bWriteBuffer[WRITE_DATA_PACKET_SIZE];

//*****************************************************************************
//
//! This function performs the update on the selected port.
//!
//! This function is called directly by the boot loader or it is called as a
//! result of an update request from the application.
//!
//! \return Never returns.
//
//*****************************************************************************
void
Updater(void)
{
	uint32_t EraseSize=0;
	uint32_t AppAddress=0; 
	uint32_t i,j;	
	uint32_t	WriteDataPacketCount,WriteDataPacketRemainder;

	FRESULT rc; 
	FATFS fatfs;	  	 
	UINT br;

	// Led gpio pin settings
	ROM_SysCtlPeripheralEnable(LED_GPIO_SYSCTL_PERIPH);
	ROM_GPIOPinTypeGPIOOutput(LED_GPIO_PORT_BASE, LED_PORT_PIN);

	// Led blink
	ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,LED_PORT_PIN);	
	for(i=0;i<100000;i++);	
	ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,!LED_PORT_PIN);
	for(i=0;i<100000;i++);
	
// try 10 times to mounting sd card. Blink led on every try.
		j=0;
		do{
			rc = pf_mount(&fatfs);
			ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,LED_PORT_PIN);	
			for(i=0;i<100000;i++);	
			ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,!LED_PORT_PIN);
			for(i=0;i<100000;i++);
			j++;
			}
			while(rc && j<10); 
			
// if fail sd card mounting exit otherwise continue
	if(!rc){
		
// try 10 times to opening app.bin file which in sd card(if exist). Blink led on every try.		
		j=0;
		do{
			rc = pf_open("app.bin");
			ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,LED_PORT_PIN);	
			for(i=0;i<100000;i++);	
			ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,!LED_PORT_PIN);
			for(i=0;i<100000;i++);
			j++;
			}
		while(rc && j<10);            
	
// if fail app.bin file opening exit otherwise continue		
	if(!rc)
		{
// if file size is not multiple of 4 exit otherwise continue	
		if((fatfs.fsize & 0x03)==0)
			{	
				// Calculate page count that will erase according to app.bin file size
				EraseSize = fatfs.fsize/FLASH_PAGE_SIZE;
				if(fatfs.fsize%FLASH_PAGE_SIZE)
				EraseSize++;
				
				// Erase necessary pages
				AppAddress=APP_START_ADDRESS;
				for(i=0;i<EraseSize;i++)
				{
					ROM_FlashErase(AppAddress);
					AppAddress += FLASH_PAGE_SIZE;
				}
		
		AppAddress=APP_START_ADDRESS;			// Set app address to write
		// Calculate packet count according to write data packet size that user defined
		WriteDataPacketCount=fatfs.fsize/WRITE_DATA_PACKET_SIZE;
		// Calculate remainder of division
		WriteDataPacketRemainder=fatfs.fsize%WRITE_DATA_PACKET_SIZE;
				
		// Read number of WRITE_DATA_PACKET_SIZE bytes from app.bin file and 
		// write it to the flash memory number of WriteDataPacketCount times.
		for(i=0;i<WriteDataPacketCount;i++){
			pf_read(bWriteBuffer,WRITE_DATA_PACKET_SIZE,&br);
			ROM_FlashProgram((uint32_t*)bWriteBuffer,AppAddress,WRITE_DATA_PACKET_SIZE);	
			AppAddress += WRITE_DATA_PACKET_SIZE;
		}
		
		// Read 4 bytes from app.bin file and 
		// write it to the flash memory number of WriteDataPacketRemainder times.		
		for(i=0;i<WriteDataPacketRemainder/4;i++)
		{
			pf_read(bWriteBuffer,4,&br);
			ROM_FlashProgram((uint32_t*)bWriteBuffer,AppAddress,4);
			AppAddress += 4;
		}
		
		// If done blink led 2 times with long delay.
		ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,LED_PORT_PIN);	
		for(i=0;i<1000000;i++);	
		ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,!LED_PORT_PIN);
		for(i=0;i<1000000;i++);
		ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,LED_PORT_PIN);	
		for(i=0;i<1000000;i++);	
		ROM_GPIOPinWrite(LED_GPIO_PORT_BASE,LED_PORT_PIN,!LED_PORT_PIN);
		for(i=0;i<1000000;i++);
		
		// Reset and disable the SSI peripheral that used by the boot loader.
		ROM_SysCtlPeripheralDisable(SDC_SSI_SYSCTL_PERIPH);
		ROM_SysCtlPeripheralReset(SDC_SSI_SYSCTL_PERIPH);
		// Reset and disable the GPIO peripheral that used by the boot loader.
    ROM_SysCtlPeripheralDisable(SDC_GPIO_SYSCTL_PERIPH);
		ROM_SysCtlPeripheralReset(SDC_GPIO_SYSCTL_PERIPH);
		// Reset and disable the GPIO peripheral that used by the boot loader.
    ROM_SysCtlPeripheralDisable(LED_GPIO_SYSCTL_PERIPH);
		ROM_SysCtlPeripheralReset(LED_GPIO_SYSCTL_PERIPH);
		// Reset and disable the GPIO peripheral that used by the boot loader.
    ROM_SysCtlPeripheralDisable(FORCED_UPDATE_PORT_SYSCTL_PERIPH);
		ROM_SysCtlPeripheralReset(FORCED_UPDATE_PORT_SYSCTL_PERIPH);

		//((int (*)(void))APP_START_ADDRESS)();	
		//HWREG(NVIC_APINT) = (NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ);
		//while(1);
		
			}
		}
	}

// Reset	
		HWREG(NVIC_APINT) = (NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ);
	
}
