//*****************************************************************************
//
// bl_config.h - The configurable parameters of the boot loader.
//
// Copyright (c) 2012-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the DK-TM4C123G Firmware Package.
//
//*****************************************************************************

#ifndef __BL_CONFIG_H__
#define __BL_CONFIG_H__

//*****************************************************************************
//
// The following defines are used to configure the operation of the boot
// loader.  For each define, its interactions with other defines are described.
// First is the dependencies (in other words, the defines that must also be
// defined if it is defined), next are the exclusives (in other words, the
// defines that can not be defined if it is defined), and finally are the
// requirements (in other words, the defines that must be defined if it is
// defined).
//
// The following defines must be defined in order for the boot loader to
// operate:
//
//     One of CAN_ENABLE_UPDATE, ENET_ENABLE_UPDATE, I2C_ENABLE_UPDATE,
//            SSI_ENABLE_UPDATE, UART_ENABLE_UPDATE, or USB_ENABLE_UPDATE
//     APP_START_ADDRESS
//     VTABLE_START_ADDRESS
//     FLASH_PAGE_SIZE
//     STACK_SIZE
//
//*****************************************************************************

#define LED_GPIO_PORT_BASE      GPIO_PORTF_BASE
#define LED_GPIO_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOF
#define LED_PORT_PIN             GPIO_PIN_3


//*****************************************************************************
//
// The frequency of the crystal used to clock the microcontroller.
//
// This defines the crystal frequency used by the microcontroller running the
// boot loader.  If this is unknown at the time of production, then use the
// UART_AUTOBAUD feature to properly configure the UART.
//
// Depends on: None
// Exclusive of: None
// Requires: None
//
//*****************************************************************************
#define CRYSTAL_FREQ            16000000

//*****************************************************************************
//
// The starting address of the application.  This must be a multiple of 1024
// bytes (making it aligned to a page boundary).  A vector table is expected at
// this location, and the perceived validity of the vector table (stack located
// in SRAM, reset vector located in flash) is used as an indication of the
// validity of the application image.
//
// The flash image of the boot loader must not be larger than this value.
//
// Depends on: None
// Exclusive of: None
// Requires: None
//
//*****************************************************************************
#define APP_START_ADDRESS       0x2800

//*****************************************************************************
//
// The address at which the application locates its exception vector table.
// This must be a multiple of 1024 bytes (making it aligned to a page
// boundary).  Typically, an application will start with its vector table and
// this value should be set to APP_START_ADDRESS.  This option is provided to
// cater for applications which run from external memory which may not be
// accessible by the NVIC (the vector table offset register is only 30 bits
// long).
//
// Depends on: None
// Exclusive of: None
// Requires: None
//
//*****************************************************************************
#define VTABLE_START_ADDRESS    0x2800

//*****************************************************************************
//
// The size of a single, erasable page in the flash.  This must be a power
// of 2.
//
// Depends on: None
// Exclusive of: None
// Requires: None
//
//*****************************************************************************
#define FLASH_PAGE_SIZE         0x00000400

//*****************************************************************************
//
// The number of words of stack space to reserve for the boot loader.
//
// Depends on: None
// Exclusive of: None
// Requires: None
//
//*****************************************************************************
#define STACK_SIZE              48

//*****************************************************************************
//
// The GPIO module to enable in order to check for a forced update.  This will
// be one of the SYSCTL_RCGC2_GPIOx values, where "x" is replaced with the port
// name (such as B).  The value of "x" should match the value of "x" for
// FORCED_UPDATE_PORT.
//
// Depends on: none
// Exclusive of: None
// Requries: None
//
//*****************************************************************************
#define FORCED_UPDATE_PERIPH    			SYSCTL_RCGC2_GPIOF

//*****************************************************************************
//
// The GPIO port to check for a forced update.  This will be one of the
// GPIO_PORTx_BASE values, where "x" is replaced with the port name (such as
// B).  The value of "x" should match the value of "x" for
// FORCED_UPDATE_PERIPH.
//
// Depends on: none
// Exclusive of: None
// Requries: None
//
//*****************************************************************************
#define FORCED_UPDATE_PORT      					GPIO_PORTF_BASE
#define FORCED_UPDATE_PORT_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOF
//*****************************************************************************
//
// The pin to check for a forced update.  This is a value between 0 and 7.
//
// Depends on: none
// Exclusive of: None
// Requries: None
//
//*****************************************************************************
#define FORCED_UPDATE_PIN       4

//*****************************************************************************
//
// The polarity of the GPIO pin that results in a forced update.  This value
// should be 0 if the pin should be low and 1 if the pin should be high.
//
// Depends on: none
// Exclusive of: None
// Requries: None
//
//*****************************************************************************
#define FORCED_UPDATE_POLARITY  0

//*****************************************************************************
//
// This enables a weak pull-up or pull-down for the GPIO pin used in a forced
// update.  Only one of FORCED_UPDATE_WPU or FORCED_UPDATE_WPD should be
// defined, or neither if a weak pull-up or pull-down is not required.
//
// Depends on: none
// Exclusive of: None
// Requries: None
//
//*****************************************************************************
#define FORCED_UPDATE_WPU
//#define FORCED_UPDATE_WPD

#endif // __BL_CONFIG_H__
