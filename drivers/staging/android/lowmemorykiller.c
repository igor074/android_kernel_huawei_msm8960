/* drivers/misc/lowmemorykiller.c
 *
 * The lowmemorykiller driver lets user-space specify a set of memory thresholds
<<<<<<< HEAD
 * where processes with a range of oom_adj values will get killed. Specify the
 * minimum oom_adj values in /sys/module/lowmemorykiller/parameters/adj and the
 * number of free pages in /sys/module/lowmemorykiller/parameters/minfree. Both
 * files take a comma separated list of numbers in ascending order.
 *
 * For example, write "0,8" to /sys/module/lowmemorykiller/parameters/adj and
 * "1024,4096" to /sys/module/lowmemorykiller/parameters/minfree to kill processes
 * with a oom_adj value of 8 or higher when the free memory drops below 4096 pages
 * and kill processes with a oom_adj value of 0 or higher when the free memory
 * drops below 1024 pages.
=======
 * where processes with a range of oom_score_adj values will get killed. Specify
 * the minimum oom_score_adj values in
 * /sys/module/lowmemorykiller/parameters/adj and the number of free pages in
 * /sys/module/lowmemorykiller/parameters/minfree. Both files take a comma
 * separated list of numbers in ascending order.
 *
 * For example, write "0,8" to /sys/module/lowmemorykiller/parameters/adj and
 * "1024,4096" to /sys/module/lowmemorykiller/parameters/minfree to kill
 * processes with a oom_score_adj value of 8 or higher when the free memory
 * drops below 4096 pages and kill processes with a oom_score_adj value of 0 or
 * higher when the free memory drops below 1024 pages.
>>>>>>> cm-10.0
 *
 * The driver considers memory used for caches to be free, but if a large
 * percentage of the cached memory is locked this can be very inaccurate
 * and processes may not get killed until the normal oom killer is triggered.
 *
 * Copyright (C) 2007-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/oom.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/notifier.h>
#include <linux/memory.h>
#include <linux/memory_hotplug.h>

#ifdef CONFIG_HUAWEI_FEATURE_LOW_MEMORY_KILLER_STUB
#include "../../char/lowmemorykiller_stub.h"
#endif
static uint32_t lowmem_debug_level = 2;
=======
#include <linux/rcupdate.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/delay.h>

static uint32_t lowmem_debug_level = 1;
>>>>>>> cm-10.0
static int lowmem_adj[6] = {
	0,
	1,
	6,
	12,
};
static int lowmem_adj_size = 4;
<<<<<<< HEAD
static size_t lowmem_minfree[6] = {
=======
static int lowmem_minfree[6] = {
>>>>>>> cm-10.0
	3 * 512,	/* 6MB */
	2 * 1024,	/* 8MB */
	4 * 1024,	/* 16MB */
	16 * 1024,	/* 64MB */
};
static int lowmem_minfree_size = 4;

<<<<<<< HEAD
static unsigned int offlining;
static struct task_struct *lowmem_deathpending;
#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
static DEFINE_SPINLOCK(lowmem_deathpending_lock);
#else
static unsigned long lowmem_deathpending_timeout;
#endif//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
=======
static unsigned long lowmem_deathpending_timeout;
>>>>>>> cm-10.0

#define lowmem_print(level, x...)			\
	do {						\
		if (lowmem_debug_level >= (level))	\
			printk(x);			\
	} while (0)

<<<<<<< HEAD


static int
task_notify_func(struct notifier_block *self, unsigned long val, void *data);

static struct notifier_block task_nb = {
	.notifier_call	= task_notify_func,
};

#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
static void task_free_fn(struct work_struct *work)
{
        unsigned long flags;

        task_free_unregister(&task_nb);
        spin_lock_irqsave(&lowmem_deathpending_lock, flags);
        lowmem_deathpending = NULL;
        spin_unlock_irqrestore(&lowmem_deathpending_lock, flags);
}
static DECLARE_WORK(task_free_work, task_free_fn);
#endif//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER

