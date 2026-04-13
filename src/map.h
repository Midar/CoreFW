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

#ifndef __COREFW_MAP_H__
#define __COREFW_MAP_H__

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CFWMap CFWMap;

typedef struct cfw_map_iter_t {
	void *key, *obj;
	/* private */
	CFWMap *_map;
	uint32_t _pos;
} cfw_map_iter_t;

extern CFWClass *cfw_map;
extern size_t cfw_map_size(CFWMap*);
extern void *cfw_map_get(CFWMap*, void *);
extern void *cfw_map_get_c(CFWMap*, const char *);
extern bool cfw_map_set(CFWMap*, void *, void *);
extern bool cfw_map_set_c(CFWMap*, const char *, void *);
extern void cfw_map_iter(CFWMap*, cfw_map_iter_t *);
extern void cfw_map_iter_next(cfw_map_iter_t*);
#ifdef __cplusplus
}
#endif

#endif
