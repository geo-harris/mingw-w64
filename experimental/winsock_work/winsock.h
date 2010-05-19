/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_

#ifndef _INC_WINDOWS
#include <windows.h>
#endif

#include <_bsd_types.h>
#include <_timeval.h>
#include <_socket_types.h>
#include <_ip_types.h>
#include <_ip_mreq1.h>

#ifdef __cplusplus
extern "C" {
#endif

  extern int WINAPI __WSAFDIsSet(SOCKET,fd_set *);

#ifdef __cplusplus
}
#endif

#define IOCPARM_MASK 0x7f
#define IOC_VOID 0x20000000
#define IOC_OUT 0x40000000
#define IOC_IN 0x80000000
#define IOC_INOUT (IOC_IN|IOC_OUT)

#define _IO(x,y) (IOC_VOID|((x)<<8)|(y))
#define _IOR(x,y,t) (IOC_OUT|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#define _IOW(x,y,t) (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define FIONREAD _IOR('f',127,u_long)
#define FIONBIO _IOW('f',126,u_long)
#define FIOASYNC _IOW('f',125,u_long)

#define SIOCSHIWAT _IOW('s',0,u_long)
#define SIOCGHIWAT _IOR('s',1,u_long)
#define SIOCSLOWAT _IOW('s',2,u_long)
#define SIOCGLOWAT _IOR('s',3,u_long)
#define SIOCATMARK _IOR('s',7,u_long)

#define IPPROTO_IP 0
#define IPPROTO_ICMP 1
#define IPPROTO_IGMP 2
#define IPPROTO_GGP 3
#define IPPROTO_TCP 6
#define IPPROTO_PUP 12
#define IPPROTO_UDP 17
#define IPPROTO_IDP 22
#define IPPROTO_ND 77

#define IPPROTO_RAW 255
#define IPPROTO_MAX 256

#define IPPORT_ECHO 7
#define IPPORT_DISCARD 9
#define IPPORT_SYSTAT 11
#define IPPORT_DAYTIME 13
#define IPPORT_NETSTAT 15
#define IPPORT_FTP 21
#define IPPORT_TELNET 23
#define IPPORT_SMTP 25
#define IPPORT_TIMESERVER 37
#define IPPORT_NAMESERVER 42
#define IPPORT_WHOIS 43
#define IPPORT_MTP 57

#define IPPORT_TFTP 69
#define IPPORT_RJE 77
#define IPPORT_FINGER 79
#define IPPORT_TTYLINK 87
#define IPPORT_SUPDUP 95

#define IPPORT_EXECSERVER 512
#define IPPORT_LOGINSERVER 513
#define IPPORT_CMDSERVER 514
#define IPPORT_EFSSERVER 520

#define IPPORT_BIFFUDP 512
#define IPPORT_WHOSERVER 513
#define IPPORT_ROUTESERVER 520

#define IPPORT_RESERVED 1024

#define IMPLINK_IP 155
#define IMPLINK_LOWEXPER 156
#define IMPLINK_HIGHEXPER 158

#include <inaddr.h>

#define IN_CLASSA(i) (((long)(i) & 0x80000000)==0)
#define IN_CLASSA_NET 0xff000000
#define IN_CLASSA_NSHIFT 24
#define IN_CLASSA_HOST 0x00ffffff
#define IN_CLASSA_MAX 128

#define IN_CLASSB(i) (((long)(i) & 0xc0000000)==0x80000000)
#define IN_CLASSB_NET 0xffff0000
#define IN_CLASSB_NSHIFT 16
#define IN_CLASSB_HOST 0x0000ffff
#define IN_CLASSB_MAX 65536

#define IN_CLASSC(i) (((long)(i) & 0xe0000000)==0xc0000000)
#define IN_CLASSC_NET 0xffffff00
#define IN_CLASSC_NSHIFT 8
#define IN_CLASSC_HOST 0x000000ff

#define INADDR_ANY (u_long)0x00000000
#define INADDR_LOOPBACK 0x7f000001
#define INADDR_BROADCAST (u_long)0xffffffff
#define INADDR_NONE 0xffffffff

#define WSADESCRIPTION_LEN 256
#define WSASYS_STATUS_LEN 128

