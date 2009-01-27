/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within this package.
 */
#ifndef _PRSHT_H_
#define _PRSHT_H_

#ifndef _WINRESRC_
#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif
#endif

#ifndef WINCOMMCTRLAPI
#ifndef _COMCTL32_
#define WINCOMMCTRLAPI DECLSPEC_IMPORT
#else
#define WINCOMMCTRLAPI
#endif
#endif

#ifndef CCSIZEOF_STRUCT
#define CCSIZEOF_STRUCT(structname,member) (((int)((LPBYTE)(&((structname*)0)->member) - ((LPBYTE)((structname*)0)))) + sizeof(((structname*)0)->member))
#endif

#ifndef DUMMYUNIONNAME
#ifdef NONAMELESSUNION
#define DUMMYUNIONNAME u
#define DUMMYUNIONNAME2 u2
#define DUMMYUNIONNAME3 u3
#define DUMMYUNIONNAME4 u4
#define DUMMYUNIONNAME5 u5
#else
#define DUMMYUNIONNAME
#define DUMMYUNIONNAME2
#define DUMMYUNIONNAME3
#define DUMMYUNIONNAME4
#define DUMMYUNIONNAME5
#endif
#endif

#ifdef _WIN64
#include <pshpack8.h>
#else
#include <pshpack4.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SNDMSG
#ifdef __cplusplus
#define SNDMSG ::SendMessage
#else
#define SNDMSG SendMessage
#endif
#endif

#define MAXPROPPAGES 100

  struct _PSP;
  struct _PROPSHEETPAGEA;
  struct _PROPSHEETPAGEW;

  typedef struct _PSP *HPROPSHEETPAGE;
  typedef UINT (CALLBACK *LPFNPSPCALLBACKA)(HWND hwnd,UINT uMsg,struct _PROPSHEETPAGEA *ppsp);
  typedef UINT (CALLBACK *LPFNPSPCALLBACKW)(HWND hwnd,UINT uMsg,struct _PROPSHEETPAGEW *ppsp);

#ifdef UNICODE
#define LPFNPSPCALLBACK LPFNPSPCALLBACKW
#else
#define LPFNPSPCALLBACK LPFNPSPCALLBACKA
#endif

#define PSP_DEFAULT 0x00000000
#define PSP_DLGINDIRECT 0x00000001
#define PSP_USEHICON 0x00000002
#define PSP_USEICONID 0x00000004
#define PSP_USETITLE 0x00000008
#define PSP_RTLREADING 0x00000010

#define PSP_HASHELP 0x00000020
#define PSP_USEREFPARENT 0x00000040
#define PSP_USECALLBACK 0x00000080
#define PSP_PREMATURE 0x00000400

#define PSP_HIDEHEADER 0x00000800
#define PSP_USEHEADERTITLE 0x00001000
#define PSP_USEHEADERSUBTITLE 0x00002000
#define PSP_USEFUSIONCONTEXT 0x00004000

#define PSPCB_ADDREF 0
#define PSPCB_RELEASE 1
#define PSPCB_CREATE 2

#define PROPSHEETPAGEA_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETPAGEA,pcRefParent)
#define PROPSHEETPAGEW_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETPAGEW,pcRefParent)

#define PROPSHEETPAGEA_V2_SIZE CCSIZEOF_STRUCT(PROPSHEETPAGEA,pszHeaderSubTitle)
#define PROPSHEETPAGEW_V2_SIZE CCSIZEOF_STRUCT(PROPSHEETPAGEW,pszHeaderSubTitle)

  typedef LPCDLGTEMPLATE PROPSHEETPAGE_RESOURCE;

