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

#include "object.h"
#include "bool.h"

struct CFWBool {
	CFWObject obj;
	bool value;
};

static bool
ctor(void *ptr, va_list args)
{
	CFWBool *boolean = ptr;

	boolean->value = va_arg(args, int);

	return true;
}

static bool
equal(void *ptr1, void *ptr2)
{
	CFWObject *obj2 = ptr2;
	CFWBool *boolean1, *boolean2;

	if (!cfw_is(obj2, cfw_bool))
		return false;

	boolean1 = ptr1;
	boolean2 = ptr2;

	return (boolean1->value == boolean2->value);
}

static uint32_t
hash(void *ptr)
{
	CFWBool *boolean = ptr;

	return (uint32_t)boolean->value;
}

static void*
copy(void *ptr)
{
	return cfw_retain(ptr);
}

bool
cfw_bool_value(CFWBool *boolean)
{
	return boolean->value;
}

static CFWClass class = {
	.name = "CFWBool",
	.size = sizeof(CFWBool),
	.ctor = ctor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};
CFWClass *cfw_bool = &class;
