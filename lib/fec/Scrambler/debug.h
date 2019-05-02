/* 
 * @author Moses Browne Mwakyanjala
 * Copyright Feb 2018, Moses Browne Mwakyanjala
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */
#ifndef DEBUG_H
#define DEBUG_H
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
void inline hexdump(void *ptr, int buflen) {
  unsigned char *buf = (unsigned char*) ptr;
  int i, j;
  for (i = 0; i < buflen; i += 16) {
    printf("%06x: ", i);
    for (j = 0; j < 16; j++)
      if (i + j < buflen)
	printf("%02x ", buf[i + j]);
      else
	printf("   ");
    printf(" ");
    for (j = 0; j < 16; j++)
      if (i + j < buflen)
	printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
    printf("\n");
  }
}
#endif