#define PROPSHEETPAGEA_V1_FIELDS DWORD dwSize; DWORD dwFlags; HINSTANCE hInstance; __extension__ union { LPCSTR pszTemplate; PROPSHEETPAGE_RESOURCE pResource; } DUMMYUNIONNAME; __extension__ union { HICON hIcon; LPCSTR pszIcon; } DUMMYUNIONNAME2; LPCSTR pszTitle; DLGPROC pfnDlgProc; LPARAM lParam; LPFNPSPCALLBACKA pfnCallback; UINT *pcRefParent;
#define PROPSHEETPAGEW_V1_FIELDS DWORD dwSize; DWORD dwFlags; HINSTANCE hInstance; __extension__ union { LPCWSTR pszTemplate; PROPSHEETPAGE_RESOURCE pResource; } DUMMYUNIONNAME; __extension__ union { HICON hIcon; LPCWSTR pszIcon; } DUMMYUNIONNAME2; LPCWSTR pszTitle; DLGPROC pfnDlgProc; LPARAM lParam; LPFNPSPCALLBACKW pfnCallback; UINT *pcRefParent;
#define _PROPSHEETPAGEA_V3 _PROPSHEETPAGEA
#define _PROPSHEETPAGEW_V3 _PROPSHEETPAGEW

  typedef struct _PROPSHEETPAGEA_V1 {
    PROPSHEETPAGEA_V1_FIELDS
  } PROPSHEETPAGEA_V1,*LPPROPSHEETPAGEA_V1;

  typedef const PROPSHEETPAGEA_V1 *LPCPROPSHEETPAGEA_V1;

  typedef struct _PROPSHEETPAGEA_V2 {
    PROPSHEETPAGEA_V1_FIELDS

      LPCSTR pszHeaderTitle;
    LPCSTR pszHeaderSubTitle;
  } PROPSHEETPAGEA_V2,*LPPROPSHEETPAGEA_V2;

  typedef const PROPSHEETPAGEA_V2 *LPCPROPSHEETPAGEA_V2;

  typedef struct _PROPSHEETPAGEA_V3 {
    PROPSHEETPAGEA_V1_FIELDS
      LPCSTR pszHeaderTitle;
    LPCSTR pszHeaderSubTitle;
    HANDLE hActCtx;
  } PROPSHEETPAGEA_V3,*LPPROPSHEETPAGEA_V3;

  typedef const PROPSHEETPAGEA_V3 *LPCPROPSHEETPAGEA_V3;

  typedef struct _PROPSHEETPAGEW_V1 {
    PROPSHEETPAGEW_V1_FIELDS
  } PROPSHEETPAGEW_V1,*LPPROPSHEETPAGEW_V1;

  typedef const PROPSHEETPAGEW_V1 *LPCPROPSHEETPAGEW_V1;

  typedef struct _PROPSHEETPAGEW_V2 {
    PROPSHEETPAGEW_V1_FIELDS
      LPCWSTR pszHeaderTitle;
    LPCWSTR pszHeaderSubTitle;
  } PROPSHEETPAGEW_V2,*LPPROPSHEETPAGEW_V2;

  typedef const PROPSHEETPAGEW_V2 *LPCPROPSHEETPAGEW_V2;

  typedef struct _PROPSHEETPAGEW_V3 {
    PROPSHEETPAGEW_V1_FIELDS
      LPCWSTR pszHeaderTitle;
    LPCWSTR pszHeaderSubTitle;
    HANDLE hActCtx;
  } PROPSHEETPAGEW_V3,*LPPROPSHEETPAGEW_V3;

  typedef const PROPSHEETPAGEW_V3 *LPCPROPSHEETPAGEW_V3;
  typedef PROPSHEETPAGEA_V3 PROPSHEETPAGEA_LATEST;
  typedef PROPSHEETPAGEW_V3 PROPSHEETPAGEW_LATEST;
  typedef LPPROPSHEETPAGEA_V3 LPPROPSHEETPAGEA_LATEST;
  typedef LPPROPSHEETPAGEW_V3 LPPROPSHEETPAGEW_LATEST;
  typedef LPCPROPSHEETPAGEA_V3 LPCPROPSHEETPAGEA_LATEST;
  typedef LPCPROPSHEETPAGEW_V3 LPCPROPSHEETPAGEW_LATEST;

  typedef PROPSHEETPAGEA_V3 PROPSHEETPAGEA;
  typedef PROPSHEETPAGEW_V3 PROPSHEETPAGEW;
  typedef LPPROPSHEETPAGEA_V3 LPPROPSHEETPAGEA;
  typedef LPPROPSHEETPAGEW_V3 LPPROPSHEETPAGEW;
  typedef LPCPROPSHEETPAGEA_V3 LPCPROPSHEETPAGEA;
  typedef LPCPROPSHEETPAGEW_V3 LPCPROPSHEETPAGEW;

