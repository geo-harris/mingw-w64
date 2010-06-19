/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _WINCRED_H_
#define _WINCRED_H_
#include <_mingw.h>

#include <_mingw_unicode.h>

#define WINADVAPI DECLSPEC_IMPORT

#ifndef CREDUIAPI
#ifndef _CREDUI_
#define CREDUIAPI DECLSPEC_IMPORT
#else
#define CREDUIAPI
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SECHANDLE_DEFINED__
#define __SECHANDLE_DEFINED__

  typedef struct _SecHandle {
    ULONG_PTR dwLower;
    ULONG_PTR dwUpper;
  } SecHandle,*PSecHandle;
#endif

  typedef PSecHandle PCtxtHandle;

#ifndef _WINBASE_
#ifndef _FILETIME_
#define _FILETIME_

  typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
  } FILETIME;

  typedef struct _FILETIME *PFILETIME;
  typedef struct _FILETIME *LPFILETIME;
#endif
#endif

#ifndef _NTDEF_
  typedef LONG NTSTATUS,*PNTSTATUS;
#endif

#define STATUS_LOGON_FAILURE ((NTSTATUS)0xC000006DL)
#define STATUS_WRONG_PASSWORD ((NTSTATUS)0xC000006AL)
#define STATUS_PASSWORD_EXPIRED ((NTSTATUS)0xC0000071L)
#define STATUS_PASSWORD_MUST_CHANGE ((NTSTATUS)0xC0000224L)
#define STATUS_ACCESS_DENIED ((NTSTATUS)0xC0000022L)
#define STATUS_DOWNGRADE_DETECTED ((NTSTATUS)0xC0000388L)
#define STATUS_AUTHENTICATION_FIREWALL_FAILED ((NTSTATUS)0xC0000413L)
#define STATUS_ACCOUNT_DISABLED ((NTSTATUS)0xC0000072L)
#define STATUS_ACCOUNT_RESTRICTION ((NTSTATUS)0xC000006EL)
#define STATUS_ACCOUNT_LOCKED_OUT ((NTSTATUS)0xC0000234L)
#define STATUS_ACCOUNT_EXPIRED ((NTSTATUS)0xC0000193L)
#define STATUS_LOGON_TYPE_NOT_GRANTED ((NTSTATUS)0xC000015BL)

#define NERR_BASE 2100
#define NERR_PasswordExpired (NERR_BASE+142)

