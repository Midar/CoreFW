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

#ifndef __COREFW_FILE_H__
#define __COREFW_FILE_H__

#include "class.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct CFWFile CFWFile;

extern CFWClass *cfw_file;

extern CFWFile *cfw_stdin;
extern CFWFile *cfw_stdout;
extern CFWFile *cfw_stderr;
#ifdef __cplusplus
}
#endif

#endif
