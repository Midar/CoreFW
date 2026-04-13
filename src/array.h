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

#ifndef __COREFW_ARRAY_H__
#define __COREFW_ARRAY_H__

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CFWArray CFWArray;
extern CFWClass *cfw_array;
extern size_t cfw_array_count(CFWArray *);
extern void *cfw_array_get(CFWArray *, size_t);
extern bool cfw_array_set(CFWArray *, size_t, void *);
extern bool cfw_array_push(CFWArray *, void *);
extern void *cfw_array_last(CFWArray *);
extern bool cfw_array_pop(CFWArray *);
extern bool cfw_array_contains(CFWArray *, void *);
extern bool cfw_array_contains_ptr(CFWArray *, void *);
extern size_t cfw_array_find(CFWArray *, void *);
extern size_t cfw_array_find_ptr(CFWArray *, void *);
#ifdef __cplusplus
}
#endif

#endif