#define CREDUIP_IS_USER_PASSWORD_ERROR(_Status) ((_Status)==ERROR_LOGON_FAILURE || (_Status)==HRESULT_FROM_WIN32(ERROR_LOGON_FAILURE) || (_Status)==STATUS_LOGON_FAILURE || (_Status)==HRESULT_FROM_NT(STATUS_LOGON_FAILURE) || (_Status)==ERROR_ACCESS_DENIED || (_Status)==HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED) || (_Status)==STATUS_ACCESS_DENIED || (_Status)==HRESULT_FROM_NT(STATUS_ACCESS_DENIED) || (_Status)==ERROR_INVALID_PASSWORD || (_Status)==HRESULT_FROM_WIN32(ERROR_INVALID_PASSWORD) || (_Status)==STATUS_WRONG_PASSWORD || (_Status)==HRESULT_FROM_NT(STATUS_WRONG_PASSWORD) || (_Status)==SEC_E_NO_CREDENTIALS || (_Status)==SEC_E_LOGON_DENIED)
#define CREDUIP_IS_DOWNGRADE_ERROR(_Status) ((_Status)==ERROR_DOWNGRADE_DETECTED || (_Status)==HRESULT_FROM_WIN32(ERROR_DOWNGRADE_DETECTED) || (_Status)==STATUS_DOWNGRADE_DETECTED || (_Status)==HRESULT_FROM_NT(STATUS_DOWNGRADE_DETECTED))
#define CREDUIP_IS_EXPIRED_ERROR(_Status) ((_Status)==ERROR_PASSWORD_EXPIRED || (_Status)==HRESULT_FROM_WIN32(ERROR_PASSWORD_EXPIRED) || (_Status)==STATUS_PASSWORD_EXPIRED || (_Status)==HRESULT_FROM_NT(STATUS_PASSWORD_EXPIRED) || (_Status)==ERROR_PASSWORD_MUST_CHANGE || (_Status)==HRESULT_FROM_WIN32(ERROR_PASSWORD_MUST_CHANGE) || (_Status)==STATUS_PASSWORD_MUST_CHANGE || (_Status)==HRESULT_FROM_NT(STATUS_PASSWORD_MUST_CHANGE) || (_Status)==NERR_PasswordExpired || (_Status)==HRESULT_FROM_WIN32(NERR_PasswordExpired))
#define CREDUI_IS_AUTHENTICATION_ERROR(_Status) (CREDUIP_IS_USER_PASSWORD_ERROR(_Status) || CREDUIP_IS_DOWNGRADE_ERROR(_Status) || CREDUIP_IS_EXPIRED_ERROR(_Status))
#define CREDUI_NO_PROMPT_AUTHENTICATION_ERROR(_Status) ((_Status)==ERROR_AUTHENTICATION_FIREWALL_FAILED || (_Status)==HRESULT_FROM_WIN32(ERROR_AUTHENTICATION_FIREWALL_FAILED) || (_Status)==STATUS_AUTHENTICATION_FIREWALL_FAILED || (_Status)==HRESULT_FROM_NT(STATUS_AUTHENTICATION_FIREWALL_FAILED) || (_Status)==ERROR_ACCOUNT_DISABLED || (_Status)==HRESULT_FROM_WIN32(ERROR_ACCOUNT_DISABLED) || (_Status)==STATUS_ACCOUNT_DISABLED || (_Status)==HRESULT_FROM_NT(STATUS_ACCOUNT_DISABLED) || (_Status)==ERROR_ACCOUNT_RESTRICTION || (_Status)==HRESULT_FROM_WIN32(ERROR_ACCOUNT_RESTRICTION) || (_Status)==STATUS_ACCOUNT_RESTRICTION || (_Status)==HRESULT_FROM_NT(STATUS_ACCOUNT_RESTRICTION) || (_Status)==ERROR_ACCOUNT_LOCKED_OUT || (_Status)==HRESULT_FROM_WIN32(ERROR_ACCOUNT_LOCKED_OUT) || (_Status)==STATUS_ACCOUNT_LOCKED_OUT || (_Status)==HRESULT_FROM_NT(STATUS_ACCOUNT_LOCKED_OUT) || (_Status)==ERROR_ACCOUNT_EXPIRED || (_Status)==HRESULT_FROM_WIN32(ERROR_ACCOUNT_EXPIRED) || (_Status)==STATUS_ACCOUNT_EXPIRED || (_Status)==HRESULT_FROM_NT(STATUS_ACCOUNT_EXPIRED) || (_Status)==ERROR_LOGON_TYPE_NOT_GRANTED || (_Status)==HRESULT_FROM_WIN32(ERROR_LOGON_TYPE_NOT_GRANTED) || (_Status)==STATUS_LOGON_TYPE_NOT_GRANTED || (_Status)==HRESULT_FROM_NT(STATUS_LOGON_TYPE_NOT_GRANTED))

