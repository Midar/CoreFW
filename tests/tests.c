/*
 * Copyright (c) 2012, 2014 Jonathan Schleifer <js@nil.im>
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

#include <stdio.h>

#include "object.h"
#include "refpool.h"
#include "string.h"
#include "int.h"
#include "array.h"
#include "map.h"

static void
print_map(CFWMap *map)
{
	cfw_map_iter_t iter;

	cfw_map_iter(map, &iter);

	fputs("{\n", stdout);

	while (iter.key != NULL) {
		printf("\t%s = ", cfw_string_c(iter.key));

		if (cfw_is(iter.obj, cfw_string))
			printf("%s\n", cfw_string_c(iter.obj));
		else if (cfw_is(iter.obj, cfw_int))
			printf("%jd\n", cfw_int_value(iter.obj));

		cfw_map_iter_next(&iter);
	}

	fputs("}\n", stdout);
}

int
main(void)
{
	CFWRefPool *pool;
	CFWArray *array;
	CFWString *str, *str2;
	CFWMap *map;
	size_t i;

	pool = cfw_new(cfw_refpool);

	array = cfw_create(cfw_array,
	    cfw_create(cfw_string, "Hallo"),
	    cfw_create(cfw_string, " Welt"),
	    cfw_create(cfw_string, "!"), (void*)NULL);

	str = cfw_new(cfw_string, (void*)NULL);

	for (i = 0; i < cfw_array_size(array); i++)
		cfw_string_append(str, cfw_array_get(array, i));

	cfw_unref(pool);

	puts(cfw_string_c(str));

	pool = cfw_new(cfw_refpool);
	str2 = cfw_create(cfw_string, "ll");
	printf("%zd\n", cfw_string_find(str, str2, cfw_range_all));

	cfw_unref(pool);
	cfw_unref(str);

	pool = cfw_new(cfw_refpool);

	map = cfw_create(cfw_map,
	    cfw_create(cfw_string, "Hallo"),
	    cfw_create(cfw_string, "Welt!"),
	    cfw_create(cfw_string, "Test"),
	    cfw_create(cfw_string, "success!"),
	    cfw_create(cfw_string, "int"),
	    cfw_create(cfw_int, INTMAX_C(1234)), NULL);

	print_map(map);

	cfw_map_set(map,
	    cfw_create(cfw_string, "Hallo"),
	    cfw_create(cfw_string, "Test"));

	print_map(map);

	cfw_map_set(map, cfw_create(cfw_string, "Hallo"), NULL);
	print_map(map);

	cfw_unref(pool);

	return 0;
}
