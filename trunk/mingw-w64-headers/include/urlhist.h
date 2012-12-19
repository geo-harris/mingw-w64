/*** Autogenerated by WIDL 1.5.19 from include/urlhist.idl - Do not edit ***/

#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include <rpc.h>
#include <rpcndr.h>

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#include <ole2.h>
#endif

#ifndef __urlhist_h__
#define __urlhist_h__

/* Forward declarations */

#ifndef __IEnumSTATURL_FWD_DEFINED__
#define __IEnumSTATURL_FWD_DEFINED__
typedef interface IEnumSTATURL IEnumSTATURL;
#endif

#ifndef __IUrlHistoryStg_FWD_DEFINED__
#define __IUrlHistoryStg_FWD_DEFINED__
typedef interface IUrlHistoryStg IUrlHistoryStg;
#endif

#ifndef __IUrlHistoryStg2_FWD_DEFINED__
#define __IUrlHistoryStg2_FWD_DEFINED__
typedef interface IUrlHistoryStg2 IUrlHistoryStg2;
#endif

#ifndef __IUrlHistoryNotify_FWD_DEFINED__
#define __IUrlHistoryNotify_FWD_DEFINED__
typedef interface IUrlHistoryNotify IUrlHistoryNotify;
#endif

/* Headers for imported files */

#include <objidl.h>
#include <oleidl.h>
#include <oaidl.h>
#include <docobj.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STATURL_QUERYFLAG_ISCACHED  0x010000
#define STATURL_QUERYFLAG_NOURL     0x020000
#define STATURL_QUERYFLAG_NOTITLE   0x040000
#define STATURL_QUERYFLAG_TOPLEVEL  0x080000
#define STATURLFLAG_ISCACHED    0x0001
#define STATURLFLAG_ISTOPLEVEL  0x0002
typedef enum _ADDURL_FLAG {
    ADDURL_FIRST = 0,
    ADDURL_ADDTOHISTORYANDCACHE = 0,
    ADDURL_ADDTOCACHE = 1,
    ADDURL_Max = 0x7fffffff
} ADDURL_FLAG;
/*****************************************************************************
 * IEnumSTATURL interface
 */
#ifndef __IEnumSTATURL_INTERFACE_DEFINED__
#define __IEnumSTATURL_INTERFACE_DEFINED__

