// -----------------------------------------------------------------------------
/*
 * Copyright (c) 2007 Fabian Greif, Roboterclub Aachen e.V.
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: can_buffer.c 758 2010-08-01 23:50:48Z jacobsen $
 */
// -----------------------------------------------------------------------------

#include "can_private.h"
#include "can_buffer.h"
#include "utils.h"

#if CAN_RX_BUFFER_SIZE > 0 || CAN_TX_BUFFER_SIZE > 0

// -----------------------------------------------------------------------------
void can_buffer_init(tCANBuffer *buf, uint8_t size, tCAN *list)
{
	ENTER_CRITICAL_SECTION;
	buf->size = size;
	buf->buf = list;
	
	buf->head = 0;
	buf->tail = 0;
	buf->used = 0;
	LEAVE_CRITICAL_SECTION;
}

// -----------------------------------------------------------------------------
bool can_buffer_empty(tCANBuffer *buf)
{
	uint8_t used;
	
	ENTER_CRITICAL_SECTION;
	used = buf->used;
	LEAVE_CRITICAL_SECTION;
	
	if (used == 0)
		return true;
	else
		return false;
}

// -----------------------------------------------------------------------------
bool can_buffer_full(tCANBuffer *buf)
{
	uint8_t used;
	uint8_t size;
	
	ENTER_CRITICAL_SECTION;
	used = buf->used;
	size = buf->size;
	LEAVE_CRITICAL_SECTION;
	
	if (used >= size)
		return true;
	else
		return false;
}

// -----------------------------------------------------------------------------
tCAN *can_buffer_get_enqueue_ptr(tCANBuffer *buf)
{
	if (can_buffer_full( buf ))
		return NULL;
	
	return &buf->buf[buf->head];
}

// -----------------------------------------------------------------------------
void can_buffer_enqueue(tCANBuffer *buf)
{
	ENTER_CRITICAL_SECTION;
	buf->used ++;
	if (++buf->head >= buf->size)
		buf->head = 0;
	LEAVE_CRITICAL_SECTION;
}

// -----------------------------------------------------------------------------
tCAN *can_buffer_get_dequeue_ptr(tCANBuffer *buf)
{
	if (can_buffer_empty( buf ))
		return NULL;
	
	return &buf->buf[buf->tail];
}

// -----------------------------------------------------------------------------
void can_buffer_dequeue(tCANBuffer *buf)
{
	ENTER_CRITICAL_SECTION;
	buf->used --;
	if (++buf->tail >= buf->size)
		buf->tail = 0;
	LEAVE_CRITICAL_SECTION;
}

#endif
