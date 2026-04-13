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

#ifndef __COREFW_HASH_H__
#define __COREFW_HASH_H__

#define CFW_HASH_INIT(hash) hash = 0
#define CFW_HASH_ADD(hash, byte)	\
	{				\
		hash += (uint8_t)byte;	\
		hash += (hash << 10);	\
		hash ^= (hash >> 6);	\
	}
#define CFW_HASH_FINALIZE(hash)		\
	{				\
		hash += (hash << 3);	\
		hash ^= (hash >> 11);	\
		hash += (hash << 15);	\
	}
#define CFW_HASH_ADD_HASH(hash, other_)				\
	{							\
		uint32_t other = other_;			\
		CFW_HASH_ADD(hash, (other >> 24) & 0xFF);	\
		CFW_HASH_ADD(hash, (other >> 16) & 0xFF);	\
		CFW_HASH_ADD(hash, (other >>  8) & 0xFF);	\
		CFW_HASH_ADD(hash, other & 0xFF);		\
	}

#endif
