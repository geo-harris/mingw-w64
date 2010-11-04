#ifndef WIN_PTHREAD_H
#define WIN_PTHREAD_H

#include <windows.h>
#include <setjmp.h>
#include "rwlock.h"

#define LIFE_THREAD 0xBAB1F00D
#define DEAD_THREAD 0xDEADBEEF

/* private non-public types.  */
typedef struct _pthread_cleanup _pthread_cleanup;
struct _pthread_cleanup
{
    void (*func)(pthread_once_t *);
    void *arg;
    _pthread_cleanup *next;
};

typedef struct _pthread_v _pthread_v;
struct _pthread_v
{
    unsigned int valid;   
    void *ret_arg;
    void *(* func)(void *);
    _pthread_cleanup *clean;
    HANDLE h;
    int cancelled;
    unsigned int p_state;
    unsigned int keymax;
    void **keyval;
    DWORD tid;
    int rwlc;
    pthread_rwlock_t rwlq[RWLS_PER_THREAD];

    jmp_buf jb;
};

#define pthread_cleanup_push(F, A)\
{\
    const _pthread_cleanup _pthread_cup = {(F), (A), pthread_self()->clean};\
    _ReadWriteBarrier();\
    pthread_self()->clean = (_pthread_cleanup *) &_pthread_cup;\
    _ReadWriteBarrier()

/* Note that if async cancelling is used, then there is a race here */
#define pthread_cleanup_pop(E)\
    (pthread_self()->clean = _pthread_cup.next, (E?_pthread_cup.func((pthread_once_t *)_pthread_cup.arg):0));}

int _pthread_tryjoin(pthread_t t, void **res);

void thread_print_set(int state);
void thread_print(volatile pthread_t t, char *txt);

#endif