typedef struct WSAData {
  WORD wVersion;
  WORD wHighVersion;
#ifdef _WIN64
  unsigned short iMaxSockets;
  unsigned short iMaxUdpDg;
  char *lpVendorInfo;
  char szDescription[WSADESCRIPTION_LEN+1];
  char szSystemStatus[WSASYS_STATUS_LEN+1];
#else
  char szDescription[WSADESCRIPTION_LEN+1];
  char szSystemStatus[WSASYS_STATUS_LEN+1];
  unsigned short iMaxSockets;
  unsigned short iMaxUdpDg;
  char *lpVendorInfo;
#endif
} WSADATA;

typedef WSADATA *LPWSADATA;

#define IP_OPTIONS 1
#define IP_MULTICAST_IF 2
#define IP_MULTICAST_TTL 3
#define IP_MULTICAST_LOOP 4
#define IP_ADD_MEMBERSHIP 5
#define IP_DROP_MEMBERSHIP 6
#define IP_TTL 7
#define IP_TOS 8
#define IP_DONTFRAGMENT 9

#define IP_DEFAULT_MULTICAST_TTL 1
#define IP_DEFAULT_MULTICAST_LOOP 1
#define IP_MAX_MEMBERSHIPS 20

#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3
#define SOCK_RDM 4
#define SOCK_SEQPACKET 5

#define SO_DEBUG 0x0001
#define SO_ACCEPTCONN 0x0002
#define SO_REUSEADDR 0x0004
#define SO_KEEPALIVE 0x0008
#define SO_DONTROUTE 0x0010
#define SO_BROADCAST 0x0020
#define SO_USELOOPBACK 0x0040
#define SO_LINGER 0x0080
#define SO_OOBINLINE 0x0100

#define SO_DONTLINGER (u_int)(~SO_LINGER)

#define SO_SNDBUF 0x1001
#define SO_RCVBUF 0x1002
#define SO_SNDLOWAT 0x1003
#define SO_RCVLOWAT 0x1004
#define SO_SNDTIMEO 0x1005
#define SO_RCVTIMEO 0x1006
#define SO_ERROR 0x1007
#define SO_TYPE 0x1008

#define SO_CONNDATA 0x7000
#define SO_CONNOPT 0x7001
#define SO_DISCDATA 0x7002
#define SO_DISCOPT 0x7003
#define SO_CONNDATALEN 0x7004
#define SO_CONNOPTLEN 0x7005
#define SO_DISCDATALEN 0x7006
#define SO_DISCOPTLEN 0x7007

#define SO_OPENTYPE 0x7008

#define SO_SYNCHRONOUS_ALERT 0x10
#define SO_SYNCHRONOUS_NONALERT 0x20

#define SO_MAXDG 0x7009
#define SO_MAXPATHDG 0x700A
#define SO_UPDATE_ACCEPT_CONTEXT 0x700B
#define SO_CONNECT_TIME 0x700C

#define TCP_NODELAY 0x0001
#define TCP_BSDURGENT 0x7000

#define AF_UNSPEC 0
#define AF_UNIX 1
#define AF_INET 2
#define AF_IMPLINK 3
#define AF_PUP 4
#define AF_CHAOS 5
#define AF_IPX 6
#define AF_NS 6
#define AF_ISO 7
#define AF_OSI AF_ISO
#define AF_ECMA 8
#define AF_DATAKIT 9
#define AF_CCITT 10
#define AF_SNA 11
#define AF_DECnet 12
#define AF_DLI 13
#define AF_LAT 14
#define AF_HYLINK 15
#define AF_APPLETALK 16
#define AF_NETBIOS 17
#define AF_VOICEVIEW 18
#define AF_FIREFOX 19
#define AF_UNKNOWN1 20
#define AF_BAN 21

#define AF_MAX 22

#define PF_UNSPEC AF_UNSPEC
#define PF_UNIX AF_UNIX
#define PF_INET AF_INET
#define PF_IMPLINK AF_IMPLINK
#define PF_PUP AF_PUP
#define PF_CHAOS AF_CHAOS
#define PF_NS AF_NS
#define PF_IPX AF_IPX
#define PF_ISO AF_ISO
#define PF_OSI AF_OSI
#define PF_ECMA AF_ECMA
#define PF_DATAKIT AF_DATAKIT
#define PF_CCITT AF_CCITT
#define PF_SNA AF_SNA
#define PF_DECnet AF_DECnet
#define PF_DLI AF_DLI
#define PF_LAT AF_LAT
#define PF_HYLINK AF_HYLINK
#define PF_APPLETALK AF_APPLETALK
#define PF_VOICEVIEW AF_VOICEVIEW
#define PF_FIREFOX AF_FIREFOX
#define PF_UNKNOWN1 AF_UNKNOWN1
#define PF_BAN AF_BAN