static int
task_notify_func(struct notifier_block *self, unsigned long val, void *data)
{
	struct task_struct *task = data;

#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	if (task == lowmem_deathpending) {
             schedule_work(&task_free_work);
        }
#else
	if (task == lowmem_deathpending)
		lowmem_deathpending = NULL;
#endif
	return NOTIFY_OK;
}

#ifdef CONFIG_MEMORY_HOTPLUG
static int lmk_hotplug_callback(struct notifier_block *self,
				unsigned long cmd, void *data)
{
	switch (cmd) {
	/* Don't care LMK cases */
	case MEM_ONLINE:
	case MEM_OFFLINE:
	case MEM_CANCEL_ONLINE:
	case MEM_CANCEL_OFFLINE:
	case MEM_GOING_ONLINE:
		offlining = 0;
		lowmem_print(4, "lmk in normal mode\n");
		break;
	/* LMK should account for movable zone */
	case MEM_GOING_OFFLINE:
		offlining = 1;
		lowmem_print(4, "lmk in hotplug mode\n");
		break;
	}
	return NOTIFY_DONE;
}
#endif



static int lowmem_shrink(struct shrinker *s, struct shrink_control *sc)
{
	struct task_struct *p;
=======
static int test_task_flag(struct task_struct *p, int flag)
{
	struct task_struct *t = p;

	do {
		task_lock(t);
		if (test_tsk_thread_flag(t, flag)) {
			task_unlock(t);
			return 1;
		}
		task_unlock(t);
	} while_each_thread(p, t);

	return 0;
}

static DEFINE_MUTEX(scan_mutex);

static int lowmem_shrink(struct shrinker *s, struct shrink_control *sc)
{
	struct task_struct *tsk;
>>>>>>> cm-10.0
	struct task_struct *selected = NULL;
	int rem = 0;
	int tasksize;
	int i;
<<<<<<< HEAD
	int min_adj = OOM_ADJUST_MAX + 1;
	int selected_tasksize = 0;
	int selected_oom_adj;
	int array_size = ARRAY_SIZE(lowmem_adj);
	int other_free = global_page_state(NR_FREE_PAGES);
	int other_file = global_page_state(NR_FILE_PAGES) -
						global_page_state(NR_SHMEM);
	struct zone *zone;
#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	unsigned long flags;
#endif

	if (offlining) {
		/* Discount all free space in the section being offlined */
		for_each_zone(zone) {
			 if (zone_idx(zone) == ZONE_MOVABLE) {
				other_free -= zone_page_state(zone,
						NR_FREE_PAGES);
				lowmem_print(4, "lowmem_shrink discounted "
					"%lu pages in movable zone\n",
					zone_page_state(zone, NR_FREE_PAGES));
			}
		}
	}
	/*
	 * If we already have a death outstanding, then
	 * bail out right away; indicating to vmscan
	 * that we have nothing further to offer on
	 * this pass.
	 *
	 */
#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	if (lowmem_deathpending)
		return 0;
#else
	if (lowmem_deathpending &&
	    time_before_eq(jiffies, lowmem_deathpending_timeout))
		return 0;
#endif//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
=======
	int min_score_adj = OOM_SCORE_ADJ_MAX + 1;
	int selected_tasksize = 0;
	int selected_oom_score_adj;
	int array_size = ARRAY_SIZE(lowmem_adj);
	int other_free;
	int other_file;
	unsigned long nr_to_scan = sc->nr_to_scan;

	if (nr_to_scan > 0) {
		if (mutex_lock_interruptible(&scan_mutex) < 0)
			return 0;
	}

	other_free = global_page_state(NR_FREE_PAGES);
	other_file = global_page_state(NR_FILE_PAGES) -
						global_page_state(NR_SHMEM);
>>>>>>> cm-10.0

	if (lowmem_adj_size < array_size)
		array_size = lowmem_adj_size;
	if (lowmem_minfree_size < array_size)
		array_size = lowmem_minfree_size;
	for (i = 0; i < array_size; i++) {
		if (other_free < lowmem_minfree[i] &&
		    other_file < lowmem_minfree[i]) {
<<<<<<< HEAD
			min_adj = lowmem_adj[i];
			break;
		}
	}
	if (sc->nr_to_scan > 0)
		lowmem_print(3, "lowmem_shrink %lu, %x, ofree %d %d, ma %d\n",
			     sc->nr_to_scan, sc->gfp_mask, other_free, other_file,
			     min_adj);
=======
			min_score_adj = lowmem_adj[i];
			break;
		}
	}
	if (nr_to_scan > 0)
		lowmem_print(3, "lowmem_shrink %lu, %x, ofree %d %d, ma %d\n",
				nr_to_scan, sc->gfp_mask, other_free,
				other_file, min_score_adj);
