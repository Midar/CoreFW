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

#include "object.h"
#include "int.h"

struct CFWInt {
	CFWObject obj;
	intmax_t value;
};

static bool
ctor(void *ptr, va_list args)
{
	CFWInt *integer = ptr;

	integer->value = va_arg(args, intmax_t);

	return true;
}

static bool
equal(void *ptr1, void *ptr2)
{
	CFWObject *obj2 = ptr2;
	CFWInt *int1, *int2;

	if (obj2->cls != cfw_int)
		return false;

	int1 = ptr1;
	int2 = ptr2;

	return (int1->value == int2->value);
}

static uint32_t
hash(void *ptr)
{
	CFWInt *integer = ptr;

	return (uint32_t)integer->value;
}

static void*
copy(void *ptr)
{
	return cfw_ref(ptr);
}

intmax_t
cfw_int_value(CFWInt *integer)
{
	return integer->value;
}

static CFWClass class = {
	.name = "CFWInt",
	.size = sizeof(CFWInt),
	.ctor = ctor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};
CFWClass *cfw_int = &class;