#define CRED_MAX_STRING_LENGTH 256
#define CRED_MAX_USERNAME_LENGTH (256+1+256)
#define CRED_MAX_GENERIC_TARGET_NAME_LENGTH 32767
#define CRED_MAX_DOMAIN_TARGET_NAME_LENGTH (256+1+80)
#define CRED_MAX_VALUE_SIZE 256
#define CRED_MAX_ATTRIBUTES 64

  typedef struct _CREDENTIAL_ATTRIBUTEA {
    LPSTR Keyword;
    DWORD Flags;
    DWORD ValueSize;
    LPBYTE Value;
  } CREDENTIAL_ATTRIBUTEA,*PCREDENTIAL_ATTRIBUTEA;

  typedef struct _CREDENTIAL_ATTRIBUTEW {
    LPWSTR Keyword;
    DWORD Flags;
    DWORD ValueSize;
    LPBYTE Value;
  } CREDENTIAL_ATTRIBUTEW,*PCREDENTIAL_ATTRIBUTEW;

  __MINGW_TYPEDEF_AW(CREDENTIAL_ATTRIBUTE)
  __MINGW_TYPEDEF_AW(PCREDENTIAL_ATTRIBUTE)

#define CRED_SESSION_WILDCARD_NAME_W L"*Session"
#define CRED_SESSION_WILDCARD_NAME_A "*Session"
#define CRED_SESSION_WILDCARD_NAME_LENGTH (sizeof(CRED_SESSION_WILDCARD_NAME_A)-1)

#define CRED_SESSION_WILDCARD_NAME __MINGW_NAME_UAW(CRED_SESSION_WILDCARD_NAME)

#define CRED_FLAGS_PASSWORD_FOR_CERT 0x0001
#define CRED_FLAGS_PROMPT_NOW 0x0002
#define CRED_FLAGS_USERNAME_TARGET 0x0004
#define CRED_FLAGS_OWF_CRED_BLOB 0x0008
#define CRED_FLAGS_VALID_FLAGS 0x000F

#define CRED_TYPE_GENERIC 1
#define CRED_TYPE_DOMAIN_PASSWORD 2
#define CRED_TYPE_DOMAIN_CERTIFICATE 3
#define CRED_TYPE_DOMAIN_VISIBLE_PASSWORD 4
#define CRED_TYPE_MAXIMUM 5
#define CRED_TYPE_MAXIMUM_EX (CRED_TYPE_MAXIMUM+1000)

#define CRED_MAX_CREDENTIAL_BLOB_SIZE 512

#define CRED_PERSIST_NONE 0
#define CRED_PERSIST_SESSION 1
#define CRED_PERSIST_LOCAL_MACHINE 2
#define CRED_PERSIST_ENTERPRISE 3

  typedef struct _CREDENTIALA {
    DWORD Flags;
    DWORD Type;
    LPSTR TargetName;
    LPSTR Comment;
    FILETIME LastWritten;
    DWORD CredentialBlobSize;
    LPBYTE CredentialBlob;
    DWORD Persist;
    DWORD AttributeCount;
    PCREDENTIAL_ATTRIBUTEA Attributes;
    LPSTR TargetAlias;
    LPSTR UserName;
  } CREDENTIALA,*PCREDENTIALA;

  typedef struct _CREDENTIALW {
    DWORD Flags;
    DWORD Type;
    LPWSTR TargetName;
    LPWSTR Comment;
    FILETIME LastWritten;
    DWORD CredentialBlobSize;
    LPBYTE CredentialBlob;
    DWORD Persist;
    DWORD AttributeCount;
    PCREDENTIAL_ATTRIBUTEW Attributes;
    LPWSTR TargetAlias;
    LPWSTR UserName;
  } CREDENTIALW,*PCREDENTIALW;

  __MINGW_TYPEDEF_AW(CREDENTIAL)
  __MINGW_TYPEDEF_AW(PCREDENTIAL)