>>>>>>> cm-10.0
	rem = global_page_state(NR_ACTIVE_ANON) +
		global_page_state(NR_ACTIVE_FILE) +
		global_page_state(NR_INACTIVE_ANON) +
		global_page_state(NR_INACTIVE_FILE);
<<<<<<< HEAD
	if (sc->nr_to_scan <= 0 || min_adj == OOM_ADJUST_MAX + 1) {
		lowmem_print(5, "lowmem_shrink %lu, %x, return %d\n",
			     sc->nr_to_scan, sc->gfp_mask, rem);
		return rem;
	}
	selected_oom_adj = min_adj;

	read_lock(&tasklist_lock);
	for_each_process(p) {
		struct mm_struct *mm;
		struct signal_struct *sig;
		int oom_adj;

		task_lock(p);
		mm = p->mm;
		sig = p->signal;
		if (!mm || !sig) {
			task_unlock(p);
			continue;
		}
		oom_adj = sig->oom_adj;
		if (oom_adj < min_adj) {
			task_unlock(p);
			continue;
		}
		tasksize = get_mm_rss(mm);
=======
	if (nr_to_scan <= 0 || min_score_adj == OOM_SCORE_ADJ_MAX + 1) {
		lowmem_print(5, "lowmem_shrink %lu, %x, return %d\n",
			     nr_to_scan, sc->gfp_mask, rem);

		if (nr_to_scan > 0)
			mutex_unlock(&scan_mutex);

		return rem;
	}
	selected_oom_score_adj = min_score_adj;

	rcu_read_lock();
	for_each_process(tsk) {
		struct task_struct *p;
		int oom_score_adj;

		if (tsk->flags & PF_KTHREAD)
			continue;

		/* if task no longer has any memory ignore it */
		if (test_task_flag(tsk, TIF_MM_RELEASED))
			continue;

		if (time_before_eq(jiffies, lowmem_deathpending_timeout)) {
			if (test_task_flag(tsk, TIF_MEMDIE)) {
				rcu_read_unlock();
				/* give the system time to free up the memory */
				msleep_interruptible(20);
				mutex_unlock(&scan_mutex);
				return 0;
			}
		}

		p = find_lock_task_mm(tsk);
		if (!p)
			continue;

		oom_score_adj = p->signal->oom_score_adj;
		if (oom_score_adj < min_score_adj) {
			task_unlock(p);
			continue;
		}
		tasksize = get_mm_rss(p->mm);
>>>>>>> cm-10.0
		task_unlock(p);
		if (tasksize <= 0)
			continue;
		if (selected) {
<<<<<<< HEAD
			if (oom_adj < selected_oom_adj)
				continue;
			if (oom_adj == selected_oom_adj &&
=======
			if (oom_score_adj < selected_oom_score_adj)
				continue;
			if (oom_score_adj == selected_oom_score_adj &&
>>>>>>> cm-10.0
			    tasksize <= selected_tasksize)
				continue;
		}
		selected = p;
		selected_tasksize = tasksize;
<<<<<<< HEAD
		selected_oom_adj = oom_adj;
		lowmem_print(2, "select %d (%s), adj %d, size %d, to kill\n",
			     p->pid, p->comm, oom_adj, tasksize);
	}
	if (selected) {
#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	    spin_lock_irqsave(&lowmem_deathpending_lock, flags);
            if (!lowmem_deathpending) {
                lowmem_print(1, "send sigkill to %d (%s), adj %d, size %d\n",
			     selected->pid, selected->comm,
			     selected_oom_adj, selected_tasksize);
		lowmem_deathpending = selected;
		task_free_register(&task_nb);
#ifdef CONFIG_HUAWEI_FEATURE_LOW_MEMORY_KILLER_STUB
                sysLowKernel_write(selected);
#endif
		force_sig(SIGKILL, selected);
		rem -= selected_tasksize;
	    }
	    spin_unlock_irqrestore(&lowmem_deathpending_lock, flags);
#else//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER

		lowmem_print(1, "send sigkill to %d (%s), adj %d, size %d\n",
			     selected->pid, selected->comm,
			     selected_oom_adj, selected_tasksize);
		lowmem_deathpending = selected;
		lowmem_deathpending_timeout = jiffies + HZ;
#ifdef CONFIG_HUAWEI_FEATURE_LOW_MEMORY_KILLER_STUB
        sysLowKernel_write(selected);
#endif
		force_sig(SIGKILL, selected);
		rem -= selected_tasksize;
#endif//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	}
	lowmem_print(4, "lowmem_shrink %lu, %x, return %d\n",
		     sc->nr_to_scan, sc->gfp_mask, rem);
	read_unlock(&tasklist_lock);
	return rem;
}

#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
static void lowmem_vm_shrinker(int largest, int rss_threshold)
{
       struct task_struct *p;
       struct task_struct *selected = NULL;
       int vmsize, rssize;
       int min_adj, min_large_adj;
       int selected_vmsize = 0;
       int selected_oom_adj;
       int array_size = ARRAY_SIZE(lowmem_adj);
       unsigned long flags;

       /*
        * If we already have a death outstanding, then
       * bail out right away; indicating to vmscan
        * that we have nothing further to offer on
        * this pass.
        *
        */
      if (lowmem_deathpending)
               return;

       if (lowmem_adj_size < array_size)
               array_size = lowmem_adj_size;
       if (lowmem_minfree_size < array_size)
              array_size = lowmem_minfree_size;

       min_adj = lowmem_adj[array_size - 2];  /* lock onto cached processes only */
       min_large_adj = lowmem_adj[array_size - 3];  /* Minimum priority for large processes */

       lowmem_print(3, "lowmem_vm_shrink ma %d, large ma %d, largest %d, rss_threshold=%d\n",
                     min_adj, min_large_adj, largest, rss_threshold);

       selected_oom_adj = min_adj;
       read_lock(&tasklist_lock);
       for_each_process(p) {
               struct mm_struct *mm;
               struct signal_struct *sig;
               int oom_adj;

               task_lock(p);
               mm = p->mm;
               sig = p->signal;
               if (!mm || !sig) {
                       task_unlock(p);
                       continue;
               }
               oom_adj = sig->oom_adj;
               vmsize = get_mm_hiwater_vm(mm);
                rssize = get_mm_rss(mm) * PAGE_SIZE;
               task_unlock(p);

               if (vmsize <= 0)
                       continue;

                /* Only look at cached processes */
                if (oom_adj < min_adj) {
                    /* Is this a very large home process in the background? */
                    if ((oom_adj > min_large_adj) && (rssize >= rss_threshold)) {
                        selected = p;
                        selected_vmsize = vmsize;
                       selected_oom_adj = oom_adj;
                        lowmem_print(2, "lowmem_vm_shrink override %d (%s), adj %d, vm size %d, rs size %d to kill",
                                         p->pid, p->comm, oom_adj, vmsize, rssize);
                        break;
                   }

                    continue;
                }

                /* Is this process a better fit than last selected? */
               if (selected) {
                       if (oom_adj < selected_oom_adj)
                               continue;

                        /* If looking for largest, ignore priority */
                       if ((largest || (oom_adj == selected_oom_adj)) &&
                           (vmsize <= selected_vmsize))
                               continue;
               }

               selected = p;
               selected_vmsize = vmsize;

               if (largest == 0)  /* Do not filter by priority if searching for largest */
                       selected_oom_adj = oom_adj;

               lowmem_print(2, "lowmem_vm_shrink select %d (%s), adj %d, vm size %d, rs size %d to kill\n",
                            p->pid, p->comm, oom_adj, vmsize, rssize);
       }
       if (selected) {
               spin_lock_irqsave(&lowmem_deathpending_lock, flags);
               if (!lowmem_deathpending) {
                       lowmem_print(1,
                               "lowmem_vm_shrink send sigkill to %d (%s), adj %d, vm size %d\n",
                               selected->pid, selected->comm,
                               selected_oom_adj, selected_vmsize);
                       lowmem_deathpending = selected;
                       task_free_register(&task_nb);
                       force_sig(SIGKILL, selected);
               }
               spin_unlock_irqrestore(&lowmem_deathpending_lock, flags);
       }
       lowmem_print(4, "lowmem_vm_shrink, saved %d\n", selected_vmsize);
       read_unlock(&tasklist_lock);
       return;
}
#endif//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER

=======
		selected_oom_score_adj = oom_score_adj;
		lowmem_print(2, "select %d (%s), adj %d, size %d, to kill\n",
			     p->pid, p->comm, oom_score_adj, tasksize);
	}
	if (selected) {
		lowmem_print(1, "send sigkill to %d (%s), adj %d, size %d\n",
			     selected->pid, selected->comm,
			     selected_oom_score_adj, selected_tasksize);
		lowmem_deathpending_timeout = jiffies + HZ;
		send_sig(SIGKILL, selected, 0);
		set_tsk_thread_flag(selected, TIF_MEMDIE);
		rem -= selected_tasksize;
		rcu_read_unlock();
		/* give the system time to free up the memory */
		msleep_interruptible(20);
	} else
		rcu_read_unlock();

	lowmem_print(4, "lowmem_shrink %lu, %x, return %d\n",
		     nr_to_scan, sc->gfp_mask, rem);
	mutex_unlock(&scan_mutex);
	return rem;
}