typedef IEnumSTATURL *LPENUMSTATURL;
typedef struct _STATURL {
    DWORD cbSize;
    LPWSTR pwcsUrl;
    LPWSTR pwcsTitle;
    FILETIME ftLastVisited;
    FILETIME ftLastUpdated;
    FILETIME ftExpires;
    DWORD dwFlags;
} STATURL;
typedef struct _STATURL *LPSTATURL;
DEFINE_GUID(IID_IEnumSTATURL, 0x3c374a42, 0xbae4, 0x11cf, 0xbf,0x7d, 0x00,0xaa,0x00,0x69,0x46,0xee);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("3c374a42-bae4-11cf-bf7d-00aa006946ee")
IEnumSTATURL : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Next(
        ULONG celt,
        LPSTATURL rgelt,
        ULONG *pceltFetched) = 0;

    virtual HRESULT STDMETHODCALLTYPE Skip(
        ULONG celt) = 0;

    virtual HRESULT STDMETHODCALLTYPE Reset(
        ) = 0;

    virtual HRESULT STDMETHODCALLTYPE Clone(
        IEnumSTATURL **ppenum) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetFilter(
        LPCOLESTR poszFilter,
        DWORD dwFlags) = 0;

};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(IEnumSTATURL, 0x3c374a42, 0xbae4, 0x11cf, 0xbf,0x7d, 0x00,0xaa,0x00,0x69,0x46,0xee)
#endif
#else
typedef struct IEnumSTATURLVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IEnumSTATURL* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IEnumSTATURL* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IEnumSTATURL* This);

    /*** IEnumSTATURL methods ***/
    HRESULT (STDMETHODCALLTYPE *Next)(
        IEnumSTATURL* This,
        ULONG celt,
        LPSTATURL rgelt,
        ULONG *pceltFetched);

    HRESULT (STDMETHODCALLTYPE *Skip)(
        IEnumSTATURL* This,
        ULONG celt);

    HRESULT (STDMETHODCALLTYPE *Reset)(
        IEnumSTATURL* This);

    HRESULT (STDMETHODCALLTYPE *Clone)(
        IEnumSTATURL* This,
        IEnumSTATURL **ppenum);

    HRESULT (STDMETHODCALLTYPE *SetFilter)(
        IEnumSTATURL* This,
        LPCOLESTR poszFilter,
        DWORD dwFlags);

    END_INTERFACE
} IEnumSTATURLVtbl;
interface IEnumSTATURL {
    CONST_VTBL IEnumSTATURLVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define IEnumSTATURL_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IEnumSTATURL_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IEnumSTATURL_Release(This) (This)->lpVtbl->Release(This)
/*** IEnumSTATURL methods ***/
#define IEnumSTATURL_Next(This,celt,rgelt,pceltFetched) (This)->lpVtbl->Next(This,celt,rgelt,pceltFetched)
#define IEnumSTATURL_Skip(This,celt) (This)->lpVtbl->Skip(This,celt)
#define IEnumSTATURL_Reset(This) (This)->lpVtbl->Reset(This)
#define IEnumSTATURL_Clone(This,ppenum) (This)->lpVtbl->Clone(This,ppenum)
#define IEnumSTATURL_SetFilter(This,poszFilter,dwFlags) (This)->lpVtbl->SetFilter(This,poszFilter,dwFlags)
#else
/*** IUnknown methods ***/
static FORCEINLINE HRESULT IEnumSTATURL_QueryInterface(IEnumSTATURL* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static FORCEINLINE ULONG IEnumSTATURL_AddRef(IEnumSTATURL* This) {
    return This->lpVtbl->AddRef(This);
}
static FORCEINLINE ULONG IEnumSTATURL_Release(IEnumSTATURL* This) {
    return This->lpVtbl->Release(This);
}
/*** IEnumSTATURL methods ***/
static FORCEINLINE HRESULT IEnumSTATURL_Next(IEnumSTATURL* This,ULONG celt,LPSTATURL rgelt,ULONG *pceltFetched) {
    return This->lpVtbl->Next(This,celt,rgelt,pceltFetched);
}
static FORCEINLINE HRESULT IEnumSTATURL_Skip(IEnumSTATURL* This,ULONG celt) {
    return This->lpVtbl->Skip(This,celt);
}
static FORCEINLINE HRESULT IEnumSTATURL_Reset(IEnumSTATURL* This) {
    return This->lpVtbl->Reset(This);
}
static FORCEINLINE HRESULT IEnumSTATURL_Clone(IEnumSTATURL* This,IEnumSTATURL **ppenum) {
    return This->lpVtbl->Clone(This,ppenum);
}
static FORCEINLINE HRESULT IEnumSTATURL_SetFilter(IEnumSTATURL* This,LPCOLESTR poszFilter,DWORD dwFlags) {
    return This->lpVtbl->SetFilter(This,poszFilter,dwFlags);
}
#endif
#endif

#endif

HRESULT STDMETHODCALLTYPE IEnumSTATURL_Next_Proxy(
    IEnumSTATURL* This,
    ULONG celt,
    LPSTATURL rgelt,
    ULONG *pceltFetched);
void __RPC_STUB IEnumSTATURL_Next_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumSTATURL_Skip_Proxy(
    IEnumSTATURL* This,
    ULONG celt);
void __RPC_STUB IEnumSTATURL_Skip_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumSTATURL_Reset_Proxy(
    IEnumSTATURL* This);
void __RPC_STUB IEnumSTATURL_Reset_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumSTATURL_Clone_Proxy(
    IEnumSTATURL* This,
    IEnumSTATURL **ppenum);
void __RPC_STUB IEnumSTATURL_Clone_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IEnumSTATURL_SetFilter_Proxy(
    IEnumSTATURL* This,
    LPCOLESTR poszFilter,
    DWORD dwFlags);
void __RPC_STUB IEnumSTATURL_SetFilter_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __IEnumSTATURL_INTERFACE_DEFINED__ */

/*****************************************************************************
 * IUrlHistoryStg interface
 */
#ifndef __IUrlHistoryStg_INTERFACE_DEFINED__
#define __IUrlHistoryStg_INTERFACE_DEFINED__

typedef IUrlHistoryStg *LPURLHISTORYSTG;
DEFINE_GUID(IID_IUrlHistoryStg, 0x3c374a41, 0xbae4, 0x11cf, 0xbf,0x7d, 0x00,0xaa,0x00,0x69,0x46,0xee);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("3c374a41-bae4-11cf-bf7d-00aa006946ee")
IUrlHistoryStg : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE AddUrl(
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE DeleteUrl(
        LPCOLESTR pocsUrl,
        DWORD dwFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE QueryUrl(
        LPCOLESTR pocsUrl,
        DWORD dwFlags,
        LPSTATURL lpSTATURL) = 0;

    virtual HRESULT STDMETHODCALLTYPE BindToObject(
        LPCOLESTR pocsUrl,
        REFIID riid,
        void **ppvOut) = 0;

    virtual HRESULT STDMETHODCALLTYPE EnumUrls(
        IEnumSTATURL **ppEnum) = 0;

};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(IUrlHistoryStg, 0x3c374a41, 0xbae4, 0x11cf, 0xbf,0x7d, 0x00,0xaa,0x00,0x69,0x46,0xee)
#endif
#else
typedef struct IUrlHistoryStgVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IUrlHistoryStg* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IUrlHistoryStg* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IUrlHistoryStg* This);

    /*** IUrlHistoryStg methods ***/
    HRESULT (STDMETHODCALLTYPE *AddUrl)(
        IUrlHistoryStg* This,
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags);

    HRESULT (STDMETHODCALLTYPE *DeleteUrl)(
        IUrlHistoryStg* This,
        LPCOLESTR pocsUrl,
        DWORD dwFlags);

    HRESULT (STDMETHODCALLTYPE *QueryUrl)(
        IUrlHistoryStg* This,
        LPCOLESTR pocsUrl,
        DWORD dwFlags,
        LPSTATURL lpSTATURL);

    HRESULT (STDMETHODCALLTYPE *BindToObject)(
        IUrlHistoryStg* This,
        LPCOLESTR pocsUrl,
        REFIID riid,
        void **ppvOut);

    HRESULT (STDMETHODCALLTYPE *EnumUrls)(
        IUrlHistoryStg* This,
        IEnumSTATURL **ppEnum);

    END_INTERFACE
} IUrlHistoryStgVtbl;
interface IUrlHistoryStg {
    CONST_VTBL IUrlHistoryStgVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define IUrlHistoryStg_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IUrlHistoryStg_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IUrlHistoryStg_Release(This) (This)->lpVtbl->Release(This)
/*** IUrlHistoryStg methods ***/
#define IUrlHistoryStg_AddUrl(This,pocsUrl,pocsTitle,dwFlags) (This)->lpVtbl->AddUrl(This,pocsUrl,pocsTitle,dwFlags)
#define IUrlHistoryStg_DeleteUrl(This,pocsUrl,dwFlags) (This)->lpVtbl->DeleteUrl(This,pocsUrl,dwFlags)
#define IUrlHistoryStg_QueryUrl(This,pocsUrl,dwFlags,lpSTATURL) (This)->lpVtbl->QueryUrl(This,pocsUrl,dwFlags,lpSTATURL)
#define IUrlHistoryStg_BindToObject(This,pocsUrl,riid,ppvOut) (This)->lpVtbl->BindToObject(This,pocsUrl,riid,ppvOut)
#define IUrlHistoryStg_EnumUrls(This,ppEnum) (This)->lpVtbl->EnumUrls(This,ppEnum)
#else
/*** IUnknown methods ***/
static FORCEINLINE HRESULT IUrlHistoryStg_QueryInterface(IUrlHistoryStg* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static FORCEINLINE ULONG IUrlHistoryStg_AddRef(IUrlHistoryStg* This) {
    return This->lpVtbl->AddRef(This);
}
static FORCEINLINE ULONG IUrlHistoryStg_Release(IUrlHistoryStg* This) {
    return This->lpVtbl->Release(This);
}
/*** IUrlHistoryStg methods ***/
static FORCEINLINE HRESULT IUrlHistoryStg_AddUrl(IUrlHistoryStg* This,LPCOLESTR pocsUrl,LPCOLESTR pocsTitle,DWORD dwFlags) {
    return This->lpVtbl->AddUrl(This,pocsUrl,pocsTitle,dwFlags);
}
static FORCEINLINE HRESULT IUrlHistoryStg_DeleteUrl(IUrlHistoryStg* This,LPCOLESTR pocsUrl,DWORD dwFlags) {
    return This->lpVtbl->DeleteUrl(This,pocsUrl,dwFlags);
}
static FORCEINLINE HRESULT IUrlHistoryStg_QueryUrl(IUrlHistoryStg* This,LPCOLESTR pocsUrl,DWORD dwFlags,LPSTATURL lpSTATURL) {
    return This->lpVtbl->QueryUrl(This,pocsUrl,dwFlags,lpSTATURL);
}
static FORCEINLINE HRESULT IUrlHistoryStg_BindToObject(IUrlHistoryStg* This,LPCOLESTR pocsUrl,REFIID riid,void **ppvOut) {
    return This->lpVtbl->BindToObject(This,pocsUrl,riid,ppvOut);
}
static FORCEINLINE HRESULT IUrlHistoryStg_EnumUrls(IUrlHistoryStg* This,IEnumSTATURL **ppEnum) {
    return This->lpVtbl->EnumUrls(This,ppEnum);
}
#endif
#endif

#endif

HRESULT STDMETHODCALLTYPE IUrlHistoryStg_AddUrl_Proxy(
    IUrlHistoryStg* This,
    LPCOLESTR pocsUrl,
    LPCOLESTR pocsTitle,
    DWORD dwFlags);
void __RPC_STUB IUrlHistoryStg_AddUrl_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IUrlHistoryStg_DeleteUrl_Proxy(
    IUrlHistoryStg* This,
    LPCOLESTR pocsUrl,
    DWORD dwFlags);
void __RPC_STUB IUrlHistoryStg_DeleteUrl_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IUrlHistoryStg_QueryUrl_Proxy(
    IUrlHistoryStg* This,
    LPCOLESTR pocsUrl,
    DWORD dwFlags,
    LPSTATURL lpSTATURL);
void __RPC_STUB IUrlHistoryStg_QueryUrl_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IUrlHistoryStg_BindToObject_Proxy(
    IUrlHistoryStg* This,
    LPCOLESTR pocsUrl,
    REFIID riid,
    void **ppvOut);
void __RPC_STUB IUrlHistoryStg_BindToObject_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IUrlHistoryStg_EnumUrls_Proxy(
    IUrlHistoryStg* This,
    IEnumSTATURL **ppEnum);
void __RPC_STUB IUrlHistoryStg_EnumUrls_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __IUrlHistoryStg_INTERFACE_DEFINED__ */

/*****************************************************************************
 * IUrlHistoryStg2 interface
 */
#ifndef __IUrlHistoryStg2_INTERFACE_DEFINED__
#define __IUrlHistoryStg2_INTERFACE_DEFINED__

typedef IUrlHistoryStg2 *LPURLHISTORYSTG2;
DEFINE_GUID(IID_IUrlHistoryStg2, 0xafa0dc11, 0xc313, 0x11d0, 0x83,0x1a, 0x00,0xc0,0x4f,0xd5,0xae,0x38);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("afa0dc11-c313-11d0-831a-00c04fd5ae38")
IUrlHistoryStg2 : public IUrlHistoryStg
{
    virtual HRESULT STDMETHODCALLTYPE AddUrlAndNotify(
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags,
        WINBOOL fWriteHistory,
        IOleCommandTarget *poctNotify,
        IUnknown *punkISFolder) = 0;

    virtual HRESULT STDMETHODCALLTYPE ClearHistory(
        ) = 0;

};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(IUrlHistoryStg2, 0xafa0dc11, 0xc313, 0x11d0, 0x83,0x1a, 0x00,0xc0,0x4f,0xd5,0xae,0x38)
#endif
#else
typedef struct IUrlHistoryStg2Vtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IUrlHistoryStg2* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IUrlHistoryStg2* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IUrlHistoryStg2* This);