#ifdef UNICODE
#define PROPSHEETPAGE PROPSHEETPAGEW
#define LPPROPSHEETPAGE LPPROPSHEETPAGEW
#define LPCPROPSHEETPAGE LPCPROPSHEETPAGEW
#define PROPSHEETPAGE_V1_SIZE PROPSHEETPAGEW_V1_SIZE
#define PROPSHEETPAGE_V2_SIZE PROPSHEETPAGEW_V2_SIZE
#define PROPSHEETPAGE_V1 PROPSHEETPAGEW_V1
#define LPPROPSHEETPAGE_V1 LPPROPSHEETPAGEW_V1
#define LPCPROPSHEETPAGE_V1 LPCPROPSHEETPAGEW_V1
#define PROPSHEETPAGE_V2 PROPSHEETPAGEW_V2
#define LPPROPSHEETPAGE_V2 LPPROPSHEETPAGEW_V2
#define LPCPROPSHEETPAGE_V2 LPCPROPSHEETPAGEW_V2
#define PROPSHEETPAGE_V3 PROPSHEETPAGEW_V3
#define LPPROPSHEETPAGE_V3 LPPROPSHEETPAGEW_V3
#define LPCPROPSHEETPAGE_V3 LPCPROPSHEETPAGEW_V3
#define PROPSHEETPAGE_LATEST PROPSHEETPAGEW_LATEST
#define LPPROPSHEETPAGE_LATEST LPPROPSHEETPAGEW_LATEST
#define LPCPROPSHEETPAGE_LATEST LPCPROPSHEETPAGEW_LATEST
#else
#define PROPSHEETPAGE PROPSHEETPAGEA
#define LPPROPSHEETPAGE LPPROPSHEETPAGEA
#define LPCPROPSHEETPAGE LPCPROPSHEETPAGEA
#define PROPSHEETPAGE_V1_SIZE PROPSHEETPAGEA_V1_SIZE
#define PROPSHEETPAGE_V2_SIZE PROPSHEETPAGEA_V2_SIZE
#define PROPSHEETPAGE_V1 PROPSHEETPAGEA_V1
#define LPPROPSHEETPAGE_V1 LPPROPSHEETPAGEA_V1
#define LPCPROPSHEETPAGE_V1 LPCPROPSHEETPAGEA_V1
#define PROPSHEETPAGE_V2 PROPSHEETPAGEA_V2
#define LPPROPSHEETPAGE_V2 LPPROPSHEETPAGEA_V2
#define LPCPROPSHEETPAGE_V2 LPCPROPSHEETPAGEA_V2
#define PROPSHEETPAGE_V3 PROPSHEETPAGEA_V3
#define LPPROPSHEETPAGE_V3 LPPROPSHEETPAGEA_V3
#define LPCPROPSHEETPAGE_V3 LPCPROPSHEETPAGEA_V3
#define PROPSHEETPAGE_LATEST PROPSHEETPAGEA_LATEST
#define LPPROPSHEETPAGE_LATEST LPPROPSHEETPAGEA_LATEST
#define LPCPROPSHEETPAGE_LATEST LPCPROPSHEETPAGEA_LATEST
#endif

#define PSH_DEFAULT 0x00000000
#define PSH_PROPTITLE 0x00000001
#define PSH_USEHICON 0x00000002
#define PSH_USEICONID 0x00000004
#define PSH_PROPSHEETPAGE 0x00000008
#define PSH_WIZARDHASFINISH 0x00000010
#define PSH_WIZARD 0x00000020
#define PSH_USEPSTARTPAGE 0x00000040
#define PSH_NOAPPLYNOW 0x00000080
#define PSH_USECALLBACK 0x00000100
#define PSH_HASHELP 0x00000200
#define PSH_MODELESS 0x00000400
#define PSH_RTLREADING 0x00000800
#define PSH_WIZARDCONTEXTHELP 0x00001000