#define CRED_TI_SERVER_FORMAT_UNKNOWN 0x0001
#define CRED_TI_DOMAIN_FORMAT_UNKNOWN 0x0002
#define CRED_TI_ONLY_PASSWORD_REQUIRED 0x0004
#define CRED_TI_USERNAME_TARGET 0x0008
#define CRED_TI_CREATE_EXPLICIT_CRED 0x0010
#define CRED_TI_WORKGROUP_MEMBER 0x0020
#define CRED_TI_VALID_FLAGS 0x003F

  typedef struct _CREDENTIAL_TARGET_INFORMATIONA {
    LPSTR TargetName;
    LPSTR NetbiosServerName;
    LPSTR DnsServerName;
    LPSTR NetbiosDomainName;
    LPSTR DnsDomainName;
    LPSTR DnsTreeName;
    LPSTR PackageName;
    ULONG Flags;
    DWORD CredTypeCount;
    LPDWORD CredTypes;
  } CREDENTIAL_TARGET_INFORMATIONA,*PCREDENTIAL_TARGET_INFORMATIONA;

  typedef struct _CREDENTIAL_TARGET_INFORMATIONW {
    LPWSTR TargetName;
    LPWSTR NetbiosServerName;
    LPWSTR DnsServerName;
    LPWSTR NetbiosDomainName;
    LPWSTR DnsDomainName;
    LPWSTR DnsTreeName;
    LPWSTR PackageName;
    ULONG Flags;
    DWORD CredTypeCount;
    LPDWORD CredTypes;
  } CREDENTIAL_TARGET_INFORMATIONW,*PCREDENTIAL_TARGET_INFORMATIONW;

  __MINGW_TYPEDEF_AW(CREDENTIAL_TARGET_INFORMATION)
  __MINGW_TYPEDEF_AW(PCREDENTIAL_TARGET_INFORMATION)

#define CERT_HASH_LENGTH 20

  typedef struct _CERT_CREDENTIAL_INFO {
    ULONG cbSize;
    UCHAR rgbHashOfCert[CERT_HASH_LENGTH];
  } CERT_CREDENTIAL_INFO,*PCERT_CREDENTIAL_INFO;

  typedef struct _USERNAME_TARGET_CREDENTIAL_INFO {
    LPWSTR UserName;
  } USERNAME_TARGET_CREDENTIAL_INFO,*PUSERNAME_TARGET_CREDENTIAL_INFO;

  typedef enum _CRED_MARSHAL_TYPE {
    CertCredential = 1,UsernameTargetCredential
  } CRED_MARSHAL_TYPE,*PCRED_MARSHAL_TYPE;

  typedef struct _CREDUI_INFOA {
    DWORD cbSize;
    HWND hwndParent;
    PCSTR pszMessageText;
    PCSTR pszCaptionText;
    HBITMAP hbmBanner;
  } CREDUI_INFOA,*PCREDUI_INFOA;

  typedef struct _CREDUI_INFOW {
    DWORD cbSize;
    HWND hwndParent;
    PCWSTR pszMessageText;
    PCWSTR pszCaptionText;
    HBITMAP hbmBanner;
  } CREDUI_INFOW,*PCREDUI_INFOW;

  __MINGW_TYPEDEF_AW(CREDUI_INFO)
  __MINGW_TYPEDEF_AW(PCREDUI_INFO)

#define CREDUI_MAX_MESSAGE_LENGTH 32767
#define CREDUI_MAX_CAPTION_LENGTH 128
#define CREDUI_MAX_GENERIC_TARGET_LENGTH CRED_MAX_GENERIC_TARGET_NAME_LENGTH
#define CREDUI_MAX_DOMAIN_TARGET_LENGTH CRED_MAX_DOMAIN_TARGET_NAME_LENGTH
#define CREDUI_MAX_USERNAME_LENGTH CRED_MAX_USERNAME_LENGTH
#define CREDUI_MAX_PASSWORD_LENGTH (CRED_MAX_CREDENTIAL_BLOB_SIZE / 2)