    /*** IUrlHistoryStg methods ***/
    HRESULT (STDMETHODCALLTYPE *AddUrl)(
        IUrlHistoryStg2* This,
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags);

    HRESULT (STDMETHODCALLTYPE *DeleteUrl)(
        IUrlHistoryStg2* This,
        LPCOLESTR pocsUrl,
        DWORD dwFlags);

    HRESULT (STDMETHODCALLTYPE *QueryUrl)(
        IUrlHistoryStg2* This,
        LPCOLESTR pocsUrl,
        DWORD dwFlags,
        LPSTATURL lpSTATURL);

    HRESULT (STDMETHODCALLTYPE *BindToObject)(
        IUrlHistoryStg2* This,
        LPCOLESTR pocsUrl,
        REFIID riid,
        void **ppvOut);

    HRESULT (STDMETHODCALLTYPE *EnumUrls)(
        IUrlHistoryStg2* This,
        IEnumSTATURL **ppEnum);

    /*** IUrlHistoryStg2 methods ***/
    HRESULT (STDMETHODCALLTYPE *AddUrlAndNotify)(
        IUrlHistoryStg2* This,
        LPCOLESTR pocsUrl,
        LPCOLESTR pocsTitle,
        DWORD dwFlags,
        WINBOOL fWriteHistory,
        IOleCommandTarget *poctNotify,
        IUnknown *punkISFolder);