#define PF_MAX AF_MAX

#define SOL_SOCKET 0xffff

#define SOMAXCONN 5

#define MSG_OOB 0x1
#define MSG_PEEK 0x2
#define MSG_DONTROUTE 0x4

#define MSG_MAXIOVLEN 16

#define MSG_PARTIAL 0x8000

#define MAXGETHOSTSTRUCT 1024

#define FD_READ 0x01
#define FD_WRITE 0x02
#define FD_OOB 0x04
#define FD_ACCEPT 0x08
#define FD_CONNECT 0x10
#define FD_CLOSE 0x20

#ifndef WSABASEERR

#define WSABASEERR 10000

#define WSAEINTR (WSABASEERR+4)
#define WSAEBADF (WSABASEERR+9)
#define WSAEACCES (WSABASEERR+13)
#define WSAEFAULT (WSABASEERR+14)
#define WSAEINVAL (WSABASEERR+22)
#define WSAEMFILE (WSABASEERR+24)

#define WSAEWOULDBLOCK (WSABASEERR+35)
#define WSAEINPROGRESS (WSABASEERR+36)
#define WSAEALREADY (WSABASEERR+37)
#define WSAENOTSOCK (WSABASEERR+38)
#define WSAEDESTADDRREQ (WSABASEERR+39)
#define WSAEMSGSIZE (WSABASEERR+40)
#define WSAEPROTOTYPE (WSABASEERR+41)
#define WSAENOPROTOOPT (WSABASEERR+42)
#define WSAEPROTONOSUPPORT (WSABASEERR+43)
#define WSAESOCKTNOSUPPORT (WSABASEERR+44)
#define WSAEOPNOTSUPP (WSABASEERR+45)
#define WSAEPFNOSUPPORT (WSABASEERR+46)
#define WSAEAFNOSUPPORT (WSABASEERR+47)
#define WSAEADDRINUSE (WSABASEERR+48)
#define WSAEADDRNOTAVAIL (WSABASEERR+49)
#define WSAENETDOWN (WSABASEERR+50)
#define WSAENETUNREACH (WSABASEERR+51)
#define WSAENETRESET (WSABASEERR+52)
#define WSAECONNABORTED (WSABASEERR+53)
#define WSAECONNRESET (WSABASEERR+54)
#define WSAENOBUFS (WSABASEERR+55)
#define WSAEISCONN (WSABASEERR+56)
#define WSAENOTCONN (WSABASEERR+57)
#define WSAESHUTDOWN (WSABASEERR+58)
#define WSAETOOMANYREFS (WSABASEERR+59)
#define WSAETIMEDOUT (WSABASEERR+60)
#define WSAECONNREFUSED (WSABASEERR+61)
#define WSAELOOP (WSABASEERR+62)
#define WSAENAMETOOLONG (WSABASEERR+63)
#define WSAEHOSTDOWN (WSABASEERR+64)
#define WSAEHOSTUNREACH (WSABASEERR+65)
#define WSAENOTEMPTY (WSABASEERR+66)
#define WSAEPROCLIM (WSABASEERR+67)
#define WSAEUSERS (WSABASEERR+68)
#define WSAEDQUOT (WSABASEERR+69)
#define WSAESTALE (WSABASEERR+70)
#define WSAEREMOTE (WSABASEERR+71)

#define WSASYSNOTREADY (WSABASEERR+91)
#define WSAVERNOTSUPPORTED (WSABASEERR+92)
#define WSANOTINITIALISED (WSABASEERR+93)

#define WSAEDISCON (WSABASEERR+101)

#define WSAHOST_NOT_FOUND (WSABASEERR+1001)
#define WSATRY_AGAIN (WSABASEERR+1002)
#define WSANO_RECOVERY (WSABASEERR+1003)
#define WSANO_DATA (WSABASEERR+1004)
#endif

