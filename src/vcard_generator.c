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

#include "vcard_generator.h"

GString *vcard_generate(gchar *first_name, gchar *last_name, gchar *title, gchar *street, gchar *postal_code, gchar *city, gchar *country, gchar *company, gchar *email_personal, gchar *email_business, gchar *phone_personal, gchar *phone_mobile, gchar *phone_business, gchar *website)
{
	GString *generated_data = NULL;
	
	generated_data = g_string_new("BEGIN:VCARD\nVERSION:3.0\n");
	g_string_append_printf(generated_data, "N:%s;%s;;;\n", last_name, first_name);
	g_string_append_printf(generated_data, "FN:%s %s\n", first_name, last_name);
	if(strlen(company) > 0) g_string_append_printf(generated_data, "ORG:%s\n", company);
	if(strlen(title) > 0) g_string_append_printf(generated_data, "TITLE:%s\n", title);
	if(strlen(email_personal) > 0) g_string_append_printf(generated_data, "EMAIL;type=HOME:%s\n", email_personal);
	if(strlen(email_business) > 0) g_string_append_printf(generated_data, "EMAIL;type=WORK:%s\n", email_business);
	if(strlen(phone_personal) > 0) g_string_append_printf(generated_data, "TEL;type=HOME:%s\n", phone_personal);
	if(strlen(phone_mobile) > 0) g_string_append_printf(generated_data, "TEL;type=CELL:%s\n", phone_mobile);
	if(strlen(phone_business) > 0) g_string_append_printf(generated_data, "TEL;type=WORK:%s\n", phone_business);
	if(strlen(street) > 0 || strlen(city) > 0 || strlen(country) > 0) g_string_append_printf(generated_data, "ADR:;;%s;%s;;%s;%s\n", street, city, postal_code, country);
	if(strlen(website) > 0) g_string_append_printf(generated_data, "URL:%s\n", website);
	g_string_append_printf(generated_data, "END:VCARD");
	
	return generated_data;
}