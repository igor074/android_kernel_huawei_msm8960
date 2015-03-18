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
 */

#include <linux/slab.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/diagchar.h>
#include <linux/sched.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/workqueue.h>
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
=======
#include <linux/ratelimit.h>
#include <linux/workqueue.h>
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
#include <linux/smux.h>
>>>>>>> cm-10.0
#include <asm/current.h>
#ifdef CONFIG_DIAG_OVER_USB
#include <mach/usbdiag.h>
#endif
#include "diagchar_hdlc.h"
#include "diagmem.h"
#include "diagchar.h"
#include "diagfwd.h"
#include "diagfwd_hsic.h"
<<<<<<< HEAD

static void diag_read_hsic_work_fn(struct work_struct *work)
{
=======
#include "diagfwd_smux.h"

#define N_MDM_WRITE	8
#define N_MDM_READ	1

#define READ_HSIC_BUF_SIZE 2048

#define NUM_HSIC_BUF_TBL_ENTRIES N_MDM_WRITE

static void diag_read_hsic_work_fn(struct work_struct *work)
{
	unsigned char *buf_in_hsic = NULL;
	int num_reads_submitted = 0;
	int err = 0;
	int write_ptrs_available;

>>>>>>> cm-10.0
	if (!driver->hsic_ch) {
		pr_err("DIAG in %s: driver->hsic_ch == 0\n", __func__);
		return;
	}

	/*
<<<<<<< HEAD
	 * If there is no hsic data being read from the hsic and there
	 * is no hsic data being written to the usb mdm channel
	 */
	if (!driver->in_busy_hsic_read && !driver->in_busy_hsic_write_on_mdm) {
		/*
		 * Initiate the read from the hsic.  The hsic read is
		 * asynchronous.  Once the read is complete the read
		 * callback function will be called.
		 */
		int err;
		driver->in_busy_hsic_read = 1;
		APPEND_DEBUG('i');
		err = diag_bridge_read((char *)driver->buf_in_hsic,
					IN_BUF_SIZE);
		if (err) {
			pr_err("DIAG: Error initiating HSIC read, err: %d\n",
				err);
			/*
			 * If the error is recoverable, then clear
			 * the read flag, so we will resubmit a
			 * read on the next frame.  Otherwise, don't
			 * resubmit a read on the next frame.
			 */
			if ((-ESHUTDOWN) != err)
				driver->in_busy_hsic_read = 0;
		}
	}

	/*
	 * If for some reason there was no hsic data, set up
	 * the next read
	 */
	if (!driver->in_busy_hsic_read)
		queue_work(driver->diag_hsic_wq, &driver->diag_read_hsic_work);
=======
	 * Determine the current number of available buffers for writing after
	 * reading from the HSIC has completed.
	 */
	if (driver->logging_mode == MEMORY_DEVICE_MODE)
		write_ptrs_available = driver->poolsize_hsic_write -
					driver->num_hsic_buf_tbl_entries;
	else
		write_ptrs_available = driver->poolsize_hsic_write -
					driver->count_hsic_write_pool;

	/*
	 * Queue up a read on the HSIC for all available buffers in the
	 * pool, exhausting the pool.
	 */
	do {
		/*
		 * If no more write buffers are available,
		 * stop queuing reads
		 */
		if (write_ptrs_available <= 0)
			break;

		write_ptrs_available--;

		buf_in_hsic = diagmem_alloc(driver, READ_HSIC_BUF_SIZE,
							POOL_TYPE_HSIC);
		if (buf_in_hsic) {
			/*
			 * Initiate the read from the hsic.  The hsic read is
			 * asynchronous.  Once the read is complete the read
			 * callback function will be called.
			 */
			pr_debug("diag: read from HSIC\n");
			num_reads_submitted++;
			err = diag_bridge_read((char *)buf_in_hsic,
							READ_HSIC_BUF_SIZE);
			if (err) {
				num_reads_submitted--;

				/* Return the buffer to the pool */
				diagmem_free(driver, buf_in_hsic,
						POOL_TYPE_HSIC);

				pr_err_ratelimited("diag: Error initiating HSIC read, err: %d\n",
					err);
				/*
				 * An error occurred, discontinue queuing
				 * reads
				 */
				break;
			}
		}
	} while (buf_in_hsic);

	/*
	 * If there are no buffers available or for some reason there
	 * was no hsic data, and if no unrecoverable error occurred
	 * (-ENODEV is an unrecoverable error), then set up the next read
	 */
	if ((num_reads_submitted == 0) && (err != -ENODEV))
		queue_work(driver->diag_bridge_wq,
				 &driver->diag_read_hsic_work);
>>>>>>> cm-10.0
}

static void diag_hsic_read_complete_callback(void *ctxt, char *buf,
					int buf_size, int actual_size)
{
<<<<<<< HEAD
	/* The read of the data from the HSIC bridge is complete */
	driver->in_busy_hsic_read = 0;

	if (!driver->hsic_ch) {
		pr_err("DIAG in %s: driver->hsic_ch == 0\n", __func__);
		return;
	}

	APPEND_DEBUG('j');
	if (actual_size > 0) {
		if (!buf) {
			pr_err("Out of diagmem for HSIC\n");
		} else {
			driver->write_ptr_mdm->length = actual_size;
			/*
			 * Set flag to denote hsic data is currently
			 * being written to the usb mdm channel.
			 * driver->buf_in_hsic was given to
			 * diag_bridge_read(), so buf here should be
			 * driver->buf_in_hsic
			 */
			driver->in_busy_hsic_write_on_mdm = 1;
			diag_device_write((void *)buf, HSIC_DATA,
						driver->write_ptr_mdm);
		}
	} else {
		pr_err("DIAG in %s: actual_size: %d\n", __func__, actual_size);
=======
	int err = -2;

	if (!driver->hsic_ch) {
		/*
		 * The hsic channel is closed. Return the buffer to
		 * the pool.  Do not send it on.
		 */
		diagmem_free(driver, buf, POOL_TYPE_HSIC);
		pr_debug("diag: In %s: driver->hsic_ch == 0, actual_size: %d\n",
			__func__, actual_size);
		return;
	}

	/* Note that zero length is valid and still needs to be sent */
	if (actual_size >= 0) {
		if (!buf) {
			pr_err("diag: Out of diagmem for HSIC\n");
		} else {
			/*
			 * Send data in buf to be written on the
			 * appropriate device, e.g. USB MDM channel
			 */
			driver->write_len_mdm = actual_size;
			err = diag_device_write((void *)buf, HSIC_DATA, NULL);
			/* If an error, return buffer to the pool */
			if (err) {
				diagmem_free(driver, buf, POOL_TYPE_HSIC);
				pr_err("diag: In %s, error calling diag_device_write, err: %d\n",
					__func__, err);
			}
		}
	} else {
		/*
		 * The buffer has an error status associated with it. Do not
		 * pass it on. Note that -ENOENT is sent when the diag bridge
		 * is closed.
		 */
		diagmem_free(driver, buf, POOL_TYPE_HSIC);
		pr_debug("diag: In %s: error status: %d\n", __func__,
			actual_size);
>>>>>>> cm-10.0
	}

	/*
	 * If for some reason there was no hsic data to write to the
	 * mdm channel, set up another read
	 */
<<<<<<< HEAD
	if (!driver->in_busy_hsic_write_on_mdm)
		queue_work(driver->diag_hsic_wq, &driver->diag_read_hsic_work);
=======
	if (err &&
		((driver->logging_mode == MEMORY_DEVICE_MODE) ||
		(driver->usb_mdm_connected && !driver->hsic_suspend))) {
		queue_work(driver->diag_bridge_wq,
				 &driver->diag_read_hsic_work);
	}
>>>>>>> cm-10.0
}

static void diag_hsic_write_complete_callback(void *ctxt, char *buf,
					int buf_size, int actual_size)
{
	/* The write of the data to the HSIC bridge is complete */
	driver->in_busy_hsic_write = 0;

	if (!driver->hsic_ch) {
		pr_err("DIAG in %s: driver->hsic_ch == 0\n", __func__);
		return;
	}

	if (actual_size < 0)
		pr_err("DIAG in %s: actual_size: %d\n", __func__, actual_size);

<<<<<<< HEAD
	queue_work(driver->diag_hsic_wq, &driver->diag_read_mdm_work);
=======
	if (driver->usb_mdm_connected)
		queue_work(driver->diag_bridge_wq, &driver->diag_read_mdm_work);
}

static int diag_hsic_suspend(void *ctxt)
{
	pr_debug("diag: hsic_suspend\n");

	/* Don't allow suspend if a write in the HSIC is in progress */
	if (driver->in_busy_hsic_write)
		return -EBUSY;

	/* Don't allow suspend if in MEMORY_DEVICE_MODE */
	if (driver->logging_mode == MEMORY_DEVICE_MODE)
		return -EBUSY;

	driver->hsic_suspend = 1;

	return 0;
}

static void diag_hsic_resume(void *ctxt)
{
	pr_debug("diag: hsic_resume\n");
	driver->hsic_suspend = 0;

	if ((driver->logging_mode == MEMORY_DEVICE_MODE) ||
				(driver->usb_mdm_connected))
		queue_work(driver->diag_bridge_wq,
			 &driver->diag_read_hsic_work);
>>>>>>> cm-10.0
}

static struct diag_bridge_ops hsic_diag_bridge_ops = {
	.ctxt = NULL,
	.read_complete_cb = diag_hsic_read_complete_callback,
	.write_complete_cb = diag_hsic_write_complete_callback,
<<<<<<< HEAD
=======
	.suspend = diag_hsic_suspend,
	.resume = diag_hsic_resume,
>>>>>>> cm-10.0
};

static int diag_hsic_close(void)
{
	if (driver->hsic_device_enabled) {
		driver->hsic_ch = 0;
		if (driver->hsic_device_opened) {
			driver->hsic_device_opened = 0;
			diag_bridge_close();
		}
<<<<<<< HEAD
		pr_debug("DIAG in %s: closed successfully\n", __func__);
	} else {
		pr_debug("DIAG in %s: already closed\n", __func__);
=======
		pr_debug("diag: in %s: closed successfully\n", __func__);
	} else {
		pr_debug("diag: in %s: already closed\n", __func__);
	}

	return 0;
}

/* diagfwd_cancel_hsic is called to cancel outstanding read/writes */
int diagfwd_cancel_hsic(void)
{
	int err;

	if (driver->hsic_device_enabled) {
		if (driver->hsic_device_opened) {
			driver->hsic_ch = 0;
			driver->hsic_device_opened = 0;
			diag_bridge_close();
			err = diag_bridge_open(&hsic_diag_bridge_ops);
			if (err) {
				pr_err("diag: HSIC channel open error: %d\n",
					err);
			} else {
				pr_debug("diag: opened HSIC channel\n");
				driver->hsic_device_opened = 1;
				driver->hsic_ch = 1;
			}
		}
>>>>>>> cm-10.0
	}

	return 0;
}

<<<<<<< HEAD
/* diagfwd_connect_hsic is called when the USB mdm channel is connected */
static int diagfwd_connect_hsic(void)
{
	int err;

	pr_debug("DIAG in %s\n", __func__);

	err = usb_diag_alloc_req(driver->mdm_ch, N_MDM_WRITE, N_MDM_READ);
	if (err)
		pr_err("DIAG: unable to alloc USB req on mdm ch err:%d\n", err);

	driver->usb_mdm_connected = 1;
	driver->in_busy_hsic_write_on_mdm = 0;
	driver->in_busy_hsic_read_on_mdm = 0;
	driver->in_busy_hsic_write = 0;
	driver->in_busy_hsic_read = 0;
=======
/* diagfwd_connect_bridge is called when the USB mdm channel is connected */
int diagfwd_connect_bridge(int process_cable)
{
	int err;

	pr_debug("diag: in %s\n", __func__);

	/* If the usb cable is being connected */
	if (process_cable) {
		err = usb_diag_alloc_req(driver->mdm_ch, N_MDM_WRITE,
			N_MDM_READ);
		if (err)
			pr_err("diag: unable to alloc USB req on mdm"
				" ch err:%d\n", err);

		driver->usb_mdm_connected = 1;
	}

	if (driver->hsic_device_enabled) {
		driver->in_busy_hsic_read_on_device = 0;
		driver->in_busy_hsic_write = 0;
	} else if (driver->diag_smux_enabled) {
		driver->in_busy_smux = 0;
		diagfwd_connect_smux();
		return 0;
	}
>>>>>>> cm-10.0

	/* If the hsic (diag_bridge) platform device is not open */
	if (driver->hsic_device_enabled) {
		if (!driver->hsic_device_opened) {
			err = diag_bridge_open(&hsic_diag_bridge_ops);
			if (err) {
<<<<<<< HEAD
				pr_err("DIAG: HSIC channel open error: %d\n",
					err);
			} else {
				pr_info("DIAG: opened HSIC channel\n");
				driver->hsic_device_opened = 1;
			}
		} else {
			pr_info("DIAG: HSIC channel already open\n");
=======
				pr_err("diag: HSIC channel open error: %d\n",
					err);
			} else {
				pr_debug("diag: opened HSIC channel\n");
				driver->hsic_device_opened = 1;
			}
		} else {
			pr_debug("diag: HSIC channel already open\n");
>>>>>>> cm-10.0
		}

		/*
		 * Turn on communication over usb mdm and hsic, if the hsic
		 * device driver is enabled and opened
		 */
		if (driver->hsic_device_opened)
			driver->hsic_ch = 1;

		/* Poll USB mdm channel to check for data */
<<<<<<< HEAD
		queue_work(driver->diag_hsic_wq, &driver->diag_read_mdm_work);

		/* Poll HSIC channel to check for data */
		queue_work(driver->diag_hsic_wq, &driver->diag_read_hsic_work);
	} else {
		/* The hsic device driver has not yet been enabled */
		pr_info("DIAG: HSIC channel not yet enabled\n");
=======
		if (driver->logging_mode == USB_MODE)
			queue_work(driver->diag_bridge_wq,
					&driver->diag_read_mdm_work);

		/* Poll HSIC channel to check for data */
		queue_work(driver->diag_bridge_wq,
				 &driver->diag_read_hsic_work);
	} else {
		/* The hsic device driver has not yet been enabled */
		pr_info("diag: HSIC channel not yet enabled\n");
>>>>>>> cm-10.0
	}

	return 0;
}

/*
<<<<<<< HEAD
 * diagfwd_disconnect_hsic is called when the USB mdm channel
 * is disconnected
 */
static int diagfwd_disconnect_hsic(void)
{
	pr_debug("DIAG in %s\n", __func__);

	driver->usb_mdm_connected = 0;
	usb_diag_free_req(driver->mdm_ch);
	driver->in_busy_hsic_write_on_mdm = 1;
	driver->in_busy_hsic_read_on_mdm = 1;
	driver->in_busy_hsic_write = 1;
	driver->in_busy_hsic_read = 1;

	/* Turn off communication over usb mdm and hsic */
	driver->hsic_ch = 0;

=======
 * diagfwd_disconnect_bridge is called when the USB mdm channel
 * is disconnected
 */
int diagfwd_disconnect_bridge(int process_cable)
{
	pr_debug("diag: In %s, process_cable: %d\n", __func__, process_cable);

	/* If the usb cable is being disconnected */
	if (process_cable) {
		driver->usb_mdm_connected = 0;
		usb_diag_free_req(driver->mdm_ch);
	}

	if (driver->logging_mode != MEMORY_DEVICE_MODE) {
		if (driver->hsic_device_enabled) {
			driver->in_busy_hsic_read_on_device = 1;
			driver->in_busy_hsic_write = 1;
			/* Turn off communication over usb mdm and hsic */
			return diag_hsic_close();
		} else if (driver->diag_smux_enabled) {
			driver->in_busy_smux = 1;
			driver->lcid = LCID_INVALID;
			/* Turn off communication over usb mdm and smux */
			msm_smux_close(LCID_VALID);
		}
	}
>>>>>>> cm-10.0
	return 0;
}

/*
 * diagfwd_write_complete_hsic is called after the asynchronous
 * usb_diag_write() on mdm channel is complete
 */
<<<<<<< HEAD
static int diagfwd_write_complete_hsic(void)
{
	/*
	 * Clear flag to denote that the write of the hsic data on the
	 * usb mdm channel is complete
	 */
	driver->in_busy_hsic_write_on_mdm = 0;

	if (!driver->hsic_ch) {
		pr_err("DIAG in %s: driver->hsic_ch == 0\n", __func__);
		return 0;
	}

	APPEND_DEBUG('q');

	/* Read data from the hsic */
	queue_work(driver->diag_hsic_wq, &driver->diag_read_hsic_work);
=======
int diagfwd_write_complete_hsic(struct diag_request *diag_write_ptr)
{
	unsigned char *buf = (diag_write_ptr) ? diag_write_ptr->buf : NULL;

	if (buf) {
		/* Return buffers to their pools */
		diagmem_free(driver, (unsigned char *)buf, POOL_TYPE_HSIC);
		diagmem_free(driver, (unsigned char *)diag_write_ptr,
							POOL_TYPE_HSIC_WRITE);
	}

	if (!driver->hsic_ch) {
		pr_err("diag: In %s: driver->hsic_ch == 0\n", __func__);
		return 0;
	}

	/* Read data from the hsic */
	queue_work(driver->diag_bridge_wq, &driver->diag_read_hsic_work);
>>>>>>> cm-10.0

	return 0;
}

/* Called after the asychronous usb_diag_read() on mdm channel is complete */
<<<<<<< HEAD
static int diagfwd_read_complete_hsic(struct diag_request *diag_read_ptr)
{
	/* The read of the usb driver on the mdm (not hsic) has completed */
	driver->in_busy_hsic_read_on_mdm = 0;
	driver->read_len_mdm = diag_read_ptr->actual;

=======
static int diagfwd_read_complete_bridge(struct diag_request *diag_read_ptr)
{
	/* The read of the usb driver on the mdm (not hsic) has completed */
	driver->in_busy_hsic_read_on_device = 0;
	driver->read_len_mdm = diag_read_ptr->actual;

	if (driver->diag_smux_enabled) {
		diagfwd_read_complete_smux();
		return 0;
	}
	/* If SMUX not enabled, check for HSIC */
>>>>>>> cm-10.0
	if (!driver->hsic_ch) {
		pr_err("DIAG in %s: driver->hsic_ch == 0\n", __func__);
		return 0;
	}

	/*
	 * The read of the usb driver on the mdm channel has completed.
	 * If there is no write on the hsic in progress, check if the
	 * read has data to pass on to the hsic. If so, pass the usb
	 * mdm data on to the hsic.
	 */
	if (!driver->in_busy_hsic_write && driver->usb_buf_mdm_out &&
		(driver->read_len_mdm > 0)) {

		/*
		 * Initiate the hsic write. The hsic write is
		 * asynchronous. When complete the write
		 * complete callback function will be called
		 */
		int err;
		driver->in_busy_hsic_write = 1;
		err = diag_bridge_write(driver->usb_buf_mdm_out,
					driver->read_len_mdm);
		if (err) {
<<<<<<< HEAD
			pr_err("DIAG: mdm data on hsic write err: %d\n", err);
=======
			pr_err_ratelimited("diag: mdm data on hsic write err: %d\n",
					err);
>>>>>>> cm-10.0
			/*
			 * If the error is recoverable, then clear
			 * the write flag, so we will resubmit a
			 * write on the next frame.  Otherwise, don't
			 * resubmit a write on the next frame.
			 */
<<<<<<< HEAD
			if ((-ESHUTDOWN) != err)
=======
			if ((-ENODEV) != err)
>>>>>>> cm-10.0
				driver->in_busy_hsic_write = 0;
		}
	}

	/*
	 * If there is no write of the usb mdm data on the
	 * hsic channel
	 */
	if (!driver->in_busy_hsic_write)
<<<<<<< HEAD
		queue_work(driver->diag_hsic_wq, &driver->diag_read_mdm_work);
=======
		queue_work(driver->diag_bridge_wq, &driver->diag_read_mdm_work);
>>>>>>> cm-10.0

	return 0;
}

<<<<<<< HEAD
static void diagfwd_hsic_notifier(void *priv, unsigned event,
=======
static void diagfwd_bridge_notifier(void *priv, unsigned event,
>>>>>>> cm-10.0
					struct diag_request *d_req)
{
	switch (event) {
	case USB_DIAG_CONNECT:
<<<<<<< HEAD
		diagfwd_connect_hsic();
		break;
	case USB_DIAG_DISCONNECT:
		diagfwd_disconnect_hsic();
		break;
	case USB_DIAG_READ_DONE:
		diagfwd_read_complete_hsic(d_req);
		break;
	case USB_DIAG_WRITE_DONE:
		diagfwd_write_complete_hsic();
		break;
	default:
		pr_err("DIAG in %s: Unknown event from USB diag:%u\n",
=======
		diagfwd_connect_bridge(1);
		break;
	case USB_DIAG_DISCONNECT:
		queue_work(driver->diag_bridge_wq,
			 &driver->diag_disconnect_work);
		break;
	case USB_DIAG_READ_DONE:
		queue_work(driver->diag_bridge_wq,
				&driver->diag_usb_read_complete_work);
		break;
	case USB_DIAG_WRITE_DONE:
		if (driver->hsic_device_enabled)
			diagfwd_write_complete_hsic(d_req);
		else if (driver->diag_smux_enabled)
			diagfwd_write_complete_smux();
		break;
	default:
		pr_err("diag: in %s: Unknown event from USB diag:%u\n",
>>>>>>> cm-10.0
			__func__, event);
		break;
	}
}

<<<<<<< HEAD
static void diag_read_mdm_work_fn(struct work_struct *work)
{
=======
static void diag_usb_read_complete_fn(struct work_struct *w)
{
	diagfwd_read_complete_bridge(driver->usb_read_mdm_ptr);
}

static void diag_disconnect_work_fn(struct work_struct *w)
{
	diagfwd_disconnect_bridge(1);
}

static void diag_read_mdm_work_fn(struct work_struct *work)
{
	int ret;
	if (driver->diag_smux_enabled) {
		if (driver->lcid && driver->usb_buf_mdm_out &&
					 (driver->read_len_mdm > 0)) {
			ret = msm_smux_write(driver->lcid,  NULL,
		 driver->usb_buf_mdm_out, driver->read_len_mdm);
			if (ret)
				pr_err("diag: writing to SMUX ch, r = %d,"
					"lcid = %d\n", ret, driver->lcid);
		}
		driver->usb_read_mdm_ptr->buf = driver->usb_buf_mdm_out;
		driver->usb_read_mdm_ptr->length = USB_MAX_OUT_BUF;
		usb_diag_read(driver->mdm_ch, driver->usb_read_mdm_ptr);
		return;
	}

	/* if SMUX not enabled, check for HSIC */
>>>>>>> cm-10.0
	if (!driver->hsic_ch) {
		pr_err("DIAG in %s: driver->hsic_ch == 0\n", __func__);
		return;
	}

	/*
	 * If there is no data being read from the usb mdm channel
	 * and there is no mdm channel data currently being written
	 * to the hsic
	 */
<<<<<<< HEAD
	if (!driver->in_busy_hsic_read_on_mdm && !driver->in_busy_hsic_write) {
		APPEND_DEBUG('x');

		/* Setup the next read from usb mdm channel */
		driver->in_busy_hsic_read_on_mdm = 1;
=======
	if (!driver->in_busy_hsic_read_on_device &&
				 !driver->in_busy_hsic_write) {
		APPEND_DEBUG('x');

		/* Setup the next read from usb mdm channel */
		driver->in_busy_hsic_read_on_device = 1;
>>>>>>> cm-10.0
		driver->usb_read_mdm_ptr->buf = driver->usb_buf_mdm_out;
		driver->usb_read_mdm_ptr->length = USB_MAX_OUT_BUF;
		usb_diag_read(driver->mdm_ch, driver->usb_read_mdm_ptr);
		APPEND_DEBUG('y');
	}

	/*
	 * If for some reason there was no mdm channel read initiated,
	 * queue up the reading of data from the mdm channel
	 */
<<<<<<< HEAD
	if (!driver->in_busy_hsic_read_on_mdm)
		queue_work(driver->diag_hsic_wq, &driver->diag_read_mdm_work);
}

int diag_hsic_enable(void)
{
	pr_debug("DIAG in %s\n", __func__);

	driver->read_len_mdm = 0;
	if (driver->buf_in_hsic == NULL)
		driver->buf_in_hsic = kzalloc(IN_BUF_SIZE, GFP_KERNEL);
	if (driver->buf_in_hsic == NULL)
		goto err;
	if (driver->usb_buf_mdm_out  == NULL)
		driver->usb_buf_mdm_out = kzalloc(USB_MAX_OUT_BUF, GFP_KERNEL);
	if (driver->usb_buf_mdm_out == NULL)
		goto err;
	if (driver->write_ptr_mdm == NULL)
		driver->write_ptr_mdm = kzalloc(
		sizeof(struct diag_request), GFP_KERNEL);
	if (driver->write_ptr_mdm == NULL)
		goto err;
	if (driver->usb_read_mdm_ptr == NULL)
		driver->usb_read_mdm_ptr = kzalloc(
		sizeof(struct diag_request), GFP_KERNEL);
	if (driver->usb_read_mdm_ptr == NULL)
		goto err;
	driver->diag_hsic_wq = create_singlethread_workqueue("diag_hsic_wq");
#ifdef CONFIG_DIAG_OVER_USB
	INIT_WORK(&(driver->diag_read_mdm_work), diag_read_mdm_work_fn);
#endif
	INIT_WORK(&(driver->diag_read_hsic_work), diag_read_hsic_work_fn);

	driver->hsic_device_enabled = 1;

	return 0;
err:
	pr_err("DIAG could not initialize buf for HSIC\n");
	kfree(driver->buf_in_hsic);
	kfree(driver->usb_buf_mdm_out);
	kfree(driver->write_ptr_mdm);
	kfree(driver->usb_read_mdm_ptr);
	if (driver->diag_hsic_wq)
		destroy_workqueue(driver->diag_hsic_wq);

	return -ENOMEM;
=======
	if (!driver->in_busy_hsic_read_on_device)
		queue_work(driver->diag_bridge_wq,
			 &driver->diag_read_mdm_work);
>>>>>>> cm-10.0
}

static int diag_hsic_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	int err;

	if (!driver->hsic_device_enabled) {
		err = diag_hsic_enable();
		if (err) {
			pr_err("DIAG could not enable HSIC, err: %d\n", err);
			return err;
		}
	}

	/* The hsic (diag_bridge) platform device driver is enabled */
	err = diag_bridge_open(&hsic_diag_bridge_ops);
	if (err) {
		pr_err("DIAG could not open HSIC channel, err: %d\n", err);
		driver->hsic_device_opened = 0;
		return err;
	}

	pr_info("DIAG opened HSIC channel\n");
	driver->hsic_device_opened = 1;

	/*
	 * The probe function was called after the usb was connected
	 * on the legacy channel. Communication over usb mdm and hsic
	 * needs to be turned on.
	 */
	if (driver->usb_connected) {
		driver->hsic_ch = 1;
		driver->in_busy_hsic_write_on_mdm = 0;
		driver->in_busy_hsic_read_on_mdm = 0;
		driver->in_busy_hsic_write = 0;
		driver->in_busy_hsic_read = 0;

		/* Poll USB mdm channel to check for data */
		queue_work(driver->diag_hsic_wq, &driver->diag_read_mdm_work);

		/* Poll HSIC channel to check for data */
		queue_work(driver->diag_hsic_wq, &driver->diag_read_hsic_work);
=======
	int err = 0;
	pr_debug("diag: in %s\n", __func__);
	if (!driver->hsic_device_enabled) {
		diagmem_hsic_init(driver);
		INIT_WORK(&(driver->diag_read_hsic_work),
					 diag_read_hsic_work_fn);
		driver->hsic_device_enabled = 1;
	}

	/*
	 * The probe function was called after the usb was connected
	 * on the legacy channel OR ODL is turned on. Communication over usb
	 * mdm and hsic needs to be turned on.
	 */
	if (driver->usb_mdm_connected || (driver->logging_mode ==
							 MEMORY_DEVICE_MODE)) {
		/* The hsic (diag_bridge) platform device driver is enabled */
		err = diag_bridge_open(&hsic_diag_bridge_ops);
		if (err) {
			pr_err("diag: could not open HSIC, err: %d\n", err);
			driver->hsic_device_opened = 0;
			return err;
		}

		pr_info("diag: opened HSIC channel\n");
		driver->hsic_device_opened = 1;
		driver->hsic_ch = 1;

		driver->in_busy_hsic_read_on_device = 0;
		driver->in_busy_hsic_write = 0;

		if (driver->usb_mdm_connected) {
			/* Poll USB mdm channel to check for data */
			queue_work(driver->diag_bridge_wq,
					 &driver->diag_read_mdm_work);
		}

		/* Poll HSIC channel to check for data */
		queue_work(driver->diag_bridge_wq,
				 &driver->diag_read_hsic_work);
>>>>>>> cm-10.0
	}

	return err;
}

static int diag_hsic_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	pr_info("DIAG: %s called\n", __func__);
=======
	pr_debug("diag: %s called\n", __func__);
>>>>>>> cm-10.0
	diag_hsic_close();
	return 0;
}

static int diagfwd_hsic_runtime_suspend(struct device *dev)
{
	dev_dbg(dev, "pm_runtime: suspending...\n");
	return 0;
}

static int diagfwd_hsic_runtime_resume(struct device *dev)
{
	dev_dbg(dev, "pm_runtime: resuming...\n");
	return 0;
}

static const struct dev_pm_ops diagfwd_hsic_dev_pm_ops = {
	.runtime_suspend = diagfwd_hsic_runtime_suspend,
	.runtime_resume = diagfwd_hsic_runtime_resume,
};

static struct platform_driver msm_hsic_ch_driver = {
	.probe = diag_hsic_probe,
	.remove = diag_hsic_remove,
	.driver = {
		   .name = "diag_bridge",
		   .owner = THIS_MODULE,
		   .pm   = &diagfwd_hsic_dev_pm_ops,
		   },
};

<<<<<<< HEAD

void __init diagfwd_hsic_init(void)
{
	int ret;

	pr_debug("DIAG in %s\n", __func__);

#ifdef CONFIG_DIAG_OVER_USB
	driver->mdm_ch = usb_diag_open(DIAG_MDM, driver, diagfwd_hsic_notifier);
	if (IS_ERR(driver->mdm_ch)) {
		pr_err("DIAG Unable to open USB diag MDM channel\n");
		goto err;
	}
#endif
	ret = platform_driver_register(&msm_hsic_ch_driver);
	if (ret)
		pr_err("DIAG could not register HSIC device, ret: %d\n", ret);
	else
		driver->hsic_initialized = 1;

	return;
err:
	pr_err("DIAG could not initialize for HSIC execution\n");
}

void __exit diagfwd_hsic_exit(void)
{
	pr_debug("DIAG in %s\n", __func__);

	if (driver->hsic_initialized)
		diag_hsic_close();

#ifdef CONFIG_DIAG_OVER_USB
	if (driver->usb_mdm_connected)
		usb_diag_free_req(driver->mdm_ch);
#endif
	platform_driver_unregister(&msm_hsic_ch_driver);
#ifdef CONFIG_DIAG_OVER_USB
	usb_diag_close(driver->mdm_ch);
#endif
	kfree(driver->buf_in_hsic);
	kfree(driver->usb_buf_mdm_out);
	kfree(driver->write_ptr_mdm);
	kfree(driver->usb_read_mdm_ptr);
	destroy_workqueue(driver->diag_hsic_wq);

	driver->hsic_device_enabled = 0;
=======
void diagfwd_bridge_init(void)
{
	int ret;

	pr_debug("diag: in %s\n", __func__);
	driver->diag_bridge_wq = create_singlethread_workqueue(
							"diag_bridge_wq");
	driver->read_len_mdm = 0;
	driver->write_len_mdm = 0;
	driver->num_hsic_buf_tbl_entries = 0;
	if (driver->usb_buf_mdm_out  == NULL)
		driver->usb_buf_mdm_out = kzalloc(USB_MAX_OUT_BUF,
							 GFP_KERNEL);
	if (driver->usb_buf_mdm_out == NULL)
		goto err;
	/* Only used by smux move to smux probe function */
	if (driver->write_ptr_mdm == NULL)
		driver->write_ptr_mdm = kzalloc(
		sizeof(struct diag_request), GFP_KERNEL);
	if (driver->write_ptr_mdm == NULL)
		goto err;
	if (driver->usb_read_mdm_ptr == NULL)
		driver->usb_read_mdm_ptr = kzalloc(
		sizeof(struct diag_request), GFP_KERNEL);
	if (driver->usb_read_mdm_ptr == NULL)
		goto err;

	if (driver->hsic_buf_tbl == NULL)
		driver->hsic_buf_tbl = kzalloc(NUM_HSIC_BUF_TBL_ENTRIES *
				sizeof(struct diag_write_device), GFP_KERNEL);
	if (driver->hsic_buf_tbl == NULL)
		goto err;

	driver->count_hsic_pool = 0;
	driver->count_hsic_write_pool = 0;

	driver->itemsize_hsic = READ_HSIC_BUF_SIZE;
	driver->poolsize_hsic = N_MDM_WRITE;
	driver->itemsize_hsic_write = sizeof(struct diag_request);
	driver->poolsize_hsic_write = N_MDM_WRITE;

#ifdef CONFIG_DIAG_OVER_USB
	INIT_WORK(&(driver->diag_read_mdm_work), diag_read_mdm_work_fn);
#endif
	INIT_WORK(&(driver->diag_disconnect_work), diag_disconnect_work_fn);
	INIT_WORK(&(driver->diag_usb_read_complete_work),
			diag_usb_read_complete_fn);
#ifdef CONFIG_DIAG_OVER_USB
	driver->mdm_ch = usb_diag_open(DIAG_MDM, driver,
						 diagfwd_bridge_notifier);
	if (IS_ERR(driver->mdm_ch)) {
		pr_err("diag: Unable to open USB diag MDM channel\n");
		goto err;
	}
#endif
	/* register HSIC device */
	ret = platform_driver_register(&msm_hsic_ch_driver);
	if (ret)
		pr_err("diag: could not register HSIC device, ret: %d\n", ret);
	/* register SMUX device */
	ret = platform_driver_register(&msm_diagfwd_smux_driver);
	if (ret)
		pr_err("diag: could not register SMUX device, ret: %d\n", ret);

	return;
err:
	pr_err("diag: Could not initialize for bridge forwarding\n");
	kfree(driver->usb_buf_mdm_out);
	kfree(driver->hsic_buf_tbl);
	kfree(driver->write_ptr_mdm);
	kfree(driver->usb_read_mdm_ptr);
	if (driver->diag_bridge_wq)
		destroy_workqueue(driver->diag_bridge_wq);

	return;
}

void diagfwd_bridge_exit(void)
{
	pr_debug("diag: in %s\n", __func__);

	if (driver->hsic_device_enabled) {
		diag_hsic_close();
		driver->hsic_device_enabled = 0;
		diagmem_exit(driver, POOL_TYPE_ALL);
	}
	if (driver->diag_smux_enabled) {
		driver->lcid = LCID_INVALID;
		kfree(driver->buf_in_smux);
		driver->diag_smux_enabled = 0;
	}
	platform_driver_unregister(&msm_hsic_ch_driver);
	platform_driver_unregister(&msm_diagfwd_smux_driver);
	/* destroy USB MDM specific variables */
#ifdef CONFIG_DIAG_OVER_USB
	if (driver->usb_mdm_connected)
		usb_diag_free_req(driver->mdm_ch);
	usb_diag_close(driver->mdm_ch);
#endif
	kfree(driver->usb_buf_mdm_out);
	kfree(driver->hsic_buf_tbl);
	kfree(driver->write_ptr_mdm);
	kfree(driver->usb_read_mdm_ptr);
	destroy_workqueue(driver->diag_bridge_wq);
>>>>>>> cm-10.0
}
