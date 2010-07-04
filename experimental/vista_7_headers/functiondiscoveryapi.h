#ifndef _INC_FUNCTIONDISCOVERYAPI
#define _INC_FUNCTIONDISCOVERYAPI
#include <windows.h>
#if (_WIN32_WINNT >= 0x0600)

typedef enum tagQueryUpdateAction {
  QUA_ADD      = 0,
  QUA_REMOVE   = 1,
  QUA_CHANGE   = 2 
} QueryUpdateAction;

typedef enum tagSystemVisibilityFlags {
  SVF_SYSTEM   = 0,
  SVF_USER     = 1 
} SystemVisibilityFlags;

#endif /*(_WIN32_WINNT >= 0x0600)*/
#endif /*_INC_FUNCTIONDISCOVERYAPI*/
