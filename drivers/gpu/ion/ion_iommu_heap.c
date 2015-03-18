/*
<<<<<<< HEAD
 * Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
=======
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
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
#include <linux/err.h>
#include <linux/io.h>
#include <linux/ion.h>
#include <linux/mm.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/iommu.h>
#include <linux/pfn.h>
#include "ion_priv.h"

#include <asm/mach/map.h>
#include <asm/page.h>
<<<<<<< HEAD
=======
#include <asm/cacheflush.h>
>>>>>>> cm-10.0
#include <mach/iommu_domains.h>

struct ion_iommu_heap {
	struct ion_heap heap;
<<<<<<< HEAD
=======
	unsigned int has_outer_cache;
>>>>>>> cm-10.0
};

struct ion_iommu_priv_data {
	struct page **pages;
	int nrpages;
	unsigned long size;
<<<<<<< HEAD
	struct scatterlist *iommu_sglist;
=======
>>>>>>> cm-10.0
};

static int ion_iommu_heap_allocate(struct ion_heap *heap,
				      struct ion_buffer *buffer,
				      unsigned long size, unsigned long align,
				      unsigned long flags)
{
	int ret, i;
	struct ion_iommu_priv_data *data = NULL;

	if (msm_use_iommu()) {
<<<<<<< HEAD
=======
		struct scatterlist *sg;
		struct sg_table *table;
		unsigned int i;

>>>>>>> cm-10.0
		data = kmalloc(sizeof(*data), GFP_KERNEL);
		if (!data)
			return -ENOMEM;

		data->size = PFN_ALIGN(size);
		data->nrpages = data->size >> PAGE_SHIFT;
		data->pages = kzalloc(sizeof(struct page *)*data->nrpages,
				GFP_KERNEL);
		if (!data->pages) {
			ret = -ENOMEM;
			goto err1;
		}
<<<<<<< HEAD
		data->iommu_sglist = vmalloc(sizeof(*data->iommu_sglist) *
						data->nrpages);
		if (!data->iommu_sglist) {
			ret = -ENOMEM;
			goto err1;
		}

		sg_init_table(data->iommu_sglist, data->nrpages);

		for (i = 0; i < data->nrpages; i++) {
			data->pages[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
			if (!data->pages[i])
				goto err2;

			sg_set_page(&data->iommu_sglist[i], data->pages[i],
				    PAGE_SIZE, 0);
		}


=======

		table = buffer->sg_table =
				kzalloc(sizeof(struct sg_table), GFP_KERNEL);

		if (!table) {
			ret = -ENOMEM;
			goto err1;
		}
		ret = sg_alloc_table(table, data->nrpages, GFP_KERNEL);
		if (ret)
			goto err2;

		for_each_sg(table->sgl, sg, table->nents, i) {
			data->pages[i] = alloc_page(GFP_KERNEL | __GFP_ZERO);
			if (!data->pages[i])
				goto err3;

			sg_set_page(sg, data->pages[i], PAGE_SIZE, 0);
		}

>>>>>>> cm-10.0
		buffer->priv_virt = data;
		return 0;

	} else {
		return -ENOMEM;
	}


<<<<<<< HEAD
err2:
	vfree(data->iommu_sglist);
	data->iommu_sglist = NULL;
=======
err3:
	sg_free_table(buffer->sg_table);
err2:
	kfree(buffer->sg_table);
	buffer->sg_table = 0;
>>>>>>> cm-10.0

	for (i = 0; i < data->nrpages; i++) {
		if (data->pages[i])
			__free_page(data->pages[i]);
	}
	kfree(data->pages);
err1:
	kfree(data);
	return ret;
}

static void ion_iommu_heap_free(struct ion_buffer *buffer)
{
	struct ion_iommu_priv_data *data = buffer->priv_virt;
	int i;

	if (!data)
		return;

	for (i = 0; i < data->nrpages; i++)
		__free_page(data->pages[i]);

<<<<<<< HEAD
	vfree(data->iommu_sglist);
	data->iommu_sglist = NULL;

=======
>>>>>>> cm-10.0
	kfree(data->pages);
	kfree(data);
}

void *ion_iommu_heap_map_kernel(struct ion_heap *heap,
<<<<<<< HEAD
				   struct ion_buffer *buffer,
				   unsigned long flags)
=======
				struct ion_buffer *buffer)
>>>>>>> cm-10.0
{
	struct ion_iommu_priv_data *data = buffer->priv_virt;
	pgprot_t page_prot = PAGE_KERNEL;

	if (!data)
		return NULL;

<<<<<<< HEAD
	if (!ION_IS_CACHED(flags))
=======
	if (!ION_IS_CACHED(buffer->flags))
>>>>>>> cm-10.0
		page_prot = pgprot_noncached(page_prot);

	buffer->vaddr = vmap(data->pages, data->nrpages, VM_IOREMAP, page_prot);

	return buffer->vaddr;
}

void ion_iommu_heap_unmap_kernel(struct ion_heap *heap,
				    struct ion_buffer *buffer)
{
	if (!buffer->vaddr)
		return;

	vunmap(buffer->vaddr);
	buffer->vaddr = NULL;
}

int ion_iommu_heap_map_user(struct ion_heap *heap, struct ion_buffer *buffer,
<<<<<<< HEAD
			       struct vm_area_struct *vma, unsigned long flags)
=======
			       struct vm_area_struct *vma)
>>>>>>> cm-10.0
{
	struct ion_iommu_priv_data *data = buffer->priv_virt;
	int i;
	unsigned long curr_addr;
	if (!data)
		return -EINVAL;

<<<<<<< HEAD
	if (!ION_IS_CACHED(flags))
=======
	if (!ION_IS_CACHED(buffer->flags))
>>>>>>> cm-10.0
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	curr_addr = vma->vm_start;
	for (i = 0; i < data->nrpages && curr_addr < vma->vm_end; i++) {
		if (vm_insert_page(vma, curr_addr, data->pages[i])) {
			/*
			 * This will fail the mmap which will
			 * clean up the vma space properly.
			 */
			return -EINVAL;
		}
		curr_addr += PAGE_SIZE;
	}
	return 0;
}

