#include <linux/ceph/ceph_debug.h>

#include <linux/err.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/vmalloc.h>

#include <linux/ceph/msgpool.h>

<<<<<<< HEAD
static void *alloc_fn(gfp_t gfp_mask, void *arg)
{
	struct ceph_msgpool *pool = arg;
	void *p;

	p = ceph_msg_new(0, pool->front_len, gfp_mask);
	if (!p)
		pr_err("msgpool %s alloc failed\n", pool->name);
	return p;
}

static void free_fn(void *element, void *arg)
{
	ceph_msg_put(element);
=======
static void *msgpool_alloc(gfp_t gfp_mask, void *arg)
{
	struct ceph_msgpool *pool = arg;
	struct ceph_msg *msg;

	msg = ceph_msg_new(0, pool->front_len, gfp_mask, true);
	if (!msg) {
		dout("msgpool_alloc %s failed\n", pool->name);
	} else {
		dout("msgpool_alloc %s %p\n", pool->name, msg);
		msg->pool = pool;
	}
	return msg;
}

static void msgpool_free(void *element, void *arg)
{
	struct ceph_msgpool *pool = arg;
	struct ceph_msg *msg = element;

	dout("msgpool_release %s %p\n", pool->name, msg);
	msg->pool = NULL;
	ceph_msg_put(msg);
>>>>>>> cm-10.0
}

int ceph_msgpool_init(struct ceph_msgpool *pool,
		      int front_len, int size, bool blocking, const char *name)
{
<<<<<<< HEAD
	pool->front_len = front_len;
	pool->pool = mempool_create(size, alloc_fn, free_fn, pool);
=======
	dout("msgpool %s init\n", name);
	pool->front_len = front_len;
	pool->pool = mempool_create(size, msgpool_alloc, msgpool_free, pool);
>>>>>>> cm-10.0
	if (!pool->pool)
		return -ENOMEM;
	pool->name = name;
	return 0;
}

void ceph_msgpool_destroy(struct ceph_msgpool *pool)
{
<<<<<<< HEAD
=======
	dout("msgpool %s destroy\n", pool->name);
>>>>>>> cm-10.0
	mempool_destroy(pool->pool);
}

struct ceph_msg *ceph_msgpool_get(struct ceph_msgpool *pool,
				  int front_len)
{
<<<<<<< HEAD
	if (front_len > pool->front_len) {
		pr_err("msgpool_get pool %s need front %d, pool size is %d\n",
=======
	struct ceph_msg *msg;

	if (front_len > pool->front_len) {
		dout("msgpool_get %s need front %d, pool size is %d\n",
>>>>>>> cm-10.0
		       pool->name, front_len, pool->front_len);
		WARN_ON(1);

		/* try to alloc a fresh message */
<<<<<<< HEAD
		return ceph_msg_new(0, front_len, GFP_NOFS);
	}

	return mempool_alloc(pool->pool, GFP_NOFS);
=======
		return ceph_msg_new(0, front_len, GFP_NOFS, false);
	}

	msg = mempool_alloc(pool->pool, GFP_NOFS);
	dout("msgpool_get %s %p\n", pool->name, msg);
	return msg;
>>>>>>> cm-10.0
}

void ceph_msgpool_put(struct ceph_msgpool *pool, struct ceph_msg *msg)
{
<<<<<<< HEAD
=======
	dout("msgpool_put %s %p\n", pool->name, msg);

>>>>>>> cm-10.0
	/* reset msg front_len; user may have changed it */
	msg->front.iov_len = pool->front_len;
	msg->hdr.front_len = cpu_to_le32(pool->front_len);

	kref_init(&msg->kref);  /* retake single ref */
<<<<<<< HEAD
=======
	mempool_free(msg, pool->pool);
>>>>>>> cm-10.0
}
