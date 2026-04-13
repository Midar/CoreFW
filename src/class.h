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

#ifndef __COREFW_CLASS_H__
#define __COREFW_CLASS_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CFWClass {
	const char *name;
	size_t size;
	bool (*ctor)(void *, va_list);
	void (*dtor)(void *);
	bool (*equal)(void *, void *);
	uint32_t (*hash)(void *);
	void *(*copy)(void *);
} CFWClass;

extern const char *cfw_class_name(CFWClass *);
#ifdef __cplusplus
}
#endif

#endif
