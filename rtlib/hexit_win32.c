/* end helper for Windows */

#include <stdlib.h>
#include "fb.h"


/*:::::*/
void fb_hEnd ( int unused )
{

#ifdef ENABLE_MT
	DeleteCriticalSection(&__fb_global_mutex);
	DeleteCriticalSection(&__fb_string_mutex);
	DeleteCriticalSection(&__fb_mtcore_mutex);
#endif

}