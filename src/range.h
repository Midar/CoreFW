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

#ifndef __COREFW_RANGE_H__
#define __COREFW_RANGE_H__

typedef struct cfw_range_t {
	size_t start;
	size_t length;
} cfw_range_t;

extern cfw_range_t cfw_range_all;
extern cfw_range_t cfw_range(size_t, size_t);

#endif
