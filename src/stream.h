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

#ifndef __COREFW_STREAM_H__
#define __COREFW_STREAM_H__

#include <unistd.h>

#include "class.h"
#include "object.h"
#include "string.h"

struct cfw_stream_ops {
	ssize_t (*read)(void *, void *, size_t);
	bool (*write)(void *, const void *, size_t);
	bool (*at_end)(void *);
	void (*close)(void *);
};

typedef struct CFWStream {
	CFWObject obj;
	struct cfw_stream_ops *ops;
	char *cache;
	size_t cache_len;
} CFWStream;

extern CFWClass *cfw_stream;
extern ssize_t cfw_stream_read(void *, void *, size_t);
extern CFWString* cfw_stream_read_line(void *);
extern bool cfw_stream_write(void *, const void *, size_t);
extern bool cfw_stream_write_string(void *, const char *);
extern bool cfw_stream_write_line(void *, const char *);
extern bool cfw_stream_at_end(void *);
extern void cfw_stream_close(void *);
#endif
