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

#ifndef __COREFW_OBJECT_H__
#define __COREFW_OBJECT_H__

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CFWObject {
	CFWClass *cls;
	int ref_cnt;
} CFWObject;

extern CFWClass *cfw_object;
extern void *cfw_new(CFWClass *, ...);
extern void *cfw_create(CFWClass *, ...);
extern void *cfw_ref(void *);
extern void cfw_unref(void *);
extern void cfw_free(void *);
extern CFWClass* cfw_class(void *);
extern bool cfw_is(void *, CFWClass *);
extern bool cfw_equal(void *, void *);
extern uint32_t cfw_hash(void *);
extern void* cfw_copy(void *);
#ifdef __cplusplus
}
#endif

#endif
