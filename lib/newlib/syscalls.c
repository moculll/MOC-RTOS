/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 *               2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup    sys_newlib Newlib system call
 * @ingroup     sys
 * @brief       Newlib system call
 * @{
 *
 * @file
 * @brief       Newlib system call implementations
 *
 * @author      Michael Baar <michael.baar@fu-berlin.de>
 * @author      Stefan Pfeiffer <pfeiffer@inf.fu-berlin.de>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 *
 * @}
 */

#include <errno.h>
#include <malloc.h>
#include <reent.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <shellMgr/shellMgr.h>
#include <osImpl/irqImpl.h>


#ifndef NUM_HEAPS
#define NUM_HEAPS 1
#endif


extern char __HeapBase;
extern char __HeapLimit;

typedef struct  {
    char* start;
    char* end;
} mHeap_t;

static char *heap_top[NUM_HEAPS] = {
    &__HeapBase,
#if NUM_HEAPS > 1
    &_sheap1,
#endif
#if NUM_HEAPS > 2
    &_sheap2,
#endif
#if NUM_HEAPS > 3
    &_sheap3,
#endif
#if NUM_HEAPS > 4
#error "Unsupported NUM_HEAPS value, edit newlib_syscalls_default/syscalls.c to add more heaps."
#endif
};

static const mHeap_t heaps[NUM_HEAPS] = {
    {
        .start = &__HeapBase,
        .end   = &__HeapLimit
    },
#if NUM_HEAPS > 1
    {
        .start = &_sheap1,
        .end   = &_eheap1
    },
#endif
#if NUM_HEAPS > 2
    {
        .start = &_sheap2,
        .end   = &_eheap2
    },
#endif
#if NUM_HEAPS > 3
    {
        .start = &_sheap3,
        .end   = &_eheap3
    },
#endif
};

/**
 * @brief Initialize NewLib, called by __libc_init_array() from the startup script
 */
void _init(void)
{
    /* Definition copied from newlib/libc/stdio/local.h */
    extern void __sinit (struct _reent *);

    /* When running multiple threads: Initialize reentrant structure before the
     * scheduler starts. This normally happens upon the first stdio function
     * called. However, if no boot message happens this can result in two
     * concurrent "first calls" to stdio in data corruption, if no locking is
     * used. Except for ESP (which is using its own syscalls.c anyway), this
     * currently is the case in RIOT. */

        /* Also, make an exception for riotboot, which does not use stdio
         * at all. This would pull in stdio and increase .text size
         * significantly there */

    __sinit(_REENT);

}

/**
 * @brief Free resources on NewLib de-initialization, not used for RIOT
 */
/* __attribute__((used)) fixes linker errors when building with LTO, but without nano.specs */
__attribute__((used)) void _fini(void)
{
    /* nothing to do here */
}

/**
 * @brief Exit a program without cleaning up files
 *
 * If your system doesn't provide this, it is best to avoid linking with subroutines that
 * require it (exit, system).
 *
 * @param n     the exit code, 0 for all OK, >0 for not OK
 */
__attribute__((used)) void _exit(int n)
{

    while (1) {}
}

/**
 * @brief Allocate memory from the heap.
 *
 * @return      pointer to the newly allocated memory on success
 * @return      pointer set to address `-1` on failure
 */
void *_sbrk_r(struct _reent *r, ptrdiff_t incr)
{
    void *res = (void*)UINTPTR_MAX;
    unsigned int state = mDisableIrqImpl();

    for (unsigned i = 0; i < NUM_HEAPS; ++i) {
        if ((heap_top[i] + incr > heaps[i].end) ||
            (heap_top[i] + incr < heaps[i].start)) {
            continue;
        }

        res = heap_top[i];
        heap_top[i] += incr;
        break;
    }

    if (res == (void*)UINTPTR_MAX) {
        r->_errno = ENOMEM;
    }

    mRestoreIrqImpl(state);
    return res;
}

/**
 * @brief Print heap statistics
 *
 * If the CPU does not provide its own heap handling and heap_stats function,
 * but instead uses the newlib_syscall_default function, this function outputs
 * the heap statistics. If the CPU provides its own heap_stats function, it
 * should define HAVE_HEAP_STATS in its cpu_conf.h file.
 */
#ifndef HAVE_HEAP_STATS
__attribute__((weak)) void heap_stats(void)
{
    struct mallinfo minfo = mallinfo();
    long int heap_size = 0;

    for (unsigned int i = 0; i < NUM_HEAPS; i++) {
        heap_size += heaps[i].end - heaps[i].start;
    }

    printf("heap: %ld (used %d, free %ld) [bytes]\n",
           heap_size, minfo.uordblks, heap_size - minfo.uordblks);
}
#endif /* HAVE_HEAP_STATS */

