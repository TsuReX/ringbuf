/**
 * @file	Src/ringbuf.c
 *
 * @brief	Определения функций работы с кольцевым буфером.
 *
 * @author	Vasily Yurchenko <Vasiliy.Yurchenko@t-platforms.ru>
 *
 * @copyright	Copyright (C) 2019, T-Platforms. All rights reserved.
 */

#include <string.h>

#include "types.h"

void rb_init_ring_buffer(struct ring_buf *rb)
{
	rb->array_head = 0;
	rb->array_tail = 0;
	rb->overflow = 0;
}

size_t rb_get_data(struct ring_buf *rb, void *dstbuf, size_t size_to_get)
{
	size_t head = rb->array_head;
	size_t tail = rb->array_tail;
	uint32_t overflow = rb->overflow;

	BUILD_BUG_ON_NOT_POWER_OF_2(RING_BUF_SIZE);
	size_t used_size = (tail - head) & (RING_BUF_SIZE - 1);

	size_t p1_size = 0;
	size_t p2_size = 0;

	if (used_size < size_to_get)
		size_to_get = used_size;


	p1_size = size_to_get;

	if (head == tail) /* Empty dstbuf */
		return 0;

	if (head < tail) { /* Head is before tail */
		memcpy(dstbuf, &rb->buffer[head], p1_size);

	} else if (head > tail) { /* Head is behind tail */

		if (p1_size > (RING_BUF_SIZE - head))
				p1_size = RING_BUF_SIZE - head;

		p2_size = size_to_get - p1_size;

		memcpy(&((uint8_t*)dstbuf)[0], &rb->buffer[head], p1_size);
		memcpy(&((uint8_t*)dstbuf)[p1_size], &rb->buffer[0], p2_size);
	}

	head = (head + p1_size + p2_size) & (RING_BUF_SIZE - 1);
	rb->overflow = overflow;
	rb->array_head = head;
	return size_to_get;
}

size_t rb_store_data(struct ring_buf *rb, const void *srcbuf, size_t size_to_store)
{
	size_t head = rb->array_head;
	size_t tail = rb->array_tail;

	uint32_t overflow = rb->overflow;

	BUILD_BUG_ON_NOT_POWER_OF_2(RING_BUF_SIZE);
	size_t used_size = (tail - head) & (RING_BUF_SIZE - 1);

	size_t p1_size = 0;
	size_t p2_size = 0;

	if ((RING_BUF_SIZE - used_size) == 1) /* Full buffer */
		return 0;

	if ((RING_BUF_SIZE - used_size - 1) < size_to_store) {
		overflow = 1;
		size_to_store = RING_BUF_SIZE - used_size - 1;
	}

	p1_size = size_to_store;

	if (head == tail) { /* Empty buffer */

		memcpy(&rb->buffer[0], srcbuf, p1_size);
		rb->array_head = 0;
		tail = 0;

	} else if (head < tail) { /* Head is before tail */

		if (size_to_store > (RING_BUF_SIZE - tail))
			p1_size = RING_BUF_SIZE - tail;

		p2_size = size_to_store - p1_size;

		memcpy(&rb->buffer[tail], srcbuf, p1_size);
		memcpy(&rb->buffer[0], srcbuf + p1_size, p2_size);

	} else if (head > tail) /* Head is behind tail */
		memcpy(&rb->buffer[tail], srcbuf, p1_size);

	tail = (tail + p1_size + p2_size) & (RING_BUF_SIZE - 1);
	rb->overflow = overflow;
	rb->array_tail = tail;
	return size_to_store;
}

void rb_clear_data(struct ring_buf *rb)
{
	rb->array_head = rb->array_tail;
	rb->overflow = 0;
}
