#ifndef __MD5SUM_H__
#define __MD5SUM_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/*
 * Calculate and store in 'output' the MD5 digest of 'len' bytes at
 * 'input'. 'output' must have enough space to hold 16 bytes.
 */
void md5sum(uint8_t *input, int len, uint8_t output[16]);

void md5sum_str(uint8_t *input, int len, char *outstr);

#endif