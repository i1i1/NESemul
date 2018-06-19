#ifndef _MMC2_H_
#define _MMC2_H_

#include "common.h"

void mmc2_init();
byte mmc2_getb(word addr);
void mmc2_setb(word addr, byte b);

#endif /* _MMC2_H_ */

