#ifndef _ETRAX_TYPES_H
#define _ETRAX_TYPES_H

#include <asm-generic/int-ll64.h>

<<<<<<< HEAD
#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

#endif /* __ASSEMBLY__ */

=======
>>>>>>> cm-10.0
/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__

#define BITS_PER_LONG 32

#endif /* __KERNEL__ */

#endif