#define CREDUI_FLAGS_INCORRECT_PASSWORD 0x00001
#define CREDUI_FLAGS_DO_NOT_PERSIST 0x00002
#define CREDUI_FLAGS_REQUEST_ADMINISTRATOR 0x00004
#define CREDUI_FLAGS_EXCLUDE_CERTIFICATES 0x00008
#define CREDUI_FLAGS_REQUIRE_CERTIFICATE 0x00010
#define CREDUI_FLAGS_SHOW_SAVE_CHECK_BOX 0x00040
#define CREDUI_FLAGS_ALWAYS_SHOW_UI 0x00080
#define CREDUI_FLAGS_REQUIRE_SMARTCARD 0x00100
#define CREDUI_FLAGS_PASSWORD_ONLY_OK 0x00200
#define CREDUI_FLAGS_VALIDATE_USERNAME 0x00400
#define CREDUI_FLAGS_COMPLETE_USERNAME 0x00800
#define CREDUI_FLAGS_PERSIST 0x01000
#define CREDUI_FLAGS_SERVER_CREDENTIAL 0x04000
#define CREDUI_FLAGS_EXPECT_CONFIRMATION 0x20000
#define CREDUI_FLAGS_GENERIC_CREDENTIALS 0x40000
#define CREDUI_FLAGS_USERNAME_TARGET_CREDENTIALS 0x80000
#define CREDUI_FLAGS_KEEP_USERNAME 0x100000

#define CREDUI_FLAGS_PROMPT_VALID (CREDUI_FLAGS_INCORRECT_PASSWORD | CREDUI_FLAGS_DO_NOT_PERSIST | CREDUI_FLAGS_REQUEST_ADMINISTRATOR | CREDUI_FLAGS_EXCLUDE_CERTIFICATES | CREDUI_FLAGS_REQUIRE_CERTIFICATE | CREDUI_FLAGS_SHOW_SAVE_CHECK_BOX | CREDUI_FLAGS_ALWAYS_SHOW_UI | CREDUI_FLAGS_REQUIRE_SMARTCARD | CREDUI_FLAGS_PASSWORD_ONLY_OK | CREDUI_FLAGS_VALIDATE_USERNAME | CREDUI_FLAGS_COMPLETE_USERNAME | CREDUI_FLAGS_PERSIST | CREDUI_FLAGS_SERVER_CREDENTIAL | CREDUI_FLAGS_EXPECT_CONFIRMATION | CREDUI_FLAGS_GENERIC_CREDENTIALS | CREDUI_FLAGS_USERNAME_TARGET_CREDENTIALS | CREDUI_FLAGS_KEEP_USERNAME)

#define CRED_PRESERVE_CREDENTIAL_BLOB 0x1

#define CredWrite __MINGW_NAME_AW(CredWrite)
#define CredRead __MINGW_NAME_AW(CredRead)
#define CredEnumerate __MINGW_NAME_AW(CredEnumerate)
#define CredWriteDomainCredentials __MINGW_NAME_AW(CredWriteDomainCredentials)
#define CredReadDomainCredentials __MINGW_NAME_AW(CredReadDomainCredentials)
#define CredDelete __MINGW_NAME_AW(CredDelete)
#define CredRename __MINGW_NAME_AW(CredRename)
#define CredGetTargetInfo __MINGW_NAME_AW(CredGetTargetInfo)
#define CredMarshalCredential __MINGW_NAME_AW(CredMarshalCredential)
#define CredUnmarshalCredential __MINGW_NAME_AW(CredUnmarshalCredential)
#define CredIsMarshaledCredential __MINGW_NAME_AW(CredIsMarshaledCredential)
#define CredUIPromptForCredentials __MINGW_NAME_AW(CredUIPromptForCredentials)
#define CredUIParseUserName __MINGW_NAME_AW(CredUIParseUserName)
#define CredUICmdLinePromptForCredentials __MINGW_NAME_AW(CredUICmdLinePromptForCredentials)
#define CredUIConfirmCredentials __MINGW_NAME_AW(CredUIConfirmCredentials)

  WINADVAPI WINBOOL WINAPI CredWriteW(PCREDENTIALW Credential,DWORD Flags);
  WINADVAPI WINBOOL WINAPI CredWriteA(PCREDENTIALA Credential,DWORD Flags);
  WINADVAPI WINBOOL WINAPI CredReadW(LPCWSTR TargetName,DWORD Type,DWORD Flags,PCREDENTIALW *Credential);
  WINADVAPI WINBOOL WINAPI CredReadA(LPCSTR TargetName,DWORD Type,DWORD Flags,PCREDENTIALA *Credential);
  WINADVAPI WINBOOL WINAPI CredEnumerateW(LPCWSTR Filter,DWORD Flags,DWORD *Count,PCREDENTIALW **Credential);
  WINADVAPI WINBOOL WINAPI CredEnumerateA(LPCSTR Filter,DWORD Flags,DWORD *Count,PCREDENTIALA **Credential);
  WINADVAPI WINBOOL WINAPI CredWriteDomainCredentialsW(PCREDENTIAL_TARGET_INFORMATIONW TargetInfo,PCREDENTIALW Credential,DWORD Flags);
  WINADVAPI WINBOOL WINAPI CredWriteDomainCredentialsA(PCREDENTIAL_TARGET_INFORMATIONA TargetInfo,PCREDENTIALA Credential,DWORD Flags);