#define PSH_WIZARD97 0x01000000
#define PSH_WATERMARK 0x00008000
#define PSH_USEHBMWATERMARK 0x00010000
#define PSH_USEHPLWATERMARK 0x00020000
#define PSH_STRETCHWATERMARK 0x00040000
#define PSH_HEADER 0x00080000
#define PSH_USEHBMHEADER 0x00100000
#define PSH_USEPAGELANG 0x00200000

#define PSH_WIZARD_LITE 0x00400000
#define PSH_NOCONTEXTHELP 0x02000000

  typedef int (CALLBACK *PFNPROPSHEETCALLBACK)(HWND,UINT,LPARAM);

#define PROPSHEETHEADERA_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERA,pfnCallback)
#define PROPSHEETHEADERW_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERW,pfnCallback)

#define PROPSHEETHEADERA_V2_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERA,DUMMYUNION5_MEMBER(hbmHeader))
#define PROPSHEETHEADERW_V2_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERW,DUMMYUNION5_MEMBER(hbmHeader))

  typedef struct _PROPSHEETHEADERA {
    DWORD dwSize;
    DWORD dwFlags;
    HWND hwndParent;
    HINSTANCE hInstance;
    __extension__ union {
      HICON hIcon;
      LPCSTR pszIcon;
    } DUMMYUNIONNAME;
    LPCSTR pszCaption;
    UINT nPages;
    __extension__ union {
      UINT nStartPage;
      LPCSTR pStartPage;
    } DUMMYUNIONNAME2;
    __extension__ union {
      LPCPROPSHEETPAGEA ppsp;
      HPROPSHEETPAGE *phpage;
    } DUMMYUNIONNAME3;
    PFNPROPSHEETCALLBACK pfnCallback;
    __extension__ union {
      HBITMAP hbmWatermark;
      LPCSTR pszbmWatermark;
    } DUMMYUNIONNAME4;
    HPALETTE hplWatermark;
    __extension__ union {
      HBITMAP hbmHeader;
      LPCSTR pszbmHeader;
    } DUMMYUNIONNAME5;
  } PROPSHEETHEADERA,*LPPROPSHEETHEADERA;

  typedef const PROPSHEETHEADERA *LPCPROPSHEETHEADERA;

  typedef struct _PROPSHEETHEADERW {
    DWORD dwSize;
    DWORD dwFlags;
    HWND hwndParent;
    HINSTANCE hInstance;
    __extension__ union {
      HICON hIcon;
      LPCWSTR pszIcon;
    } DUMMYUNIONNAME;
    LPCWSTR pszCaption;
    UINT nPages;
    __extension__ union {
      UINT nStartPage;
      LPCWSTR pStartPage;
    } DUMMYUNIONNAME2;
    __extension__ union {
      LPCPROPSHEETPAGEW ppsp;
      HPROPSHEETPAGE *phpage;
    } DUMMYUNIONNAME3;
    PFNPROPSHEETCALLBACK pfnCallback;
    __extension__ union {
      HBITMAP hbmWatermark;
      LPCWSTR pszbmWatermark;
    } DUMMYUNIONNAME4;
    HPALETTE hplWatermark;
    __extension__ union {
      HBITMAP hbmHeader;
      LPCWSTR pszbmHeader;
    } DUMMYUNIONNAME5;
  } PROPSHEETHEADERW,*LPPROPSHEETHEADERW;

  typedef const PROPSHEETHEADERW *LPCPROPSHEETHEADERW;

