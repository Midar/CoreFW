/*
 * Copyright (c) 2012, 2026 Jonathan Schleifer <js@nil.im>
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3.0 only,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * version 3.0 for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3.0 along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <assert.h>

#include "object.h"
#include "array.h"
#include "autoreleasepool.h"

struct CFWAutoreleasePool {
	CFWObject obj;
	void **data;
	size_t size;
	CFWAutoreleasePool *prev, *next;
};

static CFWAutoreleasePool *top = CFW_NIL;

static bool
ctor(void *ptr, va_list args)
{
	CFWAutoreleasePool *pool = ptr;

	pool->data = NULL;
	pool->size = 0;

	if (top != CFW_NIL) {
		pool->prev = top;
		top->next = pool;
	} else
		pool->prev = CFW_NIL;
	pool->next = CFW_NIL;

	top = pool;

	return true;
}

static void
dtor(void *ptr)
{
	CFWAutoreleasePool *pool = ptr;
	size_t i;

	if (pool->next != CFW_NIL)
		cfw_release(pool->next);

	for (i = 0; i < pool->size; i++)
		cfw_release(pool->data[i]);

	if (pool->data != NULL)
		free(pool->data);

	top = pool->prev;

	if (top != CFW_NIL)
		top->next = CFW_NIL;
}

bool
cfw_autoreleasepool_add(void *ptr)
{
	void **ndata;

	assert(top != CFW_NIL);

	if (top->data != NULL)
		ndata = realloc(top->data, (top->size + 1) * sizeof(void *));
	else
		ndata = malloc((top->size + 1) * sizeof(void *));

	if (ndata == NULL)
		return false;

	ndata[top->size++] = ptr;

	top->data = ndata;

	return true;
}

static CFWClass class = {
	.name = "CFWAutoreleasePool",
	.size = sizeof(CFWAutoreleasePool),
	.ctor = ctor,
	.dtor = dtor
};
CFWClass *cfw_autoreleasepool = &class;
