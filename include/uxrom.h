#ifndef _UXROM_H_
#define _UXROM_H_

#include "common.h"

void uxrom_init();
byte uxrom_getb(word addr);
void uxrom_setb(word addr, byte b);

#endif /* _UXROM_H_ */