/**
 * @brief Get the process-ID of the current thread
 *
 * @return      the process ID of the current thread
 */
pid_t _getpid(void)
{
    return 0;
}

/**
 * @brief Get the process-ID of the current thread
 *
 * @return      the process ID of the current thread
 */
pid_t _getpid_r(struct _reent *ptr)
{
    (void) ptr;
    return 0;
}

/**
 * @brief Send a signal to a given thread
 *
 * @param r     TODO
 * @param pid   TODO
 * @param sig   TODO
 *
 * @return      TODO
 */
__attribute__ ((weak))
int _kill_r(struct _reent *r, pid_t pid, int sig)
{
    (void) pid;
    (void) sig;
    r->_errno = ESRCH;                      /* not implemented yet */
    return -1;
}

/* Fallback stdio_uart wrappers for when VFS is not used, does not allow any
 * other file access */
/*
 * Fallback read function
 *
 * All input is read from stdio_uart regardless of fd number. The function will
 * block until a byte is actually read.
 *
 * Note: the read function does not buffer - data will be lost if the function is not
 * called fast enough.
 */
_ssize_t _read_r(struct _reent *r, int fd, void *buffer, size_t count)
{
    (void)r;
    (void)fd;
    return shellMgr->getString(buffer, count);
}

/*
 * Fallback write function
 *
 * All output is directed to stdio_uart, independent of the given file descriptor.
 * The write call will further block until the byte is actually written to the UART.
 */
_ssize_t _write_r(struct _reent *r, int fd, const void *data, size_t count)
{
    (void) r;
    (void) fd;
    return shellMgr->outputStringSize(data, count);
}

/* Stubs to avoid linking errors, these functions do not have any effect */
int _open_r(struct _reent *r, const char *name, int flags, int mode)
{
    (void) name;
    (void) flags;
    (void) mode;
    r->_errno = ENODEV;
    return -1;
}

int _close_r(struct _reent *r, int fd)
{
    (void) fd;
    r->_errno = ENODEV;
    return -1;
}

_off_t _lseek_r(struct _reent *r, int fd, _off_t pos, int dir)
{
    (void) fd;
    (void) pos;
    (void) dir;
    r->_errno = ENODEV;
    return -1;
}

int _fstat_r(struct _reent *r, int fd, struct stat *st)
{
    (void) fd;
    (void) st;
    r->_errno = ENODEV;
    return -1;
}

int _stat_r(struct _reent *r, const char *name, struct stat *st)
{
    (void) name;
    (void) st;
    r->_errno = ENODEV;
    return -1;
}

int _unlink_r(struct _reent *r, const char *path)
{
    (void) path;
    r->_errno = ENODEV;
    return -1;
}


/**
 * Create a hard link (not implemented).
 *
 * @todo    Not implemented.
 *
 * @return  -1. Sets errno to ENOSYS.
 */
int _link_r(struct _reent *ptr, const char *old_name, const char *new_name)
{
    (void)old_name;
    (void)new_name;

    ptr->_errno = ENOSYS;

    return -1;
}

/**
 * @brief Query whether output stream is a terminal
 *
 * @param r     TODO
 * @param fd    TODO
 *
 * @return      TODO
 */
int _isatty_r(struct _reent *r, int fd)
{
    r->_errno = 0;

    if (fd == STDIN_FILENO || fd == STDOUT_FILENO || fd == STDERR_FILENO) {
        return 1;
    }

    return 0;
}

/**
 * @brief Send a signal to a thread
 *
 * @param[in] pid the pid to send to
 * @param[in] sig the signal to send
 *
 * @return TODO
 */
__attribute__ ((weak))
int _kill(pid_t pid, int sig)
{
    (void) pid;
    (void) sig;
    errno = ESRCH;                         /* not implemented yet */
    return -1;
}


/* int _gettimeofday_r(struct _reent *r, struct timeval *restrict tp, void *restrict tzp)
{
    (void)tzp;
    (void)r;
    uint64_t now = xtimer_now_usec64();
    tp->tv_sec = div_u64_by_1000000(now);
    tp->tv_usec = now - (tp->tv_sec * US_PER_SEC);
    return 0;
} */


/**
 * Current process times (not implemented).
 *
 * @param[out]  ptms    Not modified.
 *
 * @return  -1, this function always fails. errno is set to ENOSYS.
 */
clock_t _times_r(struct _reent *ptr, struct tms *ptms)
{
    (void)ptms;
    ptr->_errno = ENOSYS;

    return (-1);
}
