
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"
#include "driverlib/rom.h"


int
main(void)
{
	uint32_t i;
	FPUEnable();
  FPULazyStackingEnable();
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    //
    while(1)
    {
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);	
			for(i=0;i<100000;i++);	
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,!GPIO_PIN_2);
			for(i=0;i<100000;i++);
    }
}