int ion_iommu_heap_map_iommu(struct ion_buffer *buffer,
					struct ion_iommu_map *data,
					unsigned int domain_num,
					unsigned int partition_num,
					unsigned long align,
					unsigned long iova_length,
					unsigned long flags)
{
	struct iommu_domain *domain;
	int ret = 0;
	unsigned long extra;
<<<<<<< HEAD
	struct ion_iommu_priv_data *buffer_data = buffer->priv_virt;
=======
>>>>>>> cm-10.0
	int prot = IOMMU_WRITE | IOMMU_READ;
	prot |= ION_IS_CACHED(flags) ? IOMMU_CACHE : 0;

	BUG_ON(!msm_use_iommu());

	data->mapped_size = iova_length;
	extra = iova_length - buffer->size;

<<<<<<< HEAD
	data->iova_addr = msm_allocate_iova_address(domain_num, partition_num,
						data->mapped_size, align);

	if (!data->iova_addr) {
		ret = -ENOMEM;
		goto out;
	}
=======
	ret = msm_allocate_iova_address(domain_num, partition_num,
						data->mapped_size, align,
						&data->iova_addr);

	if (ret)
		goto out;
>>>>>>> cm-10.0

	domain = msm_get_iommu_domain(domain_num);

	if (!domain) {
		ret = -ENOMEM;
		goto out1;
	}

	ret = iommu_map_range(domain, data->iova_addr,
<<<<<<< HEAD
			      buffer_data->iommu_sglist, buffer->size, prot);
=======
			      buffer->sg_table->sgl,
			      buffer->size, prot);
>>>>>>> cm-10.0
	if (ret) {
		pr_err("%s: could not map %lx in domain %p\n",
			__func__, data->iova_addr, domain);
		goto out1;
	}

	if (extra) {
		unsigned long extra_iova_addr = data->iova_addr + buffer->size;
		ret = msm_iommu_map_extra(domain, extra_iova_addr, extra, SZ_4K,
					  prot);
		if (ret)
			goto out2;
	}
	return ret;

out2:
	iommu_unmap_range(domain, data->iova_addr, buffer->size);
out1:
	msm_free_iova_address(data->iova_addr, domain_num, partition_num,
				buffer->size);

out:

	return ret;
}

void ion_iommu_heap_unmap_iommu(struct ion_iommu_map *data)
{
	unsigned int domain_num;
	unsigned int partition_num;
	struct iommu_domain *domain;

	BUG_ON(!msm_use_iommu());

	domain_num = iommu_map_domain(data);
	partition_num = iommu_map_partition(data);

	domain = msm_get_iommu_domain(domain_num);

	if (!domain) {
		WARN(1, "Could not get domain %d. Corruption?\n", domain_num);
		return;
	}

	iommu_unmap_range(domain, data->iova_addr, data->mapped_size);
	msm_free_iova_address(data->iova_addr, domain_num, partition_num,
				data->mapped_size);

	return;
}