#ifdef UNICODE
#define PROPSHEETHEADER PROPSHEETHEADERW
#define LPPROPSHEETHEADER LPPROPSHEETHEADERW
#define LPCPROPSHEETHEADER LPCPROPSHEETHEADERW
#define PROPSHEETHEADER_V1_SIZE PROPSHEETHEADERW_V1_SIZE
#define PROPSHEETHEADER_V2_SIZE PROPSHEETHEADERW_V2_SIZE
#else
#define PROPSHEETHEADER PROPSHEETHEADERA
#define LPPROPSHEETHEADER LPPROPSHEETHEADERA
#define LPCPROPSHEETHEADER LPCPROPSHEETHEADERA
#define PROPSHEETHEADER_V1_SIZE PROPSHEETHEADERA_V1_SIZE
#define PROPSHEETHEADER_V2_SIZE PROPSHEETHEADERA_V2_SIZE
#endif

#define PSCB_INITIALIZED 1
#define PSCB_PRECREATE 2
#define PSCB_BUTTONPRESSED 3

  WINCOMMCTRLAPI HPROPSHEETPAGE WINAPI CreatePropertySheetPageA(LPCPROPSHEETPAGEA constPropSheetPagePointer);
  WINCOMMCTRLAPI HPROPSHEETPAGE WINAPI CreatePropertySheetPageW(LPCPROPSHEETPAGEW constPropSheetPagePointer);
  WINCOMMCTRLAPI WINBOOL WINAPI DestroyPropertySheetPage(HPROPSHEETPAGE);
  WINCOMMCTRLAPI INT_PTR WINAPI PropertySheetA(LPCPROPSHEETHEADERA);
  WINCOMMCTRLAPI INT_PTR WINAPI PropertySheetW(LPCPROPSHEETHEADERW);

#ifdef UNICODE
#define CreatePropertySheetPage CreatePropertySheetPageW
#define PropertySheet PropertySheetW
#else
#define CreatePropertySheetPage CreatePropertySheetPageA
#define PropertySheet PropertySheetA
#endif

  typedef WINBOOL (CALLBACK *LPFNADDPROPSHEETPAGE)(HPROPSHEETPAGE,LPARAM);
  typedef WINBOOL (CALLBACK *LPFNADDPROPSHEETPAGES)(LPVOID,LPFNADDPROPSHEETPAGE,LPARAM);

  typedef struct _PSHNOTIFY {
    NMHDR hdr;
    LPARAM lParam;
  } PSHNOTIFY,*LPPSHNOTIFY;

#define PSN_FIRST (0U-200U)
#define PSN_LAST (0U-299U)

#define PSN_SETACTIVE (PSN_FIRST-0)
#define PSN_KILLACTIVE (PSN_FIRST-1)
#define PSN_APPLY (PSN_FIRST-2)
#define PSN_RESET (PSN_FIRST-3)
#define PSN_HELP (PSN_FIRST-5)
#define PSN_WIZBACK (PSN_FIRST-6)
#define PSN_WIZNEXT (PSN_FIRST-7)
#define PSN_WIZFINISH (PSN_FIRST-8)
#define PSN_QUERYCANCEL (PSN_FIRST-9)
#define PSN_GETOBJECT (PSN_FIRST-10)
#define PSN_TRANSLATEACCELERATOR (PSN_FIRST-12)
#define PSN_QUERYINITIALFOCUS (PSN_FIRST-13)

#define PSNRET_NOERROR 0
#define PSNRET_INVALID 1
#define PSNRET_INVALID_NOCHANGEPAGE 2
#define PSNRET_MESSAGEHANDLED 3

#define PSM_SETCURSEL (WM_USER + 101)
#define PropSheet_SetCurSel(hDlg,hpage,index) SNDMSG(hDlg,PSM_SETCURSEL,(WPARAM)index,(LPARAM)hpage)

#define PSM_REMOVEPAGE (WM_USER + 102)
#define PropSheet_RemovePage(hDlg,index,hpage) SNDMSG(hDlg,PSM_REMOVEPAGE,index,(LPARAM)hpage)

#define PSM_ADDPAGE (WM_USER + 103)
#define PropSheet_AddPage(hDlg,hpage) SNDMSG(hDlg,PSM_ADDPAGE,0,(LPARAM)hpage)

#define PSM_CHANGED (WM_USER + 104)
#define PropSheet_Changed(hDlg,hwnd) SNDMSG(hDlg,PSM_CHANGED,(WPARAM)hwnd,0L)

