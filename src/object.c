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
#include "autoreleasepool.h"

void *
cfw_new(CFWClass *class, ...)
{
	CFWObject *obj;

	if ((obj = malloc(class->size)) == NULL)
		return CFW_NIL;

	obj->cls = class;
	obj->retain_cnt = 1;

	if (class->ctor != NULL) {
		va_list args;
		va_start(args, class);

		if (!class->ctor(obj, args)) {
			cfw_release(obj);
			return CFW_NIL;
		}

		va_end(args);
	}

	return obj;
}

void *
cfw_create(CFWClass *class, ...)
{
	CFWObject *obj;

	assert(class != cfw_autoreleasepool);

	if ((obj = malloc(class->size)) == NULL)
		return CFW_NIL;

	obj->cls = class;
	obj->retain_cnt = 1;

	if (class->ctor != NULL) {
		va_list args;
		va_start(args, class);

		if (!class->ctor(obj, args)) {
			cfw_release(obj);
			return CFW_NIL;
		}

		va_end(args);
	}

	if (!cfw_autoreleasepool_add(obj)) {
		cfw_release(obj);
		return CFW_NIL;
	}

	return obj;
}

void *
cfw_retain(void *ptr)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL)
		return CFW_NIL;

	obj->retain_cnt++;

	return obj;
}

void
cfw_release(void *ptr)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL)
		return;

	if (--obj->retain_cnt == 0)
		cfw_dealloc(obj);
}

void
cfw_dealloc(void *ptr)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL)
		return;

	if (obj->cls->dtor != NULL)
		obj->cls->dtor(obj);

	free(obj);
}

CFWClass *
cfw_class(void *ptr)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL)
		return CFW_NIL;

	return obj->cls;
}

bool
cfw_is(void *ptr, CFWClass *cls)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL || cls == CFW_NIL)
		return false;

	return (obj->cls == cls);
}

bool
cfw_equal(void *ptr1, void *ptr2)
{
	CFWObject *obj1 = ptr1, *obj2 = ptr2;

	if (obj1 == obj2)
		return true;

	if (obj1 == CFW_NIL || obj2 == CFW_NIL)
		return false;

	if (obj1->cls->equal != NULL)
		return obj1->cls->equal(obj1, obj2);
	else
		return (obj1 == obj2);
}

uint32_t
cfw_hash(void *ptr)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL)
		return 0;

	if (obj->cls->hash != NULL)
		return obj->cls->hash(obj);

	return (uint32_t)(uintptr_t)ptr;
}

void *
cfw_copy(void *ptr)
{
	CFWObject *obj = ptr;

	if (obj == CFW_NIL)
		return CFW_NIL;

	if (obj->cls->copy != NULL)
		return obj->cls->copy(obj);

	return CFW_NIL;
}

static CFWClass class = {
	.name = "CFWObject",
	.size = sizeof(CFWObject),
};
CFWClass *cfw_object = &class;
