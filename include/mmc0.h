#ifndef _MMC0_H_
#define _MMC0_H_

#include "common.h"

void mmc0_init();
byte mmc0_getb(word addr);
void mmc0_setb(word addr, byte b);

#endif /* _MMC0_H_ */