#define PSM_RESTARTWINDOWS (WM_USER + 105)
#define PropSheet_RestartWindows(hDlg) SNDMSG(hDlg,PSM_RESTARTWINDOWS,0,0L)

#define PSM_REBOOTSYSTEM (WM_USER + 106)
#define PropSheet_RebootSystem(hDlg) SNDMSG(hDlg,PSM_REBOOTSYSTEM,0,0L)

#define PSM_CANCELTOCLOSE (WM_USER + 107)
#define PropSheet_CancelToClose(hDlg) PostMessage(hDlg,PSM_CANCELTOCLOSE,0,0L)

#define PSM_QUERYSIBLINGS (WM_USER + 108)
#define PropSheet_QuerySiblings(hDlg,wParam,lParam) SNDMSG(hDlg,PSM_QUERYSIBLINGS,wParam,lParam)

#define PSM_UNCHANGED (WM_USER + 109)
#define PropSheet_UnChanged(hDlg,hwnd) SNDMSG(hDlg,PSM_UNCHANGED,(WPARAM)hwnd,0L)

#define PSM_APPLY (WM_USER + 110)
#define PropSheet_Apply(hDlg) SNDMSG(hDlg,PSM_APPLY,0,0L)

#define PSM_SETTITLEA (WM_USER + 111)
#define PSM_SETTITLEW (WM_USER + 120)

#ifdef UNICODE
#define PSM_SETTITLE PSM_SETTITLEW
#else
#define PSM_SETTITLE PSM_SETTITLEA
#endif

#define PropSheet_SetTitle(hDlg,wStyle,lpszText) SNDMSG(hDlg,PSM_SETTITLE,wStyle,(LPARAM)(LPCTSTR)(lpszText))

#define PSM_SETWIZBUTTONS (WM_USER + 112)
#define PropSheet_SetWizButtons(hDlg,dwFlags) PostMessage(hDlg,PSM_SETWIZBUTTONS,0,(LPARAM)dwFlags)

#define PSWIZB_BACK 0x00000001
#define PSWIZB_NEXT 0x00000002
#define PSWIZB_FINISH 0x00000004
#define PSWIZB_DISABLEDFINISH 0x00000008

#define PSM_PRESSBUTTON (WM_USER + 113)
#define PropSheet_PressButton(hDlg,iButton) PostMessage(hDlg,PSM_PRESSBUTTON,(WPARAM)iButton,0)

#define PSBTN_BACK 0
#define PSBTN_NEXT 1
#define PSBTN_FINISH 2
#define PSBTN_OK 3
#define PSBTN_APPLYNOW 4
#define PSBTN_CANCEL 5
#define PSBTN_HELP 6
#define PSBTN_MAX 6

#define PSM_SETCURSELID (WM_USER + 114)
#define PropSheet_SetCurSelByID(hDlg,id) SNDMSG(hDlg,PSM_SETCURSELID,0,(LPARAM)id)

#define PSM_SETFINISHTEXTA (WM_USER + 115)
#define PSM_SETFINISHTEXTW (WM_USER + 121)

#ifdef UNICODE
#define PSM_SETFINISHTEXT PSM_SETFINISHTEXTW
#else
#define PSM_SETFINISHTEXT PSM_SETFINISHTEXTA
#endif

#define PropSheet_SetFinishText(hDlg,lpszText) SNDMSG(hDlg,PSM_SETFINISHTEXT,0,(LPARAM)lpszText)

#define PSM_GETTABCONTROL (WM_USER + 116)
#define PropSheet_GetTabControl(hDlg) (HWND)SNDMSG(hDlg,PSM_GETTABCONTROL,0,0)

#define PSM_ISDIALOGMESSAGE (WM_USER + 117)
#define PropSheet_IsDialogMessage(hDlg,pMsg) (WINBOOL)SNDMSG(hDlg,PSM_ISDIALOGMESSAGE,0,(LPARAM)pMsg)

