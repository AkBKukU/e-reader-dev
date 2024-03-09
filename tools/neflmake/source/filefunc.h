#ifndef FILEFUNC_H
#define FILEFUNC_H

#include "def.h"

#include <stdio.h>

long fsize( FILE *fp);
u8 file_load( s8 *filename, u8 **data, u32 *size);
u8 file_save( s8 *filename, u8 *data, u32 size);

#endif
