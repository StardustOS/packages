#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <inttypes.h>
#include <stddef.h>

__BEGIN_DECLS

typedef unsigned long int blkcnt_t;	/* Used for file block counts */
typedef unsigned long int blksize_t;	/* Used for block sizes */
typedef unsigned long int clock_t;	/* Used for system times in
					   clock ticks or CLOCKS_PER_SEC
					   (see <time.h>). */

typedef uint64_t fsblkcnt_t;
typedef uint64_t fsfilcnt_t;

#if defined(__alpha__) || defined(__ia64__) || defined(__sparc64__) || defined(__s390x__)
    typedef uint32_t dev_t;		/* Used for device IDs. */
    typedef uint32_t gid_t;		/* Used for group IDs. */
    typedef uint32_t mode_t;		/* Used for some file attributes. */
    typedef uint32_t nlink_t;		/* Used for link counts. */
    typedef uint32_t uid_t;		/* Used for user IDs. */
#elif defined(__arm__) || defined(__i386__) || defined(__sparc__) || defined(__s390__) /* make sure __s390x__ hits before __s390__ */
    typedef uint32_t dev_t;
    typedef uint16_t gid_t;
    typedef uint16_t mode_t;
    typedef uint16_t nlink_t;
    typedef uint16_t uid_t;
#elif defined(__hppa__)
    typedef uint32_t dev_t;
    typedef uint32_t gid_t;
    typedef uint16_t mode_t;
    typedef uint16_t nlink_t;
    typedef uint32_t uid_t;
#elif defined(__mips__)
    typedef uint32_t dev_t;
    typedef int32_t gid_t;
    typedef uint32_t mode_t;
    typedef int32_t nlink_t;
    typedef int32_t uid_t;
#elif defined(__powerpc__) && !defined(__powerpc64__)
    typedef uint32_t dev_t;
    typedef uint32_t gid_t;
    typedef uint32_t mode_t;
    typedef uint16_t nlink_t;
    typedef uint32_t uid_t;
#elif defined(__powerpc64__) || defined(__x86_64__)
    typedef unsigned long dev_t;
    typedef uint32_t gid_t;
    typedef uint32_t mode_t;
    typedef unsigned long nlink_t;
    typedef uint32_t uid_t;
#endif

typedef int32_t id_t;			/* Used as a general identifier; can be
					   used to contain at least a pid_t,
					   uid_t or a gid_t. */
typedef unsigned long ino_t;		/* Used for file serial numbers. */
typedef int32_t key_t;			/* Used for interprocess communication. */
typedef int32_t pid_t;			/* Used for process IDs and process group IDs. */
typedef signed long ssize_t;		/* Used for a count of bytes or an error indication. */
typedef signed long suseconds_t;	/* Used for time in microseconds. */
typedef signed long time_t;		/* Used for time in seconds. */
typedef signed long useconds_t;		/* Used for time in microseconds. */

/* non-susv2 types: */
__extension__ typedef signed long long loff_t;	/* 64-bit offset */

__extension__ typedef signed long long off64_t;
#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
typedef off64_t off_t;
#else
typedef signed long off_t;             /* Used for file sizes. */
#endif

__extension__ typedef unsigned long long ino64_t;
__extension__ typedef signed long long blkcnt64_t;

typedef uint32_t uid32_t;
typedef uint32_t gid32_t;

typedef int32_t clockid_t;
typedef int32_t timer_t;

typedef long int fpos_t;

#define __socklen_t_defined
typedef uint32_t socklen_t;
typedef uint16_t sa_family_t;







__END_DECLS

#endif
