#ifndef _INC_WSDATTACHMENT
#define _INC_WSDATTACHMENT
#if (_WIN32_WINNT >= 0x0600)

#undef  INTERFACE
#define INTERFACE IWSDAttachment
DECLARE_INTERFACE_(IWSDAttachment,IUnknown)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IWSDAttachment methods */

    END_INTERFACE
};
#ifdef COBJMACROS
#define IWSDAttachment_QueryInterface(This,riid,ppvObject) (This)->pVtbl->QueryInterface(This,riid,ppvObject)
#define IWSDAttachment_AddRef(This) (This)->pVtbl->AddRef(This)
#define IWSDAttachment_Release(This) (This)->pVtbl->Release(This)
#endif /*COBJMACROS*/

#undef  INTERFACE
#define INTERFACE IWSDInboundAttachment
DECLARE_INTERFACE_(IWSDInboundAttachment,IWSDAttachment)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IWSDAttachment methods */

    /* IWSDInboundAttachment methods */
    STDMETHOD_(HRESULT,Read)(THIS_ BYTE *pBuffer,DWORD dwBytesToRead,LPDWORD pdwNumberofBytesRead) PURE;
    STDMETHOD_(HRESULT,Close)(THIS) PURE;

    END_INTERFACE
};
#ifdef COBJMACROS
#define IWSDInboundAttachment_QueryInterface(This,riid,ppvObject) (This)->pVtbl->QueryInterface(This,riid,ppvObject)
#define IWSDInboundAttachment_AddRef(This) (This)->pVtbl->AddRef(This)
#define IWSDInboundAttachment_Release(This) (This)->pVtbl->Release(This)
#define IWSDInboundAttachment_Read(This,pBuffer,dwBytesToRead,pdwNumberofBytesRead) (This)->lpVtbl->Read(This,pBuffer,dwBytesToRead,pdwNumberofBytesRead)
#define IWSDInboundAttachment_Close() (This)->lpVtbl->Close(This)
#endif /*COBJMACROS*/

#undef  INTERFACE
#define INTERFACE IWSDOutboundAttachment
DECLARE_INTERFACE_(IWSDOutboundAttachment,IWSDAttachment)
{
    BEGIN_INTERFACE

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void **ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IWSDAttachment methods */

    /* IWSDOutboundAttachment methods */
    STDMETHOD_(HRESULT,Write)(THIS_ const BYTE *pBuffer,DWORD dwBytesToWrite,LPDWORD pdwNumberofBytesWritten) PURE;
    STDMETHOD_(HRESULT,Close)(THIS) PURE;
    STDMETHOD_(HRESULT,Abort)(THIS) PURE;

    END_INTERFACE
};
#ifdef COBJMACROS
#define IWSDOutboundAttachment_QueryInterface(This,riid,ppvObject) (This)->pVtbl->QueryInterface(This,riid,ppvObject)
#define IWSDOutboundAttachment_AddRef(This) (This)->pVtbl->AddRef(This)
#define IWSDOutboundAttachment_Release(This) (This)->pVtbl->Release(This)
#define IWSDOutboundAttachment_Write(This,pBuffer,dwBytesToWrite,pdwNumberofBytesWritten) (This)->lpVtbl->Write(This,pBuffer,dwBytesToWrite,pdwNumberofBytesWritten)
#define IWSDOutboundAttachment_Close() (This)->lpVtbl->Close(This)
#define IWSDOutboundAttachment_Abort() (This)->lpVtbl->Abort(This)
#endif /*COBJMACROS*/

HRESULT WSDCreateOutboundAttachment(
  IWSDOutboundAttachment **attachmentOut
);

#endif /*(_WIN32_WINNT >= 0x0600)*/
#endif /*_INC_WSDATTACHMENT*/