>>>>>>> cm-10.0
static struct shrinker lowmem_shrinker = {
	.shrink = lowmem_shrink,
	.seeks = DEFAULT_SEEKS * 16
};

static int __init lowmem_init(void)
{
<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
       extern void kgsl_register_shrinker(void (*shrink)(int largest, int threshold));

       kgsl_register_shrinker(lowmem_vm_shrinker);
#else
	task_free_register(&task_nb);
#endif//CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	register_shrinker(&lowmem_shrinker);
#ifdef CONFIG_MEMORY_HOTPLUG
	hotplug_memory_notifier(lmk_hotplug_callback, 0);
#endif
#ifdef CONFIG_HUAWEI_FEATURE_LOW_MEMORY_KILLER_STUB
    registerlowmem();
#endif
=======
	register_shrinker(&lowmem_shrinker);
>>>>>>> cm-10.0
	return 0;
}

static void __exit lowmem_exit(void)
{
<<<<<<< HEAD
#ifdef CONFIG_HUAWEI_VM_LOW_MEMORY_KILLER
	extern void kgsl_unregister_shrinker(void);
	kgsl_unregister_shrinker();
#else
	task_free_unregister(&task_nb);
#endif
	unregister_shrinker(&lowmem_shrinker);
#ifdef CONFIG_HUAWEI_FEATURE_LOW_MEMORY_KILLER_STUB
    unregisterlowmem();
#endif
}

