/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdio.h>

void *aloca(size_t size);
void libera(void *ptr);

#endif //MEMORIA_H