    HRESULT (STDMETHODCALLTYPE *ClearHistory)(
        IUrlHistoryStg2* This);

    END_INTERFACE
} IUrlHistoryStg2Vtbl;
interface IUrlHistoryStg2 {
    CONST_VTBL IUrlHistoryStg2Vtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define IUrlHistoryStg2_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IUrlHistoryStg2_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IUrlHistoryStg2_Release(This) (This)->lpVtbl->Release(This)
/*** IUrlHistoryStg methods ***/
#define IUrlHistoryStg2_AddUrl(This,pocsUrl,pocsTitle,dwFlags) (This)->lpVtbl->AddUrl(This,pocsUrl,pocsTitle,dwFlags)
#define IUrlHistoryStg2_DeleteUrl(This,pocsUrl,dwFlags) (This)->lpVtbl->DeleteUrl(This,pocsUrl,dwFlags)
#define IUrlHistoryStg2_QueryUrl(This,pocsUrl,dwFlags,lpSTATURL) (This)->lpVtbl->QueryUrl(This,pocsUrl,dwFlags,lpSTATURL)
#define IUrlHistoryStg2_BindToObject(This,pocsUrl,riid,ppvOut) (This)->lpVtbl->BindToObject(This,pocsUrl,riid,ppvOut)
#define IUrlHistoryStg2_EnumUrls(This,ppEnum) (This)->lpVtbl->EnumUrls(This,ppEnum)
/*** IUrlHistoryStg2 methods ***/
#define IUrlHistoryStg2_AddUrlAndNotify(This,pocsUrl,pocsTitle,dwFlags,fWriteHistory,poctNotify,punkISFolder) (This)->lpVtbl->AddUrlAndNotify(This,pocsUrl,pocsTitle,dwFlags,fWriteHistory,poctNotify,punkISFolder)
#define IUrlHistoryStg2_ClearHistory(This) (This)->lpVtbl->ClearHistory(This)
#else
/*** IUnknown methods ***/
static FORCEINLINE HRESULT IUrlHistoryStg2_QueryInterface(IUrlHistoryStg2* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static FORCEINLINE ULONG IUrlHistoryStg2_AddRef(IUrlHistoryStg2* This) {
    return This->lpVtbl->AddRef(This);
}
static FORCEINLINE ULONG IUrlHistoryStg2_Release(IUrlHistoryStg2* This) {
    return This->lpVtbl->Release(This);
}
/*** IUrlHistoryStg methods ***/
static FORCEINLINE HRESULT IUrlHistoryStg2_AddUrl(IUrlHistoryStg2* This,LPCOLESTR pocsUrl,LPCOLESTR pocsTitle,DWORD dwFlags) {
    return This->lpVtbl->AddUrl(This,pocsUrl,pocsTitle,dwFlags);
}
static FORCEINLINE HRESULT IUrlHistoryStg2_DeleteUrl(IUrlHistoryStg2* This,LPCOLESTR pocsUrl,DWORD dwFlags) {
    return This->lpVtbl->DeleteUrl(This,pocsUrl,dwFlags);
}
static FORCEINLINE HRESULT IUrlHistoryStg2_QueryUrl(IUrlHistoryStg2* This,LPCOLESTR pocsUrl,DWORD dwFlags,LPSTATURL lpSTATURL) {
    return This->lpVtbl->QueryUrl(This,pocsUrl,dwFlags,lpSTATURL);
}
static FORCEINLINE HRESULT IUrlHistoryStg2_BindToObject(IUrlHistoryStg2* This,LPCOLESTR pocsUrl,REFIID riid,void **ppvOut) {
    return This->lpVtbl->BindToObject(This,pocsUrl,riid,ppvOut);
}
static FORCEINLINE HRESULT IUrlHistoryStg2_EnumUrls(IUrlHistoryStg2* This,IEnumSTATURL **ppEnum) {
    return This->lpVtbl->EnumUrls(This,ppEnum);
}
/*** IUrlHistoryStg2 methods ***/
static FORCEINLINE HRESULT IUrlHistoryStg2_AddUrlAndNotify(IUrlHistoryStg2* This,LPCOLESTR pocsUrl,LPCOLESTR pocsTitle,DWORD dwFlags,WINBOOL fWriteHistory,IOleCommandTarget *poctNotify,IUnknown *punkISFolder) {
    return This->lpVtbl->AddUrlAndNotify(This,pocsUrl,pocsTitle,dwFlags,fWriteHistory,poctNotify,punkISFolder);
}
static FORCEINLINE HRESULT IUrlHistoryStg2_ClearHistory(IUrlHistoryStg2* This) {
    return This->lpVtbl->ClearHistory(This);
}
#endif
#endif

#endif

HRESULT STDMETHODCALLTYPE IUrlHistoryStg2_AddUrlAndNotify_Proxy(
    IUrlHistoryStg2* This,
    LPCOLESTR pocsUrl,
    LPCOLESTR pocsTitle,
    DWORD dwFlags,
    WINBOOL fWriteHistory,
    IOleCommandTarget *poctNotify,
    IUnknown *punkISFolder);
void __RPC_STUB IUrlHistoryStg2_AddUrlAndNotify_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);
HRESULT STDMETHODCALLTYPE IUrlHistoryStg2_ClearHistory_Proxy(
    IUrlHistoryStg2* This);
void __RPC_STUB IUrlHistoryStg2_ClearHistory_Stub(
    IRpcStubBuffer* This,
    IRpcChannelBuffer* pRpcChannelBuffer,
    PRPC_MESSAGE pRpcMessage,
    DWORD* pdwStubPhase);

#endif  /* __IUrlHistoryStg2_INTERFACE_DEFINED__ */

/*****************************************************************************
 * IUrlHistoryNotify interface
 */
#ifndef __IUrlHistoryNotify_INTERFACE_DEFINED__
#define __IUrlHistoryNotify_INTERFACE_DEFINED__

typedef IUrlHistoryNotify *LPURLHISTORYNOTIFY;
DEFINE_GUID(IID_IUrlHistoryNotify, 0xbc40bec1, 0xc493, 0x11d0, 0x83,0x1b, 0x00,0xc0,0x4f,0xd5,0xae,0x38);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("bc40bec1-c493-11d0-831b-00c04fd5ae38")
IUrlHistoryNotify : public IOleCommandTarget
{
};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(IUrlHistoryNotify, 0xbc40bec1, 0xc493, 0x11d0, 0x83,0x1b, 0x00,0xc0,0x4f,0xd5,0xae,0x38)
#endif
#else
typedef struct IUrlHistoryNotifyVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IUrlHistoryNotify* This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IUrlHistoryNotify* This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IUrlHistoryNotify* This);

