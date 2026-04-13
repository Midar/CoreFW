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

#include <stdlib.h>

#include "object.h"
#include "map.h"
#include "hash.h"
#include "string.h"

static struct bucket {
	CFWObject *key, *obj;
	uint32_t hash;
} deleted = { CFW_NIL, CFW_NIL, 0 };

struct CFWMap {
	CFWObject obj;
	struct bucket **data;
	uint32_t size;
	size_t count;
};

static bool
ctor(void *ptr, va_list args)
{
	CFWMap *map = ptr;
	void *key;

	map->data = NULL;
	map->size = 0;
	map->count = 0;

	while ((key = va_arg(args, void *)) != CFW_NIL)
		if (!cfw_map_set(map, key, va_arg(args, void *)))
			return false;

	return true;
}

static void
dtor(void *ptr)
{
	CFWMap *map = ptr;
	uint32_t i;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != NULL && map->data[i] != &deleted) {
			cfw_release(map->data[i]->key);
			cfw_release(map->data[i]->obj);
			free(map->data[i]);
		}
	}

	if (map->data != NULL)
		free(map->data);
}

static bool
equal(void *ptr1, void *ptr2)
{
	CFWObject *obj2 = ptr2;
	CFWMap *map1, *map2;
	uint32_t i;

	if (obj2->cls != cfw_map)
		return false;

	map1 = ptr1;
	map2 = ptr2;

	if (map1->count != map2->count)
		return false;

	for (i = 0; i < map1->size; i++)
		if (map1->data[i] != NULL && map1->data[i] != &deleted &&
		    !cfw_equal(cfw_map_get(map2, map1->data[i]->key),
		    map1->data[i]->obj))
			return false;

	return true;
}

static uint32_t
hash(void *ptr)
{
	CFWMap *map = ptr;
	uint32_t i, hash = 0;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != NULL && map->data[i] != &deleted) {
			hash += map->data[i]->hash;
			hash += cfw_hash(map->data[i]->obj);
		}
	}

	return hash;
}

static void *
copy(void *ptr)
{
	CFWMap *map = ptr;
	CFWMap *new;
	uint32_t i;

	if ((new = cfw_new(cfw_map, CFW_NIL)) == CFW_NIL)
		return CFW_NIL;

	if ((new->data = malloc(sizeof(*new->data) * map->size)) == NULL)
		return CFW_NIL;
	new->size = map->size;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != NULL && map->data[i] != &deleted) {
			struct bucket *bucket;

			if ((bucket = malloc(sizeof(*bucket))) == NULL)
				return CFW_NIL;

			bucket->key = cfw_retain(map->data[i]->key);
			bucket->obj = cfw_retain(map->data[i]->obj);
			bucket->hash = map->data[i]->hash;

			new->data[i] = bucket;
		} else
			new->data[i] = NULL;
	}

	return new;
}

bool
resize(CFWMap *map, uint32_t count)
{
	size_t fullness = count * 4 / map->size;
	struct bucket **ndata;
	uint32_t i, nsize;

	if (count > UINT32_MAX)
		return false;

	if (fullness >= 3)
		nsize = map->size << 1;
	else if (fullness <= 1)
		nsize = map->size >> 1;
	else
		return true;

	if (nsize == 0)
		return false;

	if ((ndata = calloc(nsize, sizeof(*ndata))) == NULL)
		return false;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != NULL && map->data[i] != &deleted) {
			uint32_t j, last;

			last = nsize;

			j = map->data[i]->hash & (nsize - 1);
			for (; j < last && ndata[j] != NULL; j++);

			/* In case the last bucket is already used */
			if (j >= last) {
				last = map->data[i]->hash & (nsize - 1);

				for (j = 0; j < last && ndata[j] != NULL; j++);
			}

			if (j >= last) {
				free(ndata);
				return false;
			}

			ndata[j] = map->data[i];
		}
	}

	free(map->data);
	map->data = ndata;
	map->size = nsize;

	return true;
}

size_t
cfw_map_count(CFWMap *map)
{
	return map->count;
}