#define CRED_CACHE_TARGET_INFORMATION 0x1

  WINADVAPI WINBOOL WINAPI CredReadDomainCredentialsW(PCREDENTIAL_TARGET_INFORMATIONW TargetInfo,DWORD Flags,DWORD *Count,PCREDENTIALW **Credential);
  WINADVAPI WINBOOL WINAPI CredReadDomainCredentialsA(PCREDENTIAL_TARGET_INFORMATIONA TargetInfo,DWORD Flags,DWORD *Count,PCREDENTIALA **Credential);
  WINADVAPI WINBOOL WINAPI CredDeleteW(LPCWSTR TargetName,DWORD Type,DWORD Flags);
  WINADVAPI WINBOOL WINAPI CredDeleteA(LPCSTR TargetName,DWORD Type,DWORD Flags);
  WINADVAPI WINBOOL WINAPI CredRenameW(LPCWSTR OldTargetName,LPCWSTR NewTargetName,DWORD Type,DWORD Flags);
  WINADVAPI WINBOOL WINAPI CredRenameA(LPCSTR OldTargetName,LPCSTR NewTargetName,DWORD Type,DWORD Flags);

#define CRED_ALLOW_NAME_RESOLUTION 0x1

  WINADVAPI WINBOOL WINAPI CredGetTargetInfoW(LPCWSTR TargetName,DWORD Flags,PCREDENTIAL_TARGET_INFORMATIONW *TargetInfo);
  WINADVAPI WINBOOL WINAPI CredGetTargetInfoA(LPCSTR TargetName,DWORD Flags,PCREDENTIAL_TARGET_INFORMATIONA *TargetInfo);
  WINADVAPI WINBOOL WINAPI CredMarshalCredentialW(CRED_MARSHAL_TYPE CredType,PVOID Credential,LPWSTR *MarshaledCredential);
  WINADVAPI WINBOOL WINAPI CredMarshalCredentialA(CRED_MARSHAL_TYPE CredType,PVOID Credential,LPSTR *MarshaledCredential);
  WINADVAPI WINBOOL WINAPI CredUnmarshalCredentialW(LPCWSTR MarshaledCredential,PCRED_MARSHAL_TYPE CredType,PVOID *Credential);
  WINADVAPI WINBOOL WINAPI CredUnmarshalCredentialA(LPCSTR MarshaledCredential,PCRED_MARSHAL_TYPE CredType,PVOID *Credential);
  WINADVAPI WINBOOL WINAPI CredIsMarshaledCredentialW(LPCWSTR MarshaledCredential);
  WINADVAPI WINBOOL WINAPI CredIsMarshaledCredentialA(LPCSTR MarshaledCredential);
  WINADVAPI WINBOOL WINAPI CredGetSessionTypes (DWORD MaximumPersistCount,LPDWORD MaximumPersist);
  WINADVAPI VOID WINAPI CredFree (PVOID Buffer);
  CREDUIAPI DWORD WINAPI CredUIPromptForCredentialsW(PCREDUI_INFOW pUiInfo,PCWSTR pszTargetName,PCtxtHandle pContext,DWORD dwAuthError,PWSTR pszUserName,ULONG ulUserNameBufferSize,PWSTR pszPassword,ULONG ulPasswordBufferSize,WINBOOL *save,DWORD dwFlags);
  CREDUIAPI DWORD WINAPI CredUIPromptForCredentialsA(PCREDUI_INFOA pUiInfo,PCSTR pszTargetName,PCtxtHandle pContext,DWORD dwAuthError,PSTR pszUserName,ULONG ulUserNameBufferSize,PSTR pszPassword,ULONG ulPasswordBufferSize,WINBOOL *save,DWORD dwFlags);
  CREDUIAPI DWORD WINAPI CredUIParseUserNameW(CONST WCHAR *UserName,WCHAR *user,ULONG userBufferSize,WCHAR *domain,ULONG domainBufferSize);
  CREDUIAPI DWORD WINAPI CredUIParseUserNameA(CONST CHAR *userName,CHAR *user,ULONG userBufferSize,CHAR *domain,ULONG domainBufferSize);
  CREDUIAPI DWORD WINAPI CredUICmdLinePromptForCredentialsW(PCWSTR pszTargetName,PCtxtHandle pContext,DWORD dwAuthError,PWSTR UserName,ULONG ulUserBufferSize,PWSTR pszPassword,ULONG ulPasswordBufferSize,PBOOL pfSave,DWORD dwFlags);
  CREDUIAPI DWORD WINAPI CredUICmdLinePromptForCredentialsA(PCSTR pszTargetName,PCtxtHandle pContext,DWORD dwAuthError,PSTR UserName,ULONG ulUserBufferSize,PSTR pszPassword,ULONG ulPasswordBufferSize,PBOOL pfSave,DWORD dwFlags);
  CREDUIAPI DWORD WINAPI CredUIConfirmCredentialsW(PCWSTR pszTargetName,WINBOOL bConfirm);
  CREDUIAPI DWORD WINAPI CredUIConfirmCredentialsA(PCSTR pszTargetName,WINBOOL bConfirm);
  CREDUIAPI DWORD WINAPI CredUIStoreSSOCredW (PCWSTR pszRealm,PCWSTR pszUsername,PCWSTR pszPassword,WINBOOL bPersist);
  CREDUIAPI DWORD WINAPI CredUIReadSSOCredW (PCWSTR pszRealm,PWSTR *ppszUsername);

