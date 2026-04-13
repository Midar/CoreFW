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

#include "object.h"
#include "box.h"

struct CFWBox {
	CFWObject obj;
	void *ptr;
	uint32_t type;
	bool free;
};

static bool
ctor(void *ptr, va_list args)
{
	CFWBox *box = ptr;

	box->ptr = va_arg(args, void *);
	box->type = va_arg(args, uint32_t);
	box->free = va_arg(args, int);

	return true;
}

static void
dtor(void *ptr)
{
	CFWBox *box = ptr;

	if (box->free)
		free(box->ptr);
}

void *
cfw_box_ptr(CFWBox *box)
{
	return box->ptr;
}

uint32_t
cfw_box_type(CFWBox *box)
{
	return box->type;
}

static CFWClass class = {
	.name = "CFWBox",
	.size = sizeof(CFWBox),
	.ctor = ctor,
	.dtor = dtor
};
CFWClass *cfw_box = &class;
