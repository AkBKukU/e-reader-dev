#include "strutil.h"

#include <string.h>

//******************************************************************************
void extract_file_name( s8* datadst, s8* datasrc)
{
	u8 *ptr1, *ptr2;
	//
	ptr1 = strrchr( datasrc, '\\');
	ptr2 = strrchr( datasrc, '/');
	if ((ptr1) || (ptr2))
	{
		if (ptr1 < ptr2) strcpy( datadst, ptr2 + 1); else strcpy( datadst, ptr1 + 1);
	}
	else
	{
		strcpy( datadst, datasrc);
	}
}
