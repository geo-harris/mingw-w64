#ifndef _INC_WCT
#define _INC_WCT
#include <windows.h>
#if (_WIN32_WINNT >= 0x0600)

typedef LPVOID HWCT;

typedef enum _WCT_OBJECT_TYPE {
  WctCriticalSectionType,
  WctSendMessageType,
  WctMutexType,
  WctAlpcType,
  WctComType,
  WctThreadWaitType,
  WctProcessWaitType,
  WctThreadType,
  WctComActivationType,
  WctUnknownType
} WCT_OBJECT_TYPE;

typedef enum _WCT_OBJECT_STATUS {
  WctStatusNoAccess,
  WctStatusRunning,
  WctStatusBlocked,
  WctStatusPidOnly,
  WctStatusPidOnlyRpcss,
  WctStatusOwned,
  WctStatusNotOwned,
  WctStatusAbandoned,
  WctStatusUnknown,
  WctStatusError
} WCT_OBJECT_STATUS;

/* According to http://msdn.microsoft.com/en-us/magazine/cc163395.aspx
  RealObjectName has 0x8 offset and TimeOutLowPart has 0x108
  WCT_OBJNAME_LENGTH assumed to be 50
*/
#define WCT_OBJNAME_LENGTH 50

typedef struct _WAITCHAIN_NODE_INFO {
  WCT_OBJECT_TYPE   ObjectType;
  WCT_OBJECT_STATUS ObjectStatus;
  union DUMMYUNIONNAME {
    struct DUMMYSTRUCTNAME1 {
      WCHAR         ObjectName[WCT_OBJNAME_LENGTH];
      LARGE_INTEGER Timeout;
      WINBOOL       Alertable;
    } LockObject;
    struct DUMMYSTRUCTNAME2 {
      DWORD ProcessId;
      DWORD ThreadId;
      DWORD WaitTime;
      DWORD ContextSwitches;
    } ThreadObject;
  } ;
} WAITCHAIN_NODE_INFO, *PWAITCHAIN_NODE_INFO;

typedef VOID (CALLBACK *PWAITCHAINCALLBACK)(
    HWCT WctHandle,
    DWORD_PTR Context,
    DWORD CallbackStatus,
    LPDWORD NodeCount,
    PWAITCHAIN_NODE_INFO NodeInfoArray,
    LPBOOL IsCycle
);

HWCT WINAPI OpenThreadWaitChainSession(DWORD Flags,PWAITCHAINCALLBACK callback);
VOID WINAPI CloseThreadWaitChainSession(HWCT WctHandle);
WINBOOL WINAPI GetThreadWaitChain(
  HWCT WctHandle,
  DWORD_PTR Context,
  DWORD Flags,
  DWORD ThreadId,
  LPDWORD NodeCount,
  PWAITCHAIN_NODE_INFO NodeInfoArray,
  LPBOOL IsCycle
);

/* Unknown Callbacks */
typedef LPVOID (WINAPI *PCOGETCALLSTATE)();
typedef LPVOID (WINAPI *PCOGETACTIVATIONSTATE)();

VOID WINAPI RegisterWaitChainCOMCallback(
  PCOGETCALLSTATE CallStateCallback,
  PCOGETACTIVATIONSTATE ActivationStateCallback
);

#endif /* (_WIN32_WINNT >= 0x0600) */
#endif /*_INC_WCT*/
