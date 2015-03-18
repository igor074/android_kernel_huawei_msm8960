/*
 * Copyright 2007-2009 Analog Devices Inc.
 *                         Philippe Gerum <rpm@xenomai.org>
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ASM_BLACKFIN_CPU_H
#define __ASM_BLACKFIN_CPU_H

#include <linux/percpu.h>

struct blackfin_cpudata {
	struct cpu cpu;
	unsigned int imemctl;
	unsigned int dmemctl;
<<<<<<< HEAD
=======
#ifdef CONFIG_SMP
	struct task_struct *idle;
#endif
>>>>>>> cm-10.0
};

DECLARE_PER_CPU(struct blackfin_cpudata, cpu_data);

#endif
