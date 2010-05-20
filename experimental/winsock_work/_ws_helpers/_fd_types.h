#ifndef ___WSA_FD_TYPES_H
#define ___WSA_FD_TYPES_H

#include <_ws_helpers/_socket_types.h>

#ifndef FD_SETSIZE
#define FD_SETSIZE	64
#endif
typedef struct fd_set
{
	u_int	fd_count;
	SOCKET	fd_array[FD_SETSIZE];
} fd_set;

typedef struct fd_set	FD_SET;
typedef struct fd_set	*PFD_SET;
typedef struct fd_set	*LPFD_SET;

#endif /* ___WSA_FD_TYPES_H */

