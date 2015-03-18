<<<<<<< HEAD
/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
=======
/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
>>>>>>> cm-10.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/cpufreq.h>
=======
>>>>>>> cm-10.0
#include <linux/mutex.h>
#include <linux/msm_tsens.h>
#include <linux/workqueue.h>
#include <linux/cpu.h>
<<<<<<< HEAD

#define DEF_TEMP_SENSOR      0
#define DEF_THERMAL_CHECK_MS 1000
#define DEF_ALLOWED_MAX_HIGH 60
#define DEF_ALLOWED_MAX_FREQ 918000

static int enabled;
static int thermal_mitigation_enabled;
static int allowed_max_high = DEF_ALLOWED_MAX_HIGH;
static int allowed_max_low = (DEF_ALLOWED_MAX_HIGH - 10);
static int allowed_max_freq = DEF_ALLOWED_MAX_FREQ;
static int check_interval_ms = DEF_THERMAL_CHECK_MS;

module_param(allowed_max_high, int, 0);
module_param(allowed_max_freq, int, 0);
module_param(check_interval_ms, int, 0);

static struct delayed_work check_temp_work;

static int update_cpu_max_freq(struct cpufreq_policy *cpu_policy,
			       int cpu, int max_freq)
{
	int ret = 0;

	if (!cpu_policy)
		return -EINVAL;

	cpufreq_verify_within_limits(cpu_policy,
				cpu_policy->min, max_freq);
	cpu_policy->user_policy.max = max_freq;

	ret = cpufreq_update_policy(cpu);
	if (!ret)
		pr_info("msm_thermal: Limiting core%d max frequency to %d\n",
			cpu, max_freq);
=======
#include <linux/cpufreq.h>
#include <linux/msm_tsens.h>
#include <linux/msm_thermal.h>
#include <mach/cpufreq.h>

static int enabled;
static struct msm_thermal_data msm_thermal_info;
static uint32_t limited_max_freq = MSM_CPUFREQ_NO_LIMIT;
static struct delayed_work check_temp_work;

static int update_cpu_max_freq(int cpu, uint32_t max_freq)
{
	int ret = 0;

	ret = msm_cpufreq_set_freq_limits(cpu, MSM_CPUFREQ_NO_LIMIT, max_freq);
	if (ret)
		return ret;

	ret = cpufreq_update_policy(cpu);
	if (ret)
		return ret;

	limited_max_freq = max_freq;
	if (max_freq != MSM_CPUFREQ_NO_LIMIT)
		pr_info("msm_thermal: Limiting cpu%d max frequency to %d\n",
				cpu, max_freq);
	else
		pr_info("msm_thermal: Max frequency reset for cpu%d\n", cpu);
>>>>>>> cm-10.0

	return ret;
}

static void check_temp(struct work_struct *work)
{
<<<<<<< HEAD
	struct cpufreq_policy *cpu_policy = NULL;
	struct tsens_device tsens_dev;
	unsigned long temp = 0;
	unsigned int max_freq = 0;
	int update_policy = 0;
	int cpu = 0;
	int ret = 0;

	tsens_dev.sensor_num = DEF_TEMP_SENSOR;
=======
	struct tsens_device tsens_dev;
	unsigned long temp = 0;
	uint32_t max_freq = limited_max_freq;
	int cpu = 0;
	int ret = 0;

	tsens_dev.sensor_num = msm_thermal_info.sensor_id;
>>>>>>> cm-10.0
	ret = tsens_get_temp(&tsens_dev, &temp);
	if (ret) {
		pr_debug("msm_thermal: Unable to read TSENS sensor %d\n",
				tsens_dev.sensor_num);
		goto reschedule;
	}

<<<<<<< HEAD
	for_each_possible_cpu(cpu) {
		update_policy = 0;
		cpu_policy = cpufreq_cpu_get(cpu);
		if (!cpu_policy) {
			pr_debug("msm_thermal: NULL policy on cpu %d\n", cpu);
			continue;
		}
		if (temp >= allowed_max_high) {
			if (cpu_policy->max > allowed_max_freq) {
				update_policy = 1;
				max_freq = allowed_max_freq;
			} else {
				pr_debug("msm_thermal: policy max for cpu %d "
					 "already < allowed_max_freq\n", cpu);
			}
		} else if (temp < allowed_max_low) {
			if (cpu_policy->max < cpu_policy->cpuinfo.max_freq) {
				max_freq = cpu_policy->cpuinfo.max_freq;
				update_policy = 1;
			} else {
				pr_debug("msm_thermal: policy max for cpu %d "
					 "already at max allowed\n", cpu);
			}
		}

		if (update_policy)
			update_cpu_max_freq(cpu_policy, cpu, max_freq);

		cpufreq_cpu_put(cpu_policy);
=======
	if (temp >= msm_thermal_info.limit_temp)
		max_freq = msm_thermal_info.limit_freq;
	else if (temp <
		msm_thermal_info.limit_temp - msm_thermal_info.temp_hysteresis)
		max_freq = MSM_CPUFREQ_NO_LIMIT;

	if (max_freq == limited_max_freq)
		goto reschedule;

	/* Update new limits */
	for_each_possible_cpu(cpu) {
		ret = update_cpu_max_freq(cpu, max_freq);
		if (ret)
			pr_debug("Unable to limit cpu%d max freq to %d\n",
					cpu, max_freq);
>>>>>>> cm-10.0
	}

reschedule:
	if (enabled)
		schedule_delayed_work(&check_temp_work,
<<<<<<< HEAD
				msecs_to_jiffies(check_interval_ms));
=======
				msecs_to_jiffies(msm_thermal_info.poll_ms));
>>>>>>> cm-10.0
}

static void disable_msm_thermal(void)
{
	int cpu = 0;
<<<<<<< HEAD
	struct cpufreq_policy *cpu_policy = NULL;
=======
>>>>>>> cm-10.0

	/* make sure check_temp is no longer running */
	cancel_delayed_work(&check_temp_work);
	flush_scheduled_work();

<<<<<<< HEAD
	for_each_possible_cpu(cpu) {
		cpu_policy = cpufreq_cpu_get(cpu);
		if (cpu_policy) {
			if (cpu_policy->max < cpu_policy->cpuinfo.max_freq)
				update_cpu_max_freq(cpu_policy, cpu,
						    cpu_policy->
						    cpuinfo.max_freq);
			cpufreq_cpu_put(cpu_policy);
		}
=======
	if (limited_max_freq == MSM_CPUFREQ_NO_LIMIT)
		return;

	for_each_possible_cpu(cpu) {
		update_cpu_max_freq(cpu, MSM_CPUFREQ_NO_LIMIT);
>>>>>>> cm-10.0
	}
}

static int set_enabled(const char *val, const struct kernel_param *kp)
{
	int ret = 0;

	ret = param_set_bool(val, kp);
	if (!enabled)
		disable_msm_thermal();
	else
		pr_info("msm_thermal: no action for enabled = %d\n", enabled);

	pr_info("msm_thermal: enabled = %d\n", enabled);

	return ret;
}

static struct kernel_param_ops module_ops = {
	.set = set_enabled,
	.get = param_get_bool,
};

module_param_cb(enabled, &module_ops, &enabled, 0644);
MODULE_PARM_DESC(enabled, "enforce thermal limit on cpu");

<<<<<<< HEAD
static int thermal_mitigation_enabled_set(const char *val, const struct kernel_param *kp)
{
	int ret = 0;
	ret = param_set_bool(val, kp);
	pr_info("thermal_mitigation: enabled = %d\n", thermal_mitigation_enabled);
	return ret;
}
static struct kernel_param_ops thermal_mitigation_enabled_ops = {
	.set = thermal_mitigation_enabled_set,
	.get = param_get_bool,
};
module_param_cb(thermal_mitigation_enabled, &thermal_mitigation_enabled_ops, &thermal_mitigation_enabled, 0644);
MODULE_PARM_DESC(thermal_mitigation_enabled, "whether allowed to execute CPU-freq adjustment of thermal mitigation");

static int __init msm_thermal_init(void)
{
	int ret = 0;

	enabled = 1;
	thermal_mitigation_enabled = 0;
	INIT_DELAYED_WORK(&check_temp_work, check_temp);

=======
int __init msm_thermal_init(struct msm_thermal_data *pdata)
{
	int ret = 0;

	BUG_ON(!pdata);
	BUG_ON(pdata->sensor_id >= TSENS_MAX_SENSORS);
	memcpy(&msm_thermal_info, pdata, sizeof(struct msm_thermal_data));

	enabled = 1;
	INIT_DELAYED_WORK(&check_temp_work, check_temp);
>>>>>>> cm-10.0
	schedule_delayed_work(&check_temp_work, 0);

	return ret;
}
<<<<<<< HEAD
fs_initcall(msm_thermal_init);

=======
>>>>>>> cm-10.0