module_param_named(cost, lowmem_shrinker.seeks, int, S_IRUGO | S_IWUSR);
module_param_array_named(adj, lowmem_adj, int, &lowmem_adj_size,
			 S_IRUGO | S_IWUSR);
=======
	unregister_shrinker(&lowmem_shrinker);
}

#ifdef CONFIG_ANDROID_LOW_MEMORY_KILLER_AUTODETECT_OOM_ADJ_VALUES
static int lowmem_oom_adj_to_oom_score_adj(int oom_adj)
{
	if (oom_adj == OOM_ADJUST_MAX)
		return OOM_SCORE_ADJ_MAX;
	else
		return (oom_adj * OOM_SCORE_ADJ_MAX) / -OOM_DISABLE;
}

static void lowmem_autodetect_oom_adj_values(void)
{
	int i;
	int oom_adj;
	int oom_score_adj;
	int array_size = ARRAY_SIZE(lowmem_adj);

	if (lowmem_adj_size < array_size)
		array_size = lowmem_adj_size;

	if (array_size <= 0)
		return;

	oom_adj = lowmem_adj[array_size - 1];
	if (oom_adj > OOM_ADJUST_MAX)
		return;

	oom_score_adj = lowmem_oom_adj_to_oom_score_adj(oom_adj);
	if (oom_score_adj <= OOM_ADJUST_MAX)
		return;

	lowmem_print(1, "lowmem_shrink: convert oom_adj to oom_score_adj:\n");
	for (i = 0; i < array_size; i++) {
		oom_adj = lowmem_adj[i];
		oom_score_adj = lowmem_oom_adj_to_oom_score_adj(oom_adj);
		lowmem_adj[i] = oom_score_adj;
		lowmem_print(1, "oom_adj %d => oom_score_adj %d\n",
			     oom_adj, oom_score_adj);
	}
}

