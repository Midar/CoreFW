/*
 * Copyright (c) 2012 Jonathan Schleifer <js@nil.im>
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
#include "refpool.h"
#include "array.h"

struct CFWRefPool {
	CFWObject obj;
	void **data;
	size_t size;
	CFWRefPool *prev, *next;
};

static CFWRefPool *top;

static bool
ctor(void *ptr, va_list args)
{
	CFWRefPool *pool = ptr;

	pool->data = NULL;
	pool->size = 0;

	if (top != NULL) {
		pool->prev = top;
		top->next = pool;
	} else
		pool->prev = NULL;
	pool->next = NULL;

	top = pool;

	return true;
}

static void
dtor(void *ptr)
{
	CFWRefPool *pool = ptr;
	size_t i;

	if (pool->next != NULL)
		cfw_unref(pool->next);

	for (i = 0; i < pool->size; i++)
		cfw_unref(pool->data[i]);

	if (pool->data != NULL)
		free(pool->data);

	top = pool->prev;

	if (top != NULL)
		top->next = NULL;
}

bool
cfw_refpool_add(void *ptr)
{
	void **ndata;

	assert(top != NULL);

	if (top->data != NULL)
		ndata = realloc(top->data, (top->size + 1) * sizeof(void*));
	else
		ndata = malloc((top->size + 1) * sizeof(void*));

	if (ndata == NULL)
		return false;

	ndata[top->size++] = ptr;

	top->data = ndata;

	return true;
}

static CFWClass class = {
	.name = "CFWRefPool",
	.size = sizeof(CFWRefPool),
	.ctor = ctor,
	.dtor = dtor
};
CFWClass *cfw_refpool = &class;