static int ion_iommu_cache_ops(struct ion_heap *heap, struct ion_buffer *buffer,
			void *vaddr, unsigned int offset, unsigned int length,
			unsigned int cmd)
{
<<<<<<< HEAD
	unsigned long vstart, pstart;
	void (*op)(unsigned long, unsigned long, unsigned long);
	unsigned int i;
	struct ion_iommu_priv_data *data = buffer->priv_virt;

	if (!data)
		return -ENOMEM;

	switch (cmd) {
	case ION_IOC_CLEAN_CACHES:
		op = clean_caches;
		break;
	case ION_IOC_INV_CACHES:
		op = invalidate_caches;
		break;
	case ION_IOC_CLEAN_INV_CACHES:
		op = clean_and_invalidate_caches;
=======
	void (*outer_cache_op)(phys_addr_t, phys_addr_t);
	struct ion_iommu_heap *iommu_heap =
	     container_of(heap, struct  ion_iommu_heap, heap);

	switch (cmd) {
	case ION_IOC_CLEAN_CACHES:
		dmac_clean_range(vaddr, vaddr + length);
		outer_cache_op = outer_clean_range;
		break;
	case ION_IOC_INV_CACHES:
		dmac_inv_range(vaddr, vaddr + length);
		outer_cache_op = outer_inv_range;
		break;
	case ION_IOC_CLEAN_INV_CACHES:
		dmac_flush_range(vaddr, vaddr + length);
		outer_cache_op = outer_flush_range;
>>>>>>> cm-10.0
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	vstart = (unsigned long) vaddr;
	for (i = 0; i < data->nrpages; ++i, vstart += PAGE_SIZE) {
		pstart = page_to_phys(data->pages[i]);
		op(vstart, PAGE_SIZE, pstart);
	}

	return 0;
}

static struct scatterlist *ion_iommu_heap_map_dma(struct ion_heap *heap,
					      struct ion_buffer *buffer)
{
	struct ion_iommu_priv_data *data = buffer->priv_virt;
	return data->iommu_sglist;
=======
	if (iommu_heap->has_outer_cache) {
		unsigned long pstart;
		unsigned int i;
		struct ion_iommu_priv_data *data = buffer->priv_virt;
		if (!data)
			return -ENOMEM;

		for (i = 0; i < data->nrpages; ++i) {
			pstart = page_to_phys(data->pages[i]);
			outer_cache_op(pstart, pstart + PAGE_SIZE);
		}
	}
	return 0;
}

static struct sg_table *ion_iommu_heap_map_dma(struct ion_heap *heap,
					      struct ion_buffer *buffer)
{
	return buffer->sg_table;
>>>>>>> cm-10.0
}

static void ion_iommu_heap_unmap_dma(struct ion_heap *heap,
				 struct ion_buffer *buffer)
{
<<<<<<< HEAD
=======
	if (buffer->sg_table)
		sg_free_table(buffer->sg_table);
	kfree(buffer->sg_table);
	buffer->sg_table = 0;
>>>>>>> cm-10.0
}

static struct ion_heap_ops iommu_heap_ops = {
	.allocate = ion_iommu_heap_allocate,
	.free = ion_iommu_heap_free,
	.map_user = ion_iommu_heap_map_user,
	.map_kernel = ion_iommu_heap_map_kernel,
	.unmap_kernel = ion_iommu_heap_unmap_kernel,
	.map_iommu = ion_iommu_heap_map_iommu,
	.unmap_iommu = ion_iommu_heap_unmap_iommu,
	.cache_op = ion_iommu_cache_ops,
	.map_dma = ion_iommu_heap_map_dma,
	.unmap_dma = ion_iommu_heap_unmap_dma,
};

struct ion_heap *ion_iommu_heap_create(struct ion_platform_heap *heap_data)
{
	struct ion_iommu_heap *iommu_heap;

	iommu_heap = kzalloc(sizeof(struct ion_iommu_heap), GFP_KERNEL);
	if (!iommu_heap)
		return ERR_PTR(-ENOMEM);

	iommu_heap->heap.ops = &iommu_heap_ops;
	iommu_heap->heap.type = ION_HEAP_TYPE_IOMMU;
<<<<<<< HEAD
=======
	iommu_heap->has_outer_cache = heap_data->has_outer_cache;
>>>>>>> cm-10.0

	return &iommu_heap->heap;
}

void ion_iommu_heap_destroy(struct ion_heap *heap)
{
	struct ion_iommu_heap *iommu_heap =
	     container_of(heap, struct  ion_iommu_heap, heap);

	kfree(iommu_heap);
	iommu_heap = NULL;
}
