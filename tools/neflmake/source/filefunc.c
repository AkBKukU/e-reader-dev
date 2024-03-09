#include "filefunc.h"

#include <stdlib.h>

//******************************************************************************
long fsize( FILE *fp)
{
	long pos, size;
	pos = ftell( fp);
	fseek( fp, 0, SEEK_END);
	size = ftell( fp);
	fseek( fp, pos, SEEK_SET);
	return size;
}

//******************************************************************************
u8 file_load( s8 *filename, u8 **data, u32 *size)
{
	FILE *fp;
	// init
	*data = NULL;
	*size = 0;
	// open file
	fp = fopen( filename, "rb");
	if (!fp) return 0;
	// alloc mem
	*size = (u32)fsize( fp);
	*data = malloc( *size);
	// read file
	fread( *data, 1, *size, fp);
	// close file
	fclose( fp);
	//
	return 1;
}

//******************************************************************************
u8 file_save( s8 *filename, u8 *data, u32 size)
{
	FILE *fp;
	// open file
	fp = fopen( filename, "wb");
	if (!fp) return 0;
	// write file
	fwrite( data, 1, size, fp);
	// close file
	fclose( fp);
	//
	return 1;
}