#if (_WIN32_WINNT >= 0x0600)
#define CredFindBestCredential __MINGW_NAME_AW(CredFindBestCredential)
#define CredIsProtected __MINGW_NAME_AW(CredIsProtected)
#define CredPackAuthenticationBuffer __MINGW_NAME_AW(CredPackAuthenticationBuffer)
#define CredProtect __MINGW_NAME_AW(CredProtect)
#define CredUIPromptForWindowsCredentials __MINGW_NAME_AW(CredUIPromptForWindowsCredentials)
#define CredUnPackAuthenticationBuffer __MINGW_NAME_AW(CredUnPackAuthenticationBuffer)
#define CredUnprotect __MINGW_NAME_AW(CredUnprotect)

typedef enum _CRED_PROTECTION_TYPE {
  CredUnprotected         = 0,
  CredUserProtection      = 1,
  CredTrustedProtection   = 2 
} CRED_PROTECTION_TYPE, *PCRED_PROTECTION_TYPE;

WINADVAPI WINBOOL WINAPI CredFindBestCredentialA(
  LPCSTR TargetName,
  DWORD Type,
  DWORD Flags,
  PCREDENTIALA *Credential
);

WINADVAPI WINBOOL WINAPI CredFindBestCredentialW(
  LPCWSTR TargetName,
  DWORD Type,
  DWORD Flags,
  PCREDENTIALW *Credential
);

WINADVAPI WINBOOL WINAPI CredIsProtectedA(
  LPSTR pszProtectedCredentials,
  CRED_PROTECTION_TYPE *pProtectionType
);