#define h_errno WSAGetLastError()
#define HOST_NOT_FOUND WSAHOST_NOT_FOUND
#define TRY_AGAIN WSATRY_AGAIN
#define NO_RECOVERY WSANO_RECOVERY
#define NO_DATA WSANO_DATA

#define WSANO_ADDRESS WSANO_DATA
#define NO_ADDRESS WSANO_ADDRESS

#if 0
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EINPROGRESS WSAEINPROGRESS
#define EALREADY WSAEALREADY
#define ENOTSOCK WSAENOTSOCK
#define EDESTADDRREQ WSAEDESTADDRREQ
#define EMSGSIZE WSAEMSGSIZE
#define EPROTOTYPE WSAEPROTOTYPE
#define ENOPROTOOPT WSAENOPROTOOPT
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT WSAESOCKTNOSUPPORT
#define EOPNOTSUPP WSAEOPNOTSUPP
#define EPFNOSUPPORT WSAEPFNOSUPPORT
#define EAFNOSUPPORT WSAEAFNOSUPPORT
#define EADDRINUSE WSAEADDRINUSE
#define EADDRNOTAVAIL WSAEADDRNOTAVAIL
#define ENETDOWN WSAENETDOWN
#define ENETUNREACH WSAENETUNREACH
#define ENETRESET WSAENETRESET
#define ECONNABORTED WSAECONNABORTED
#define ECONNRESET WSAECONNRESET
#define ENOBUFS WSAENOBUFS
#define EISCONN WSAEISCONN
#define ENOTCONN WSAENOTCONN
#define ESHUTDOWN WSAESHUTDOWN
#define ETOOMANYREFS WSAETOOMANYREFS
#define ETIMEDOUT WSAETIMEDOUT
#define ECONNREFUSED WSAECONNREFUSED
#define ELOOP WSAELOOP
#define ENAMETOOLONG WSAENAMETOOLONG
#define EHOSTDOWN WSAEHOSTDOWN
#define EHOSTUNREACH WSAEHOSTUNREACH
#define ENOTEMPTY WSAENOTEMPTY
#define EPROCLIM WSAEPROCLIM
#define EUSERS WSAEUSERS
#define EDQUOT WSAEDQUOT
#define ESTALE WSAESTALE
#define EREMOTE WSAEREMOTE
#endif

