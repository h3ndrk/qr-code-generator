/**
 * Copyright (C) 2015 NIPE-SYSTEMS
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "qr_generator.h"

static GArray *qr_code = NULL;
static gint qr_code_size = 0;

void qr_initialize_array(gint size)
{
	gint i = 0;
	gboolean temp = FALSE;
	
	qr_code = g_array_new(FALSE, TRUE, sizeof(gboolean));
	
	for(i = 0; i < size * size; i++)
	{
		g_array_append_val(qr_code, temp);
	}
	
	qr_code_size = size;
}

void qr_free_array(void)
{
	g_array_free(qr_code, TRUE);
}

gboolean qr_get_pixel(gint x, gint y)
{
	if(qr_code == NULL)
	{
		return FALSE;
	}
	
	return g_array_index(qr_code, gboolean, y * qr_code_size + x);
}

void qr_set_pixel(gint x, gint y, gboolean value)
{
	g_array_index(qr_code, gboolean, y * qr_code_size + x) = value;
}