/**
 * @file	include/ringbuf.h
 *
 * @brief	Объявления структур ифункций для работы с кольцевым буфером.
 *
 * @author	Vasily Yurchenko <Vasiliy.Yurchenko@t-platforms.ru>
 *
 * @copyright	Copyright (C) 2019. All rights reserved.
 */

#ifndef __RINGBUF_H
#define __RINGBUF_H

#include <stdint.h>
#include <stddef.h>

/** Количество элементов кольцевого буфера. */
#define RING_BUF_SIZE	1024

/** Структура-дескриптор кольцевого буфера. */
struct ring_buf {
	uint8_t				buffer[RING_BUF_SIZE];	/**< Буфер. */
	volatile size_t		array_head;		/**< Голова буфера. Индекс позиции начала заполнения буфера. */
	volatile size_t		array_tail;		/**< Хвост буфера. Индекс позиции следующей за последним занятым элементом. */
	uint8_t				overflow;		/**< Флаг переполнения буфера. */
};

/**
 * @brief		Инициализирует структуру кольцевого буфера.
 *
 * @param[in,out]	rb	дескриптор кольцевого буфера
 *
 * @retval		void
 */
void rb_init_ring_buffer(struct ring_buf *rb);

/**
 * @brief	Возвращает значение флага переполнения буфера.
 *
 * @param[in]	rb	дескриптор кольцевого буфера
 *
 * @retval	1	произошло переполнение
 * @retval	0	переполнения не было
 */
static inline uint32_t rb_get_status(const struct ring_buf *rb)
{
	return rb->overflow;
}

/**
 * @brief	Возвращает количество элементов находящихся в буфере.
 *
 * @param[in]	rb	дескриптор кольцевого буфера
 *
 * @retval	количество элементов в буфере
 */
static inline size_t rb_get_data_size(const struct ring_buf *rb)
{
	return (rb->array_tail - rb->array_head) & (RING_BUF_SIZE - 1);
}

/**
 * @brief	Возвращает количество элементов доступных для записи(свободных).
 *
 * @param[in]	rb	дескриптор кольцевого буфера
 *
 * @retval	количество элементов доступных для записи(свободных)
 */
static inline size_t rb_get_data_fsize(const struct ring_buf *rb)
{
	return (RING_BUF_SIZE - 1) - ((rb->array_tail - rb->array_head) & (RING_BUF_SIZE - 1));
}

/**
 * @brief	Переносит запрошенное количество элементов из кольцевого буфера
 *		в указанное место.
 *		Переносимые элементы удаляются из кольцевого буфера.
 *		Если запрошенное количество элементов превышает количество
 *		имеющихся в буфере, то возвращается доступное количество элементов.
 *
 * @param[in,out]	rb		дескриптор кольцевого буфера
 * @param[out]		dstbuf		буфер, куда переносятся элементы
 * @param[in]		size_to_get	запрашиваемое количество элементов
 *
 * @retval		количество извлеченных элементов из кольцевого буфера
 */
size_t rb_get_data(struct ring_buf *rb, void *dstbuf, size_t size_to_get);

/**
 * @brief	Сохраняет указанное количество элементов в кольцевой буфер
 *		из указанного места.
 *		Если указанное количество элементов для сохранения превышает количество
 *		свободных элементов в буфере, то сохраняется кличество элементов
 *		равное количеству свободных и устанавливается флаг переполнения.
 *
 * @param[in,out]	rb		дескриптор кольцевого буфера
 * @param[in]		srcbuf		буфер, откуда копируются элементы
 * @param[in]		size_to_store	количество элементов для сохранения
 *
 * @retval		количество сохраненных элементов в кольцевой буфер
 */
size_t rb_store_data(struct ring_buf *rb, const void *srcbuf, size_t size_to_store);

/**
 * @brief	Очищает кольцевой буфер и сбрасывает флаг переполнения.
 *		Очистка происходит путем присвоения индекса хвоста индексу головы.
 *
 * @param[in,out]	ring_buf	дескриптор кольцевого буфера
 *
 * @retval		void
 */
void rb_clear_data(struct ring_buf *rb);

/**
 * @brief	Смещает голову буфера на указанное количество элементов (отбрасывает их)
 *
 * @param[in,out]	ring_buf	дескриптор кольцевого буфера
 * @param[in]		throw_size	количество отбрасываемых элементов
 *
 * @retval		количество отброшенных элементов
 */
size_t rb_throw_last_data(struct ring_buf *rb, size_t throw_size);

#endif /* __RINGBUF_H */