static int lowmem_adj_array_set(const char *val, const struct kernel_param *kp)
{
	int ret;

	ret = param_array_ops.set(val, kp);

	/* HACK: Autodetect oom_adj values in lowmem_adj array */
	lowmem_autodetect_oom_adj_values();

	return ret;
}

static int lowmem_adj_array_get(char *buffer, const struct kernel_param *kp)
{
	return param_array_ops.get(buffer, kp);
}

static void lowmem_adj_array_free(void *arg)
{
	param_array_ops.free(arg);
}

static struct kernel_param_ops lowmem_adj_array_ops = {
	.set = lowmem_adj_array_set,
	.get = lowmem_adj_array_get,
	.free = lowmem_adj_array_free,
};

static const struct kparam_array __param_arr_adj = {
	.max = ARRAY_SIZE(lowmem_adj),
	.num = &lowmem_adj_size,
	.ops = &param_ops_int,
	.elemsize = sizeof(lowmem_adj[0]),
	.elem = lowmem_adj,
};
#endif

module_param_named(cost, lowmem_shrinker.seeks, int, S_IRUGO | S_IWUSR);
#ifdef CONFIG_ANDROID_LOW_MEMORY_KILLER_AUTODETECT_OOM_ADJ_VALUES
__module_param_call(MODULE_PARAM_PREFIX, adj,
		    &lowmem_adj_array_ops,
		    .arr = &__param_arr_adj,
		    S_IRUGO | S_IWUSR, -1);
__MODULE_PARM_TYPE(adj, "array of int");
#else
module_param_array_named(adj, lowmem_adj, int, &lowmem_adj_size,
			 S_IRUGO | S_IWUSR);
#endif
>>>>>>> cm-10.0
module_param_array_named(minfree, lowmem_minfree, uint, &lowmem_minfree_size,
			 S_IRUGO | S_IWUSR);
module_param_named(debug_level, lowmem_debug_level, uint, S_IRUGO | S_IWUSR);

module_init(lowmem_init);
module_exit(lowmem_exit);

MODULE_LICENSE("GPL");