#define PSM_GETCURRENTPAGEHWND (WM_USER + 118)
#define PropSheet_GetCurrentPageHwnd(hDlg) (HWND)SNDMSG(hDlg,PSM_GETCURRENTPAGEHWND,0,0L)

#define PSM_INSERTPAGE (WM_USER + 119)
#define PropSheet_InsertPage(hDlg,index,hpage) SNDMSG(hDlg,PSM_INSERTPAGE,(WPARAM)(index),(LPARAM)(hpage))

#define PSM_SETHEADERTITLEA (WM_USER + 125)
#define PSM_SETHEADERTITLEW (WM_USER + 126)

#ifdef UNICODE
#define PSM_SETHEADERTITLE PSM_SETHEADERTITLEW
#else
#define PSM_SETHEADERTITLE PSM_SETHEADERTITLEA
#endif

#define PropSheet_SetHeaderTitle(hDlg,index,lpszText) SNDMSG(hDlg,PSM_SETHEADERTITLE,(WPARAM)(index),(LPARAM)(lpszText))

#define PSM_SETHEADERSUBTITLEA (WM_USER + 127)
#define PSM_SETHEADERSUBTITLEW (WM_USER + 128)

#ifdef UNICODE
#define PSM_SETHEADERSUBTITLE PSM_SETHEADERSUBTITLEW
#else
#define PSM_SETHEADERSUBTITLE PSM_SETHEADERSUBTITLEA
#endif

#define PropSheet_SetHeaderSubTitle(hDlg,index,lpszText) SNDMSG(hDlg,PSM_SETHEADERSUBTITLE,(WPARAM)(index),(LPARAM)(lpszText))

#define PSM_HWNDTOINDEX (WM_USER + 129)
#define PropSheet_HwndToIndex(hDlg,hwnd) (int)SNDMSG(hDlg,PSM_HWNDTOINDEX,(WPARAM)(hwnd),0)
#define PSM_INDEXTOHWND (WM_USER + 130)
#define PropSheet_IndexToHwnd(hDlg,i) (HWND)SNDMSG(hDlg,PSM_INDEXTOHWND,(WPARAM)(i),0)
#define PSM_PAGETOINDEX (WM_USER + 131)
#define PropSheet_PageToIndex(hDlg,hpage) (int)SNDMSG(hDlg,PSM_PAGETOINDEX,0,(LPARAM)(hpage))
#define PSM_INDEXTOPAGE (WM_USER + 132)
#define PropSheet_IndexToPage(hDlg,i) (HPROPSHEETPAGE)SNDMSG(hDlg,PSM_INDEXTOPAGE,(WPARAM)(i),0)
#define PSM_IDTOINDEX (WM_USER + 133)
#define PropSheet_IdToIndex(hDlg,id) (int)SNDMSG(hDlg,PSM_IDTOINDEX,0,(LPARAM)(id))
#define PSM_INDEXTOID (WM_USER + 134)
#define PropSheet_IndexToId(hDlg,i) SNDMSG(hDlg,PSM_INDEXTOID,(WPARAM)(i),0)
#define PSM_GETRESULT (WM_USER + 135)
#define PropSheet_GetResult(hDlg) SNDMSG(hDlg,PSM_GETRESULT,0,0)
#define PSM_RECALCPAGESIZES (WM_USER + 136)
#define PropSheet_RecalcPageSizes(hDlg) SNDMSG(hDlg,PSM_RECALCPAGESIZES,0,0)

#define ID_PSRESTARTWINDOWS 0x2
#define ID_PSREBOOTSYSTEM (ID_PSRESTARTWINDOWS | 0x1)

#define WIZ_CXDLG 276
#define WIZ_CYDLG 140

#define WIZ_CXBMP 80

#define WIZ_BODYX 92
#define WIZ_BODYCX 184

#define PROP_SM_CXDLG 212
#define PROP_SM_CYDLG 188

#define PROP_MED_CXDLG 227
#define PROP_MED_CYDLG 215

#define PROP_LG_CXDLG 252
#define PROP_LG_CYDLG 218

#ifdef __cplusplus
}
#endif

#include <poppack.h>
#endif
