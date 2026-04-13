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

#ifndef __COREFW_STRING_H__
#define __COREFW_STRING_H__

#include "class.h"
#include "range.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CFWString CFWString;
extern CFWClass *cfw_string;
extern size_t cfw_strnlen(const char *, size_t);
extern char *cfw_strdup(const char *);
extern char *cfw_strndup(const char *, size_t);
extern char *cfw_string_c(CFWString *);
extern size_t cfw_string_length(CFWString *);
extern bool cfw_string_set(CFWString *, const char *);
extern void cfw_string_set_nocopy(CFWString*, char *, size_t);
extern bool cfw_string_append(CFWString *, CFWString *);
extern bool cfw_string_append_c(CFWString *, const char *);
extern bool cfw_string_has_prefix(CFWString *, CFWString *);
extern bool cfw_string_has_prefix_c(CFWString *, const char *);
extern bool cfw_string_has_suffix(CFWString *, CFWString *);
extern bool cfw_string_has_suffix_c(CFWString *, const char *);
extern size_t cfw_string_find(CFWString *, CFWString *, cfw_range_t);
extern size_t cfw_string_find_c(CFWString *, const char *, cfw_range_t);
#ifdef __cplusplus
}
#endif

#endif
