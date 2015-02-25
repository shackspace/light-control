#ifndef _AVR_WDT_H_
#define _AVR_WDT_H_

#define WDTO_15MS   0
#define wdt_enable(value) set_reboot(); return;

#endif