#ifdef __cplusplus
extern "C" {
#endif

  SOCKET WINAPI accept(SOCKET s,struct sockaddr *addr,int *addrlen);
  int WINAPI bind(SOCKET s,const struct sockaddr *addr,int namelen);
  int WINAPI closesocket(SOCKET s);
  int WINAPI connect(SOCKET s,const struct sockaddr *name,int namelen);
  int WINAPI ioctlsocket(SOCKET s,long cmd,u_long *argp);
  int WINAPI getpeername(SOCKET s,struct sockaddr *name,int *namelen);
  int WINAPI getsockname(SOCKET s,struct sockaddr *name,int *namelen);
  int WINAPI getsockopt(SOCKET s,int level,int optname,char *optval,int *optlen);
  u_long WINAPI htonl(u_long hostlong);
  u_short WINAPI htons(u_short hostshort);
  unsigned long WINAPI inet_addr(const char *cp);
  char *WINAPI inet_ntoa(struct in_addr in);
  int WINAPI listen(SOCKET s,int backlog);
  u_long WINAPI ntohl(u_long netlong);
  u_short WINAPI ntohs(u_short netshort);
  int WINAPI recv(SOCKET s,char *buf,int len,int flags);
  int WINAPI recvfrom(SOCKET s,char *buf,int len,int flags,struct sockaddr *from,int *fromlen);
  int WINAPI select(int nfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,const struct timeval *timeout);
  int WINAPI send(SOCKET s,const char *buf,int len,int flags);
  int WINAPI sendto(SOCKET s,const char *buf,int len,int flags,const struct sockaddr *to,int tolen);
  int WINAPI setsockopt(SOCKET s,int level,int optname,const char *optval,int optlen);
  int WINAPI shutdown(SOCKET s,int how);
  SOCKET WINAPI socket(int af,int type,int protocol);
  struct hostent *WINAPI gethostbyaddr(const char *addr,int len,int type);
  struct hostent *WINAPI gethostbyname(const char *name);
  int WINAPI gethostname(char *name,int namelen);
  struct servent *WINAPI getservbyport(int port,const char *proto);
  struct servent *WINAPI getservbyname(const char *name,const char *proto);
  struct protoent *WINAPI getprotobynumber(int proto);
  struct protoent *WINAPI getprotobyname(const char *name);
  int WINAPI WSAStartup(WORD wVersionRequired,LPWSADATA lpWSAData);
  int WINAPI WSACleanup(void);
  void WINAPI WSASetLastError(int iError);
  int WINAPI WSAGetLastError(void);
  WINBOOL WINAPI WSAIsBlocking(void);
  int WINAPI WSAUnhookBlockingHook(void);
  FARPROC WINAPI WSASetBlockingHook(FARPROC lpBlockFunc);
  int WINAPI WSACancelBlockingCall(void);
  HANDLE WINAPI WSAAsyncGetServByName(HWND hWnd,u_int wMsg,const char *name,const char *proto,char *buf,int buflen);
  HANDLE WINAPI WSAAsyncGetServByPort(HWND hWnd,u_int wMsg,int port,const char *proto,char *buf,int buflen);
  HANDLE WINAPI WSAAsyncGetProtoByName(HWND hWnd,u_int wMsg,const char *name,char *buf,int buflen);
  HANDLE WINAPI WSAAsyncGetProtoByNumber(HWND hWnd,u_int wMsg,int number,char *buf,int buflen);
  HANDLE WINAPI WSAAsyncGetHostByName(HWND hWnd,u_int wMsg,const char *name,char *buf,int buflen);
  HANDLE WINAPI WSAAsyncGetHostByAddr(HWND hWnd,u_int wMsg,const char *addr,int len,int type,char *buf,int buflen);
  int WINAPI WSACancelAsyncRequest(HANDLE hAsyncTaskHandle);
  int WINAPI WSAAsyncSelect(SOCKET s,HWND hWnd,u_int wMsg,long lEvent);
  int WINAPI WSARecvEx(SOCKET s,char *buf,int len,int *flags);

  typedef struct _TRANSMIT_FILE_BUFFERS {
    PVOID Head;
    DWORD HeadLength;
    PVOID Tail;
    DWORD TailLength;
  } TRANSMIT_FILE_BUFFERS,*PTRANSMIT_FILE_BUFFERS,*LPTRANSMIT_FILE_BUFFERS;

#define TF_DISCONNECT 0x01
#define TF_REUSE_SOCKET 0x02
#define TF_WRITE_BEHIND 0x04

  WINBOOL WINAPI TransmitFile(SOCKET hSocket,HANDLE hFile,DWORD nNumberOfBytesToWrite,DWORD nNumberOfBytesPerSend,LPOVERLAPPED lpOverlapped,LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,DWORD dwReserved);
  WINBOOL WINAPI AcceptEx(SOCKET sListenSocket,SOCKET sAcceptSocket,PVOID lpOutputBuffer,DWORD dwReceiveDataLength,DWORD dwLocalAddressLength,DWORD dwRemoteAddressLength,LPDWORD lpdwBytesReceived,LPOVERLAPPED lpOverlapped);
  VOID WINAPI GetAcceptExSockaddrs (PVOID lpOutputBuffer,DWORD dwReceiveDataLength,DWORD dwLocalAddressLength,DWORD dwRemoteAddressLength,struct sockaddr **LocalSockaddr,LPINT LocalSockaddrLength,struct sockaddr **RemoteSockaddr,LPINT RemoteSockaddrLength);

#ifdef __cplusplus
}
#endif

#define WSAMAKEASYNCREPLY(buflen,error) MAKELONG(buflen,error)
#define WSAMAKESELECTREPLY(event,error) MAKELONG(event,error)
#define WSAGETASYNCBUFLEN(lParam) LOWORD(lParam)
#define WSAGETASYNCERROR(lParam) HIWORD(lParam)
#define WSAGETSELECTEVENT(lParam) LOWORD(lParam)
#define WSAGETSELECTERROR(lParam) HIWORD(lParam)

#ifdef IPV6STRICT
#error WINSOCK2 required.
#endif
#endif
