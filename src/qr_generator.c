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

void qr_render_code(gchar *input)
{
	guint i = 0;
	encoding_mode_t encoding_mode = NUMERIC;
	
	if(input == NULL)
	{
		return;
	}
	
	// test for numeric
	for(i = 0; i < strlen(input); i++)
	{
		if(!(input[i] == '0' ||
			input[i] == '1' ||
			input[i] == '2' ||
			input[i] == '3' ||
			input[i] == '4' ||
			input[i] == '5' ||
			input[i] == '6' ||
			input[i] == '7' ||
			input[i] == '8' ||
			input[i] == '9'))
		{
			encoding_mode = ALPHANUMERIC;
			break;
		}
	}
	
	if(encoding_mode != NUMERIC)
	{
		printf("Numeric does not match\n");
		
		// test for alphanumeric
		for(i = 0; i < strlen(input); i++)
		{
			if(!(input[i] == '0' ||
				input[i] == '1' ||
				input[i] == '2' ||
				input[i] == '3' ||
				input[i] == '4' ||
				input[i] == '5' ||
				input[i] == '6' ||
				input[i] == '7' ||
				input[i] == '8' ||
				input[i] == '9' ||
				input[i] == 'A' ||
				input[i] == 'B' ||
				input[i] == 'C' ||
				input[i] == 'D' ||
				input[i] == 'E' ||
				input[i] == 'F' ||
				input[i] == 'G' ||
				input[i] == 'H' ||
				input[i] == 'I' ||
				input[i] == 'J' ||
				input[i] == 'K' ||
				input[i] == 'L' ||
				input[i] == 'M' ||
				input[i] == 'N' ||
				input[i] == 'O' ||
				input[i] == 'P' ||
				input[i] == 'Q' ||
				input[i] == 'R' ||
				input[i] == 'S' ||
				input[i] == 'T' ||
				input[i] == 'U' ||
				input[i] == 'V' ||
				input[i] == 'W' ||
				input[i] == 'X' ||
				input[i] == 'Y' ||
				input[i] == 'Z' ||
				input[i] == ' ' ||
				input[i] == '$' ||
				input[i] == '%' ||
				input[i] == '*' ||
				input[i] == '+' ||
				input[i] == '-' ||
				input[i] == '.' ||
				input[i] == '/' ||
				input[i] == ':'))
			{
				encoding_mode = BYTE;
				break;
			}
		}
		
		if(encoding_mode != ALPHANUMERIC)
		{
			printf("Alphanumeric does not match\n");
		}
	}
	
	switch(encoding_mode)
	{
		case NUMERIC:
		{
			printf("Numeric encoding\n");
			break;
		}
		case ALPHANUMERIC:
		{
			printf("Alphanumeric encoding\n");
			break;
		}
		case BYTE:
		{
			printf("Byte encoding\n");
			break;
		}
		case KANJI:
		{
			printf("Kanji encoding\n");
			break;
		}
	}
}