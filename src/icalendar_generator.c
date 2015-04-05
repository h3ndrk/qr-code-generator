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

#include "icalendar_generator.h"

GString *ical_generate(gchar *description, guint year_start, guint month_start, guint day_start, guint hour_start, guint minute_start, guint year_end, guint month_end, guint day_end, guint hour_end, guint minute_end)
{
	GString *generated_data = NULL;
	
	generated_data = g_string_new("BEGIN:VCALENDAR\nVERSION:2.0\nBEGIN:VEVENT\n");
	g_string_append_printf(generated_data, "DTSTAMP:%i%s%i%s%iT%s%i%s%i00\n", year_start, (month_start<10)?("0"):(""), month_start, (day_start<10)?("0"):(""), day_start, (hour_start<10)?("0"):(""), hour_start, (minute_start<10)?("0"):(""), minute_start);
	g_string_append_printf(generated_data, "DTSTART:%i%s%i%s%iT%s%i%s%i00\n", year_start, (month_start<10)?("0"):(""), month_start, (day_start<10)?("0"):(""), day_start, (hour_start<10)?("0"):(""), hour_start, (minute_start<10)?("0"):(""), minute_start);
	g_string_append_printf(generated_data, "DTEND:%i%s%i%s%iT%s%i%s%i00\n", year_end, (month_end<10)?("0"):(""), month_end, (day_end<10)?("0"):(""), day_end, (hour_end<10)?("0"):(""), hour_end, (minute_end<10)?("0"):(""), minute_end);
	g_string_append_printf(generated_data, "SUMMARY:%s\n", description);
	g_string_append_printf(generated_data, "END:VEVENT\nEND:VCALENDAR");
	
	return generated_data;
}