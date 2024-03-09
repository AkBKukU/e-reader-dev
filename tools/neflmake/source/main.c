// v1.2 - added sjis support for jap e-reader

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "def.h"
#include "filefunc.h"
#include "crc32.h"
#include "strutil.h"
#include "japtext.h"

//******************************************************************************
const u8 D000_TO_D07F[] =
{
	0x43, 0x61, 0x72, 0x64, 0x2D, 0x45, 0x20, 0x52, 0x65, 0x61, 0x64, 0x65, 0x72, 0x20, 0x32, 0x30,
    0x30, 0x31, 0x00, 0x00, 0x67, 0xB7, 0x2B, 0x2E, 0x32, 0x33, 0x33, 0x33, 0x2F, 0x28, 0x2D, 0x2E,
    0x31, 0x32, 0x33, 0x32, 0x30, 0x2B, 0x2B, 0x30, 0x31, 0x32, 0x34, 0x33, 0x32, 0x2F, 0x2A, 0x2C,
    0x30, 0x33, 0x33, 0x33, 0x31, 0x2F, 0x28, 0x2C, 0x30, 0x33, 0x32, 0x33, 0x32, 0x30, 0x29, 0x2D,
    0x30, 0x30, 0x31, 0x31, 0x2F, 0x2D, 0x23, 0x20, 0x61, 0x05, 0x00, 0x00, 0x80, 0xFD, 0x77, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

struct _ARGS_MAKE_SAVE
{
	u8 type;
	s8 name[256];
	u8 region;
};
typedef struct _ARGS_MAKE_SAVE ARGS_MAKE_SAVE;

const s8 *ARG_TYPE_TO_STR[] = { "nes", "z80", "gba" };
const s8 *ARG_REGION_TO_STR[] = { "usa", "jap" };

#define ARG_TYPE_NES 0
#define ARG_TYPE_Z80 1
#define ARG_TYPE_GBA 2

#define ARG_REGION_JAP 0
#define ARG_REGION_USA 1

//******************************************************************************
void init_save( u8* data)
{
	// 1st bank
	memset( data + 0x00000, 0xFF, 0xD000);
	memcpy( data + 0x0D000, &D000_TO_D07F[0], 0x0080);
	memset( data + 0x0D080, 0x00, 0x0F80);
	memcpy( data + 0x0E000, &D000_TO_D07F[0], 0x0080);
	memset( data + 0x0E080, 0x00, 0x0F80);
	memset( data + 0x0F000, 0xFF, 0x0F80);
	memset( data + 0x0FF80, 0x00, 0x0080);
	// 2nd bank
	memset( data + 0x10000, 0xFF, 0xFF80);
	memset( data + 0x1FF80, 0x00, 0x0080);
}

// SMA4 EUR : 02 07 (vpk)
// SMA4 JAP : 02 07 (no vpk - 0E ..)

//******************************************************************************
u32 make_save( u8* datasrc, u32 sizesrc, u8* datadst, u32 sizedst, ARGS_MAKE_SAVE* args)
{
	u32 i, size_vpk, pos, crc;
	u16 data2;
	u8 isvpk, overflow = 0;
	//
	memset( datadst, 0, sizedst);
	//
	init_save( datadst);
	//
	isvpk = ((sizesrc > 3) && (strncmp( datasrc, "vpk0", 4) == 0));
	//
	memset( datadst + 0x10004, 0, 0x24);
	if (args->region == ARG_REGION_USA)
	{
		memcpy( datadst + 0x10004, args->name, strlen( args->name));
	}
	else
	{
		for (i=0;i<strlen( args->name);i++) *(u16*)(datadst + 0x10004 + (i * 2)) = ascii_to_sjis( args->name[i]);
	}
    // type (gba/nes/z80/compress)
	switch (args->type)
	{
		case ARG_TYPE_GBA : data2 = 0x0402; break; // 0x0602 (ac - jap) 0x0702 (sma4 - eur)
		case ARG_TYPE_NES : data2 = 0x050C; break;
		case ARG_TYPE_Z80 : data2 = 0x0004; break;
		default           : data2 = 0x0000; break;
	}
    if (!isvpk) data2 = data2 | 0x0100;
	//
	*(datadst + 0x10028) = (data2 >> 0) & 0xFF;
	*(datadst + 0x10029) = (data2 >> 8) & 0xFF;
    // skip
	memset( datadst + 0x1002A, 0, 10);
    // compressed data
    if (isvpk)
	{
		if (sizesrc < 0x10000)
		{
			*(datadst + 0x10034) = (sizesrc >> 0) & 0xFF;
			*(datadst + 0x10035) = (sizesrc >> 8) & 0xFF;
			pos = 0x10036;
			size_vpk = sizesrc + 2;
		}
		else
		{
			*(datadst + 0x10034) = (sizesrc >>  0) & 0xFF;
			*(datadst + 0x10035) = (sizesrc >>  8) & 0xFF;
			*(datadst + 0x10036) = (sizesrc >> 16) & 0xFF;
			*(datadst + 0x10037) = (sizesrc >> 24) & 0xFF;
			pos = 0x10038;
			size_vpk = sizesrc + 4;
		}
		if ((pos + sizesrc) > 0x1F000)
		{
			memcpy( datadst + pos, datasrc, 0x1F000 - pos);
			memcpy( datadst + 0, datasrc + (0x1F000 - pos), sizesrc - (0x1F000 - pos));
			overflow = 1;
		}
		else
		{
			memcpy( datadst + pos, datasrc, sizesrc);
			overflow = 0;
		}
	}
	else
	{
		*(datadst + 0x10034) = 0x0E;
		memcpy( datadst + 0x10035, datasrc, sizesrc);
		size_vpk = sizesrc + 1;
	}
    // data size
    *(datadst + 0x1002C) = (size_vpk >>  0) & 0xFF;
    *(datadst + 0x1002D) = (size_vpk >>  8) & 0xFF;
    *(datadst + 0x1002E) = (size_vpk >> 16) & 0xFF;
    *(datadst + 0x1002F) = (size_vpk >> 24) & 0xFF;
    // checksum
	if (overflow)
	{
		crc = crc32( datadst + 0x10004, 0x1F000 - 0x10004, 0xAA478422);
		crc = crc32( datadst + 0, size_vpk + 0x30 - (0x1F000 - 0x10004), crc);
	}
	else
	{
		crc = crc32( datadst + 0x10004, size_vpk + 0x30, 0xAA478422);
	}
    *(u32*)(datadst + 0x10000) = crc;
	//
	return 128 * 1024;
}

//******************************************************************************
void print_header()
{
	printf( "Nintendo e-Reader Flash Maker v1.2\r\n");
	printf( "(c) 2003-2004 Tim Schuerewegen\r\n");
}

//******************************************************************************
void print_usage( s8* arg_exe)
{
	s8 file_exe[256];
	//
	extract_file_name( &file_exe[0], arg_exe);
	printf( "usage :\r\n");
	printf( "  %s [options]\r\n", &file_exe[0]);
	printf( "options :\r\n");
	printf( "  -i <file>          input file                  (required)\r\n");
	printf( "  -o <file>          output file                 (required)\r\n");
	printf( "  -v                 verbose                     (optional)\r\n");
	printf( "  -type <value>      type (0=nes 1=z80 2=gba)    (required)\r\n");
	printf( "  -name <string>     name                        (default = none)\r\n");
	printf( "  -region <value>    region (0=jap 1=usa)        (default = usa)\r\n");
}

//******************************************************************************
int main( int argc, char **argv)
{
	u32 sizesrc, sizedst, size, i, arg_verbose, arg_region;
	int arg_type;
	u8 *datasrc, *datadst;
	s8 arg_file_src[256], arg_file_dst[256], arg_name[256];
	int ret;
	//
	ret = 0;
	// init
	sizesrc = sizedst = 0;
	datasrc = datadst = NULL;
	// default args
	memset( arg_file_src, 0, sizeof(arg_file_src));
	memset( arg_file_dst, 0, sizeof(arg_file_dst));
	memset( arg_name, 0, sizeof(arg_name));
	arg_type = -1;
	arg_verbose = 0;
	arg_region = ARG_REGION_USA;
	// help
	if (argc < 2)
	{
		print_header();
		printf( "\r\n");
		print_usage( argv[0]);
		ret = -1;
	}
	// process args
	if (ret == 0)
	{
		for (i=1;i<(u32)argc;i++)
		{
			if (((i+1) < (u32)argc) && (stricmp( argv[i], "-i") == 0)) { strcpy( arg_file_src, argv[i+1]); i++; }
			if (((i+1) < (u32)argc) && (stricmp( argv[i], "-o") == 0)) { strcpy( arg_file_dst, argv[i+1]); i++; }
			if (((i+0) < (u32)argc) && (stricmp( argv[i], "-v") == 0)) { arg_verbose = 1; }
			if (((i+1) < (u32)argc) && (stricmp( argv[i], "-name") == 0)) { strcpy( arg_name, argv[i+1]); i++; }
			if (((i+1) < (u32)argc) && (stricmp( argv[i], "-type") == 0)) { arg_type = atoi( argv[i+1]); i++; }
			if (((i+1) < (u32)argc) && (stricmp( argv[i], "-region") == 0)) { arg_region = atoi( argv[i+1]); i++; }
		}
	}
	//
	if (arg_verbose) print_header();
	// check args
	if (ret == 0)
	{
		if (arg_file_src[0] == 0) { printf( "\r\n"); printf( "error: required parameter \"-i\" not found\r\n"); ret = -1; }
		if (arg_file_dst[0] == 0) { printf( "\r\n"); printf( "error: required parameter \"-o\" not found\r\n"); ret = -1; }
		if (arg_verbose > 1) { printf( "\r\n"); printf( "error: invalid value for parameter \"-verbose\"\r\n"); ret = -1; }
		if ((arg_type < 0) || (arg_type > 2)) { printf( "\r\n"); printf( "error: invalid value for parameter \"-type\"\r\n"); ret = -1; }
		if (arg_region > 1) { printf( "\r\n"); printf( "error: invalid value for parameter \"-region\"\r\n"); ret = -1; }
	}
	// open/read input file
	if (ret == 0)
	{
		if (!file_load( arg_file_src, &datasrc, &sizesrc))
		{
			printf( "\r\n");
			printf( "error: could not open input file \"%s\"\r\n", arg_file_src);
			ret = -1;
		}
	}
	// allocate output memory
	if (ret == 0)
	{
		sizedst = 128 * 1024;
		datadst = malloc( sizedst);
	}
	//
	if (ret == 0)
	{
		ARGS_MAKE_SAVE args;
		//
		if (arg_verbose)
		{
			printf( "\r\n");
			printf( "input  : %s\r\n", arg_file_src);
			printf( "output : %s\r\n", arg_file_dst);
			printf( "\r\n");
			printf( "type   : %s\r\n", ARG_TYPE_TO_STR[arg_type]);
			printf( "name   : %s\r\n", arg_name);
			printf( "region : %s\r\n", ARG_REGION_TO_STR[arg_region]);
			printf( "\r\n");
			printf( "generating flash file...");
		}
		//
		memset( &args, 0, sizeof(args));
		strcpy( args.name, arg_name);
		args.type = arg_type;
		args.region = arg_region;
		//
		size = make_save( datasrc, sizesrc, datadst, sizedst, &args);
		if (size != 0)
		{
			if (arg_verbose) printf( " done\r\n");
		}
		else
		{
			if (arg_verbose) printf( "\r\n");
			printf( "\r\n");
			printf( "error: could not generate flash file\r\n");
			ret = -1;
		}
	}
	// write output file
	if ((ret== 0) && (size != 0))
	{
		if (!file_save( arg_file_dst, datadst, size))
		{
			printf( "\r\n");
			printf( "error: could not open output file \"%s\"\r\n", arg_file_dst);
			ret = -1;
		}
	}
	//
	if (datasrc) free( datasrc);
	if (datadst) free( datadst);
	//
	return ret;
}
