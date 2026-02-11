#include "platform.h"

#if defined(NB_PLATFORM_WINDOWS)
    #include "platform_win32.c"
#elif defined(NB_PLATFORM_LINUX)
    #include "platform_linux.c"
#endif