WINADVAPI WINBOOL WINAPI CredIsProtectedW(
  LPWSTR pszProtectedCredentials,
  CRED_PROTECTION_TYPE *pProtectionType
);

CREDUIAPI WINBOOL WINAPI CredPackAuthenticationBufferA(
  DWORD dwFlags,
  LPSTR pszUserName,
  LPSTR pszPassword,
  PBYTE pPackedCredentials,
  DWORD *pcbPackedCredentials
);

CREDUIAPI WINBOOL WINAPI CredPackAuthenticationBufferW(
  DWORD dwFlags,
  LPWSTR pszUserName,
  LPWSTR pszPassword,
  PBYTE pPackedCredentials,
  DWORD *pcbPackedCredentials
);

WINADVAPI WINBOOL WINAPI CredProtectW(
  WINBOOL fAsSelf,
  LPWSTR pszCredentials,
  DWORD cchCredentials,
  LPWSTR pszProtectedCredentials,
  DWORD *pcchMaxChars,
  CRED_PROTECTION_TYPE *ProtectionType
);

WINADVAPI WINBOOL WINAPI CredProtectA(
  WINBOOL fAsSelf,
  LPSTR pszCredentials,
  DWORD cchCredentials,
  LPSTR pszProtectedCredentials,
  DWORD *pcchMaxChars,
  CRED_PROTECTION_TYPE *ProtectionType
);

CREDUIAPI DWORD WINAPI CredUIPromptForWindowsCredentialsA(
  PCREDUI_INFOA pUiInfo,
  DWORD dwAuthError,
  ULONG *pulAuthPackage,
  LPCVOID pvInAuthBuffer,
  ULONG ulInAuthBufferSize,
  LPVOID *ppvOutAuthBuffer,
  ULONG *pulOutAuthBufferSize,
  WINBOOL *pfSave,
  DWORD dwFlags
);

CREDUIAPI DWORD WINAPI CredUIPromptForWindowsCredentialsW(
  PCREDUI_INFOW pUiInfo,
  DWORD dwAuthError,
  ULONG *pulAuthPackage,
  LPCVOID pvInAuthBuffer,
  ULONG ulInAuthBufferSize,
  LPVOID *ppvOutAuthBuffer,
  ULONG *pulOutAuthBufferSize,
  WINBOOL *pfSave,
  DWORD dwFlags
);

CREDUIAPI WINBOOL WINAPI CredUnPackAuthenticationBufferA(
  DWORD dwFlags,
  PVOID pAuthBuffer,
  DWORD cbAuthBuffer,
  LPSTR pszUserName,
  DWORD *pcchMaxUserName,
  LPSTR pszDomainName,
  DWORD *pcchMaxDomainame,
  LPSTR pszPassword,
  DWORD *pcchMaxPassword
);

CREDUIAPI WINBOOL WINAPI CredUnPackAuthenticationBufferW(
  DWORD dwFlags,
  PVOID pAuthBuffer,
  DWORD cbAuthBuffer,
  LPWSTR pszUserName,
  DWORD *pcchMaxUserName,
  LPWSTR pszDomainName,
  DWORD *pcchMaxDomainame,
  LPWSTR pszPassword,
  DWORD *pcchMaxPassword
);

WINADVAPI WINBOOL WINAPI CredUnprotectA(
  WINBOOL fAsSelf,
  LPSTR pszProtectedCredentials,
  DWORD cchCredentials,
  LPSTR pszCredentials,
  DWORD *pcchMaxChars
);

WINADVAPI WINBOOL WINAPI CredUnprotectW(
  WINBOOL fAsSelf,
  LPWSTR pszProtectedCredentials,
  DWORD cchCredentials,
  LPWSTR pszCredentials,
  DWORD *pcchMaxChars
);

#endif /* (_WIN32_WINNT >= 0x0600) */

#ifdef __cplusplus
}
#endif
#endif
