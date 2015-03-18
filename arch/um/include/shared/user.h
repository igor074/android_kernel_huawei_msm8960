/* 
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __USER_H__
#define __USER_H__

<<<<<<< HEAD
#include "kern_constants.h"
=======
#include <generated/asm-offsets.h>
>>>>>>> cm-10.0

/*
 * The usual definition - copied here because the kernel provides its own,
 * fancier, type-safe, definition.  Using that one would require
 * copying too much infrastructure for my taste, so userspace files
 * get less checking than kernel files.
 */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* This is to get size_t */
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stddef.h>
#endif

extern void panic(const char *fmt, ...)
	__attribute__ ((format (printf, 1, 2)));

#ifdef UML_CONFIG_PRINTK
extern int printk(const char *fmt, ...)
	__attribute__ ((format (printf, 1, 2)));
#else
static inline int printk(const char *fmt, ...)
{
	return 0;
}
#endif

<<<<<<< HEAD
extern void schedule(void);
extern int in_aton(char *str);
extern int open_gdb_chan(void);
extern size_t strlcpy(char *, const char *, size_t);
extern size_t strlcat(char *, const char *, size_t);

=======
extern int in_aton(char *str);
extern size_t strlcpy(char *, const char *, size_t);
extern size_t strlcat(char *, const char *, size_t);

/* Copied from linux/compiler-gcc.h since we can't include it directly */
#define barrier() __asm__ __volatile__("": : :"memory")

>>>>>>> cm-10.0
#endif