void *
cfw_map_get(CFWMap *map, void *key)
{
	uint32_t i, hash, last;

	if (key == CFW_NIL)
		return CFW_NIL;

	hash = cfw_hash(key);
	last = map->size;

	for (i = hash & (map->size - 1);
	    i < last && map->data[i] != NULL; i++) {
		if (map->data[i] == &deleted)
			continue;

		if (cfw_equal(map->data[i]->key, key))
			return map->data[i]->obj;
	}

	if (i < last)
		return CFW_NIL;

	/* In case the last bucket is already used */
	last = hash & (map->size - 1);

	for (i = 0; i < last && map->data[i] != NULL; i++) {
		if (map->data[i] == &deleted)
			continue;

		if (cfw_equal(map->data[i]->key, key))
			return map->data[i]->obj;
	}

	return CFW_NIL;
}

void *
cfw_map_get_c(CFWMap *map, const char *key)
{
	CFWString *str;
	void *ret;

	if ((str = cfw_new(cfw_string, key)) == CFW_NIL)
		return CFW_NIL;

	ret = cfw_map_get(map, str);

	cfw_release(str);

	return ret;
}

bool
cfw_map_set(CFWMap *map, void *key, void *obj)
{
	uint32_t i, hash, last;

	if (key == CFW_NIL)
		return false;

	if (map->data == NULL) {
		if ((map->data = malloc(sizeof(*map->data))) == NULL)
			return false;

		map->data[0] = NULL;
		map->size = 1;
		map->count = 0;
	}

	hash = cfw_hash(key);
	last = map->size;

	for (i = hash & (map->size - 1);
	    i < last && map->data[i] != NULL; i++) {
		if (map->data[i] == &deleted)
			continue;

		if (cfw_equal(map->data[i]->key, key))
			break;
	}

	/* In case the last bucket is already used */
	if (i >= last) {
		last = hash & (map->size - 1);

		for (i = 0; i < last && map->data[i] != NULL; i++) {
			if (map->data[i] == &deleted)
				continue;

			if (cfw_equal(map->data[i]->key, key))
				break;
		}
	}

	/* Key not in dictionary */
	if (i >= last || map->data[i] == NULL || map->data[i] == &deleted ||
	    !cfw_equal(map->data[i]->key, key)) {
		struct bucket *bucket;

		if (obj == CFW_NIL)
			return true;

		if (!resize(map, map->count + 1))
			return false;

		last = map->size;

		for (i = hash & (map->size - 1); i < last &&
		    map->data[i] != NULL && map->data[i] != &deleted; i++);

		/* In case the last bucket is already used */
		if (i >= last) {
			last = hash & (map->size - 1);

			for (i = 0; i < last && map->data[i] != NULL &&
			    map->data[i] != &deleted; i++);
		}

		if (i >= last)
			return false;

		if ((bucket = malloc(sizeof(*bucket))) == NULL)
			return false;

		if ((bucket->key = cfw_copy(key)) == CFW_NIL) {
			free(bucket);
			return false;
		}

		bucket->obj = cfw_retain(obj);
		bucket->hash = cfw_hash(key);

		map->data[i] = bucket;
		map->count++;

		return true;
	}

	if (obj != CFW_NIL) {
		void *old = map->data[i]->obj;
		map->data[i]->obj = cfw_retain(obj);
		cfw_release(old);
	} else {
		cfw_release(map->data[i]->key);
		cfw_release(map->data[i]->obj);

		free(map->data[i]);
		map->data[i] = &deleted;

		map->count--;

		if (!resize(map, map->count))
			return false;
	}

	return true;
}

bool
cfw_map_set_c(CFWMap *map, const char *key, void *obj)
{
	CFWString *str;
	bool ret;

	if ((str = cfw_new(cfw_string, key)) == CFW_NIL)
		return false;

	ret = cfw_map_set(map, str, obj);

	cfw_release(str);

	return ret;
}

void
cfw_map_iter(CFWMap *map, cfw_map_iter_t *iter)
{
	iter->_map = map;
	iter->_pos = 0;

	cfw_map_iter_next(iter);
}

void
cfw_map_iter_next(cfw_map_iter_t *iter)
{
	CFWMap *map = iter->_map;

	for (; iter->_pos < map->size &&
	    (map->data[iter->_pos] == NULL ||
	    map->data[iter->_pos] == &deleted); iter->_pos++);

	if (iter->_pos < map->size) {
		iter->key = map->data[iter->_pos]->key;
		iter->obj = map->data[iter->_pos]->obj;
		iter->_pos++;
	} else {
		iter->key = CFW_NIL;
		iter->obj = CFW_NIL;
	}
}

static CFWClass class = {
	.name = "CFWMap",
	.size = sizeof(CFWMap),
	.ctor = ctor,
	.dtor = dtor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};
CFWClass *cfw_map = &class;