    /*** IOleCommandTarget methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryStatus)(
        IUrlHistoryNotify* This,
        const GUID *pguidCmdGroup,
        ULONG cCmds,
        OLECMD prgCmds[],
        OLECMDTEXT *pCmdText);

    HRESULT (STDMETHODCALLTYPE *Exec)(
        IUrlHistoryNotify* This,
        const GUID *pguidCmdGroup,
        DWORD nCmdID,
        DWORD nCmdexecopt,
        VARIANT *pvaIn,
        VARIANT *pvaOut);

    END_INTERFACE
} IUrlHistoryNotifyVtbl;
interface IUrlHistoryNotify {
    CONST_VTBL IUrlHistoryNotifyVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define IUrlHistoryNotify_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define IUrlHistoryNotify_AddRef(This) (This)->lpVtbl->AddRef(This)
#define IUrlHistoryNotify_Release(This) (This)->lpVtbl->Release(This)
/*** IOleCommandTarget methods ***/
#define IUrlHistoryNotify_QueryStatus(This,pguidCmdGroup,cCmds,prgCmds,pCmdText) (This)->lpVtbl->QueryStatus(This,pguidCmdGroup,cCmds,prgCmds,pCmdText)
#define IUrlHistoryNotify_Exec(This,pguidCmdGroup,nCmdID,nCmdexecopt,pvaIn,pvaOut) (This)->lpVtbl->Exec(This,pguidCmdGroup,nCmdID,nCmdexecopt,pvaIn,pvaOut)
#else
/*** IUnknown methods ***/
static FORCEINLINE HRESULT IUrlHistoryNotify_QueryInterface(IUrlHistoryNotify* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static FORCEINLINE ULONG IUrlHistoryNotify_AddRef(IUrlHistoryNotify* This) {
    return This->lpVtbl->AddRef(This);
}
static FORCEINLINE ULONG IUrlHistoryNotify_Release(IUrlHistoryNotify* This) {
    return This->lpVtbl->Release(This);
}
/*** IOleCommandTarget methods ***/
static FORCEINLINE HRESULT IUrlHistoryNotify_QueryStatus(IUrlHistoryNotify* This,const GUID *pguidCmdGroup,ULONG cCmds,OLECMD prgCmds[],OLECMDTEXT *pCmdText) {
    return This->lpVtbl->QueryStatus(This,pguidCmdGroup,cCmds,prgCmds,pCmdText);
}
static FORCEINLINE HRESULT IUrlHistoryNotify_Exec(IUrlHistoryNotify* This,const GUID *pguidCmdGroup,DWORD nCmdID,DWORD nCmdexecopt,VARIANT *pvaIn,VARIANT *pvaOut) {
    return This->lpVtbl->Exec(This,pguidCmdGroup,nCmdID,nCmdexecopt,pvaIn,pvaOut);
}
#endif
#endif

#endif


#endif  /* __IUrlHistoryNotify_INTERFACE_DEFINED__ */

/* Begin additional prototypes for all interfaces */


/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __urlhist_h__ */
