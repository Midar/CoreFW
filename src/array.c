/*
 * Copyright (c) 2012, 2013, 2026 Jonathan Schleifer <js@nil.im>
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
#include <stdint.h>

#include "object.h"
#include "array.h"
#include "hash.h"

struct CFWArray {
	CFWObject obj;
	void **data;
	size_t size;
};

static bool
ctor(void *ptr, va_list args)
{
	CFWArray *array = ptr;
	void *obj;

	array->data = NULL;
	array->size = 0;

	while ((obj = va_arg(args, void *)) != CFW_NIL)
		if (!cfw_array_push(array, obj))
			return false;

	return true;
}

static void
dtor(void *ptr)
{
	CFWArray *array = ptr;
	size_t i;

	for (i = 0; i < array->size; i++)
		cfw_release(array->data[i]);

	if (array->data != NULL)
		free(array->data);
}

static bool
equal(void *ptr1, void *ptr2)
{
	CFWObject *obj2 = ptr2;
	CFWArray *array1, *array2;
	size_t i;

	if (!cfw_is(obj2, cfw_array))
		return false;

	array1 = ptr1;
	array2 = ptr2;

	if (array1->size != array2->size)
		return false;

	for (i = 0; i < array1->size; i++)
		if (cfw_equal(array1->data[i], array2->data[i]))
			return false;

	return true;
}

static uint32_t
hash(void *ptr)
{
	CFWArray *array = ptr;
	size_t i;
	uint32_t hash;

	CFW_HASH_INIT(hash);

	for (i = 0; i < array->size; i++)
		CFW_HASH_ADD_HASH(hash, cfw_hash(array->data[i]));

	CFW_HASH_FINALIZE(hash);

	return hash;
}

static void *
copy(void *ptr)
{
	CFWArray *array = ptr;
	CFWArray *new;
	size_t i;

	if ((new = cfw_new(cfw_array, CFW_NIL)) == CFW_NIL)
		return CFW_NIL;

	if ((new->data = malloc(sizeof(void *) * array->size)) == NULL) {
		cfw_release(new);
		return CFW_NIL;
	}
	new->size = array->size;

	for (i = 0; i < array->size; i++)
		new->data[i] = cfw_retain(array->data[i]);

	return new;
}

void *
cfw_array_get(CFWArray *array, size_t index)
{
	if (index >= array->size)
		return CFW_NIL;

	return array->data[index];
}

size_t
cfw_array_size(CFWArray *array)
{
	return array->size;
}

bool
cfw_array_set(CFWArray *array, size_t index, void *ptr)
{
	CFWObject *obj = ptr;
	CFWObject *old;

	if (index >= array->size)
		return false;

	cfw_retain(obj);
	old = array->data[index];
	array->data[index] = obj;
	cfw_release(old);

	return true;
}

bool
cfw_array_push(CFWArray *array, void *ptr)
{
	CFWObject *obj = ptr;
	void **new;

	if (array->data == NULL)
		new = malloc(sizeof(void *));
	else
		new = realloc(array->data, sizeof(void *) * (array->size + 1));

	if (new == NULL)
		return false;

	new[array->size] = cfw_retain(obj);

	array->data = new;
	array->size++;

	return true;
}

void *
cfw_array_last(CFWArray *array)
{
	if (array->size == 0)
		return CFW_NIL;

	return array->data[array->size - 1];
}

bool
cfw_array_pop(CFWArray *array)
{
	void **new;
	void *last;

	if (array->size == 0)
		return false;

	if (array->size == 1) {
		cfw_release(array->data[0]);
		free(array->data);
		array->data = NULL;
		array->size = 0;
		return true;
	}

	last = array->data[array->size - 1];

	new = realloc(array->data, sizeof(void *) * (array->size - 1));
	if (new == NULL)
		return false;

	cfw_release(last);

	array->data = new;
	array->size--;

	return true;
}

bool
cfw_array_contains(CFWArray *array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (cfw_equal(array->data[i], ptr))
			return true;

	return false;
}

bool
cfw_array_contains_ptr(CFWArray *array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (array->data[i] == ptr)
			return true;

	return false;
}

size_t
cfw_array_find(CFWArray *array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (cfw_equal(array->data[i], ptr))
			return i;

	return SIZE_MAX;
}

size_t
cfw_array_find_ptr(CFWArray *array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (array->data[i] == ptr)
			return i;

	return SIZE_MAX;
}

static CFWClass class = {
	.name = "CFWArray",
	.size = sizeof(CFWArray),
	.ctor = ctor,
	.dtor = dtor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};
CFWClass *cfw_array = &class;
