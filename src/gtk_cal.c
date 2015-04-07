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
#include <math.h>
#include <gtk/gtk.h>

#include "gtk_window.h"
#include "qr_generator.h"
#include "gtk_cal.h"
#include "icalendar_generator.h"

static GtkWidget *gtk_cal_entry_des = NULL;
static GtkWidget *gtk_cal_date_start = NULL;
static GtkWidget *gtk_cal_date_end = NULL;
static GtkWidget *gtk_cal_time_start_hour = NULL;
static GtkWidget *gtk_cal_time_start_minute = NULL;
static GtkWidget *gtk_cal_time_end_hour = NULL;
static GtkWidget *gtk_cal_time_end_minute = NULL;
// static GtkWidget *gtk_cal_time_zones = NULL;
// static GtkWidget *gtk_cal_time_zones_dst_start = NULL;
// static GtkWidget *gtk_cal_time_zones_dst_end = NULL;
static GtkWidget *gtk_cal_button_file = NULL;
static GtkTextBuffer *gtk_cal_text_buffer = NULL;

static void cb_changed_cal_entry(GtkEditable *editable, gpointer data)
{
	GString *generated_data = NULL;
	guint year_start = 0;
	guint month_start = 0;
	guint day_start = 0;
	guint hour_start = 0;
	guint minute_start = 0;
	guint year_end = 0;
	guint month_end = 0;
	guint day_end = 0;
	guint hour_end = 0;
	guint minute_end = 0;
	// gdouble timezones[] =
	// {
	// 	-12,
	// 	-11,
	// 	-10,
	// 	-9,
	// 	-8,
	// 	-8,
	// 	-7,
	// 	-7,
	// 	-7,
	// 	-6,
	// 	-6,
	// 	-6,
	// 	-6,
	// 	-5,
	// 	-5,
	// 	-5,
	// 	-4,
	// 	-4,
	// 	-4,
	// 	-4,
	// 	-3.5,
	// 	-3,
	// 	-3,
	// 	-3,
	// 	-3,
	// 	-2,
	// 	-1,
	// 	-1,
	// 	0,
	// 	0,
	// 	1,
	// 	1,
	// 	1,
	// 	1,
	// 	1,
	// 	2,
	// 	2,
	// 	2,
	// 	2,
	// 	2,
	// 	2,
	// 	2,
	// 	2,
	// 	2,
	// 	3,
	// 	3,
	// 	3,
	// 	3,
	// 	3.5,
	// 	4,
	// 	4,
	// 	4,
	// 	4.5,
	// 	5,
	// 	5,
	// 	5.5,
	// 	5.5,
	// 	5.75,
	// 	6,
	// 	6,
	// 	6.5,
	// 	7,
	// 	7,
	// 	8,
	// 	8,
	// 	8,
	// 	8,
	// 	8,
	// 	9,
	// 	9,
	// 	9,
	// 	9.5,
	// 	9.5,
	// 	10,
	// 	10,
	// 	10,
	// 	10,
	// 	10,
	// 	11,
	// 	12,
	// 	12,
	// 	13
	// };
	
	UNUSED(editable);
	UNUSED(data);
	
	gtk_calendar_get_date(GTK_CALENDAR(gtk_cal_date_start), &year_start, &month_start, &day_start);
	gtk_calendar_get_date(GTK_CALENDAR(gtk_cal_date_end), &year_end, &month_end, &day_end);
	
	hour_start = gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_cal_time_start_hour));
	minute_start = gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_cal_time_start_minute));
	hour_end = gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_cal_time_end_hour));
	minute_end = gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_cal_time_end_minute));
	
	// hour_start -= timezones[gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_cal_time_zones))];
	// hour_end -= timezones[gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_cal_time_zones))];
	
	// if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_cal_time_zones_dst_start)))
	// {
	// 	hour_start -= 1;
	// }
	
	// if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_cal_time_zones_dst_end)))
	// {
	// 	hour_end -= 1;
	// }
	
	// hour_start %= 24;
	// hour_end %= 24;
	
	generated_data = ical_generate((gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_cal_entry_des)), year_start, month_start + 1, day_start, hour_start, minute_start, year_end, month_end + 1, day_end, hour_end, minute_end);
	
	if(generated_data == NULL)
	{
		return;
	}
	
	gtk_text_buffer_set_text(gtk_cal_text_buffer, generated_data->str, -1);
	
	g_string_free(generated_data, TRUE);
}

static void cb_changed_cal_time(GtkComboBox *widget, gpointer data)
{
	UNUSED(widget);
	UNUSED(data);
	
	cb_changed_cal_entry(NULL, NULL);
}

static void cb_changed_cal_date(GtkCalendar *widget, gpointer data)
{
	UNUSED(widget);
	UNUSED(data);
	
	cb_changed_cal_entry(NULL, NULL);
}

// static void cb_changed_cal_time_zones(GtkCalendar *widget, gpointer data)
// {
// 	UNUSED(widget);
// 	UNUSED(data);
	
// 	cb_changed_cal_entry(NULL, NULL);
// }

static void cb_clicked_cal_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(gtk_cal_entry_des), "");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_start_hour), 12);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_start_minute), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_end_hour), 12);
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_end_minute), 0);
	// gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_zones), 29);
	// gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_cal_time_zones_dst_start), FALSE);
	// gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_cal_time_zones_dst_end), FALSE);
}

void cb_clicked_cal_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *icalendar_data = NULL;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	
	UNUSED(button);
	UNUSED(data);
	
	gtk_text_buffer_get_start_iter(gtk_cal_text_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(gtk_cal_text_buffer, &end_iter);
	icalendar_data = gtk_text_buffer_get_text(gtk_cal_text_buffer, &start_iter, &end_iter, TRUE);
	
	error = qr_render(icalendar_data);
	
	g_free(icalendar_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_set_output();
			
			break;
		}
		case ERR_INVALID_INPUT:
		{
			GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size=\"x-large\">Error</span>\n\nFailed to generate QR code: Invalid input.\n\nTry to type something into the input.");
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			break;
		}
		case ERR_NO_MEMORY:
		{
			GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size=\"x-large\">Error</span>\n\nFailed to generate QR code: Failed to allocate memory for input.\n\nThis means that your systems tells this program that no more memory is available. Try to close some programs.");
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			break;
		}
		case ERR_RANGE:
		{
			GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size=\"x-large\">Error</span>\n\nFailed to generate QR code: Input data is too large.\n\nQR codes have a maximum size of input data. Try to shorten your input data.");
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			break;
		}
	}
}

static void cb_file_set_cal(GtkFileChooserButton *widget, gpointer data)
{
	gchar *filename = NULL;
	gchar *file_contents = NULL;
	GError *error = NULL;
	
	UNUSED(data);
	
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
	
	if(filename == NULL)
	{
		GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size=\"x-large\">Error</span>\n\nFailed to retrieve path of selected file: No file selected.");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
	else
	{
		g_file_get_contents(filename, &file_contents, NULL, &error);
		
		if(error != NULL)
		{
			fprintf(stderr, "Failed to open file %s: %s\n", filename, error->message);
			
			GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size=\"x-large\">Error</span>\n\nFailed to open file: %s\n\n%s", filename, error->message);
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			
			g_error_free(error);
		}
		
		gtk_text_buffer_set_text(gtk_cal_text_buffer, file_contents, -1);
		
		g_free(file_contents);
		g_free(filename);
	}
}

GtkWidget *gtk_cal_qr_new(void)
{
	GtkWidget *gtk_cal_label = gtk_label_new(NULL);
	GtkWidget *gtk_cal_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_cal_scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_cal_entry_des = gtk_entry_new();
	gtk_cal_date_start = gtk_calendar_new();
	gtk_cal_date_end = gtk_calendar_new();
	GtkWidget *gtk_cal_label_start = gtk_label_new("Start date and time");
	GtkWidget *gtk_cal_label_end = gtk_label_new("End date and time");
	GtkWidget *gtk_cal_label_start_colon = gtk_label_new(":");
	GtkWidget *gtk_cal_label_end_colon = gtk_label_new(":");
	gtk_cal_time_start_hour = gtk_combo_box_text_new();
	gtk_cal_time_start_minute = gtk_combo_box_text_new();
	gtk_cal_time_end_hour = gtk_combo_box_text_new();
	gtk_cal_time_end_minute = gtk_combo_box_text_new();
	// gtk_cal_time_zones = gtk_combo_box_text_new();
	// gtk_cal_time_zones_dst_start = gtk_check_button_new_with_label("Summer time (DST)");
	// gtk_cal_time_zones_dst_end = gtk_check_button_new_with_label("Summer time (DST)");
	GtkWidget *gtk_cal_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_cal_horizontal_date_pickers = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	GtkWidget *gtk_cal_vertical_date_pickers_start = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_cal_vertical_date_pickers_end = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_cal_horizontal_date_pickers_start_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *gtk_cal_horizontal_date_pickers_end_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *gtk_cal_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *gtk_cal_description_label = gtk_label_new("Either iCalendar file import or manual information entering are supported. Modify the generated iCalendar at the bottom.");
	GtkWidget *gtk_cal_button_file_label = gtk_label_new("iCalendar (.ics) file");
	GtkWidget *gtk_cal_button_file_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	gtk_cal_button_file = gtk_file_chooser_button_new("Select a .ics (iCalendar) file", GTK_FILE_CHOOSER_ACTION_OPEN);
	GtkWidget *gtk_cal_text_view_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_cal_text_view = gtk_text_view_new();
	gtk_cal_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_cal_text_view));
	
	gtk_label_set_markup(GTK_LABEL(gtk_cal_label), "<span size=\"xx-large\">Generate from calendar event</span>");
	gtk_widget_set_halign(gtk_cal_label, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_cal_label_start, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_cal_label_end, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_cal_description_label, GTK_ALIGN_START);
	gtk_label_set_line_wrap(GTK_LABEL(gtk_cal_description_label), TRUE);
	gtk_label_set_justify(GTK_LABEL(gtk_cal_description_label), GTK_JUSTIFY_FILL);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_hour), 23, "23", "23");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_start_hour), 12);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 23, "23", "23");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 24, "24", "24");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 25, "25", "25");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 26, "26", "26");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 27, "27", "27");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 28, "28", "28");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 29, "29", "29");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 30, "30", "30");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 31, "31", "31");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 32, "32", "32");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 33, "33", "33");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 34, "34", "34");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 35, "35", "35");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 36, "36", "36");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 37, "37", "37");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 38, "38", "38");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 39, "39", "39");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 40, "40", "40");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 41, "41", "41");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 42, "42", "42");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 43, "43", "43");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 44, "44", "44");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 45, "45", "45");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 46, "46", "46");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 47, "47", "47");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 48, "48", "48");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 49, "49", "49");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 50, "50", "50");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 51, "51", "51");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 52, "52", "52");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 53, "53", "53");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 54, "54", "54");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 55, "55", "55");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 56, "56", "56");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 57, "57", "57");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 58, "58", "58");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_start_minute), 59, "59", "59");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_start_minute), 0);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_hour), 23, "23", "23");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_end_hour), 12);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 23, "23", "23");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 24, "24", "24");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 25, "25", "25");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 26, "26", "26");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 27, "27", "27");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 28, "28", "28");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 29, "29", "29");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 30, "30", "30");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 31, "31", "31");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 32, "32", "32");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 33, "33", "33");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 34, "34", "34");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 35, "35", "35");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 36, "36", "36");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 37, "37", "37");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 38, "38", "38");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 39, "39", "39");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 40, "40", "40");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 41, "41", "41");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 42, "42", "42");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 43, "43", "43");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 44, "44", "44");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 45, "45", "45");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 46, "46", "46");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 47, "47", "47");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 48, "48", "48");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 49, "49", "49");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 50, "50", "50");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 51, "51", "51");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 52, "52", "52");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 53, "53", "53");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 54, "54", "54");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 55, "55", "55");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 56, "56", "56");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 57, "57", "57");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 58, "58", "58");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_end_minute), 59, "59", "59");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_end_minute), 0);
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 0, "0", "(GMT-12:00) International Date Line West");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 1, "1", "(GMT-11:00) Midway Island, Samoa");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 2, "2", "(GMT-10:00) Hawaii");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 3, "3", "(GMT-09:00) Alaska");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 4, "4", "(GMT-08:00) Pacific Time (US & Canada)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 5, "5", "(GMT-08:00) Tijuana, Baja California");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 6, "6", "(GMT-07:00) Arizona");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 7, "7", "(GMT-07:00) Chihuahua, La Paz, Mazatlan");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 8, "8", "(GMT-07:00) Mountain Time (US & Canada)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 9, "9", "(GMT-06:00) Central America");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 10, "10", "(GMT-06:00) Central Time (US & Canada)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 11, "11", "(GMT-06:00) Guadalajara, Mexico City, Monterrey");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 12, "12", "(GMT-06:00) Saskatchewan");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 13, "13", "(GMT-05:00) Bogota, Lima, Quito, Rio Branco");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 14, "14", "(GMT-05:00) Eastern Time (US & Canada)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 15, "15", "(GMT-05:00) Indiana (East)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 16, "16", "(GMT-04:00) Atlantic Time (Canada)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 17, "17", "(GMT-04:00) Caracas, La Paz");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 18, "18", "(GMT-04:00) Manaus");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 19, "19", "(GMT-04:00) Santiago");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 20, "20", "(GMT-03:30) Newfoundland");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 21, "21", "(GMT-03:00) Brasilia");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 22, "22", "(GMT-03:00) Buenos Aires, Georgetown");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 23, "23", "(GMT-03:00) Greenland");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 24, "24", "(GMT-03:00) Montevideo");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 25, "25", "(GMT-02:00) Mid-Atlantic");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 26, "26", "(GMT-01:00) Cape Verde Is.");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 27, "27", "(GMT-01:00) Azores");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 28, "28", "(GMT+00:00) Casablanca, Monrovia, Reykjavik");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 29, "29", "(GMT+00:00) Greenwich Mean Time: Dublin, Edinburgh, Lisbon, London");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 30, "30", "(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 31, "31", "(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 32, "32", "(GMT+01:00) Brussels, Copenhagen, Madrid, Paris");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 33, "33", "(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 34, "34", "(GMT+01:00) West Central Africa");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 35, "35", "(GMT+02:00) Amman");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 36, "36", "(GMT+02:00) Athens, Bucharest, Istanbul");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 37, "37", "(GMT+02:00) Beirut");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 38, "38", "(GMT+02:00) Cairo");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 39, "39", "(GMT+02:00) Harare, Pretoria");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 40, "40", "(GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 41, "41", "(GMT+02:00) Jerusalem");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 42, "42", "(GMT+02:00) Minsk");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 43, "43", "(GMT+02:00) Windhoek");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 44, "44", "(GMT+03:00) Kuwait, Riyadh, Baghdad");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 45, "45", "(GMT+03:00) Moscow, St. Petersburg, Volgograd");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 46, "46", "(GMT+03:00) Nairobi");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 47, "47", "(GMT+03:00) Tbilisi");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 48, "48", "(GMT+03:30) Tehran");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 49, "49", "(GMT+04:00) Abu Dhabi, Muscat");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 50, "50", "(GMT+04:00) Baku");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 51, "51", "(GMT+04:00) Yerevan");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 52, "52", "(GMT+04:30) Kabul");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 53, "53", "(GMT+05:00) Yekaterinburg");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 54, "54", "(GMT+05:00) Islamabad, Karachi, Tashkent");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 55, "55", "(GMT+05:30) Sri Jayawardenapura");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 56, "56", "(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 57, "57", "(GMT+05:45) Kathmandu");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 58, "58", "(GMT+06:00) Almaty, Novosibirsk");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 59, "59", "(GMT+06:00) Astana, Dhaka");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 60, "60", "(GMT+06:30) Yangon (Rangoon)");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 61, "61", "(GMT+07:00) Bangkok, Hanoi, Jakarta");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 62, "62", "(GMT+07:00) Krasnoyarsk");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 63, "63", "(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 64, "64", "(GMT+08:00) Kuala Lumpur, Singapore");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 65, "65", "(GMT+08:00) Irkutsk, Ulaan Bataar");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 66, "66", "(GMT+08:00) Perth");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 67, "67", "(GMT+08:00) Taipei");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 68, "68", "(GMT+09:00) Osaka, Sapporo, Tokyo");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 69, "69", "(GMT+09:00) Seoul");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 70, "70", "(GMT+09:00) Yakutsk");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 71, "71", "(GMT+09:30) Adelaide");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 72, "72", "(GMT+09:30) Darwin");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 73, "73", "(GMT+10:00) Brisbane");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 74, "74", "(GMT+10:00) Canberra, Melbourne, Sydney");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 75, "75", "(GMT+10:00) Hobart");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 76, "76", "(GMT+10:00) Guam, Port Moresby");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 77, "77", "(GMT+10:00) Vladivostok");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 78, "78", "(GMT+11:00) Magadan, Solomon Is., New Caledonia");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 79, "79", "(GMT+12:00) Auckland, Wellington");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 80, "80", "(GMT+12:00) Fiji, Kamchatka, Marshall Is.");
	// gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_cal_time_zones), 81, "81", "(GMT+13:00) Nuku'alofa");
	// gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_cal_time_zones), 29);
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_cal_entry_des), "Event description");
	// gtk_entry_set_text(GTK_ENTRY(gtk_cal_entry_des), "Event description");
	gtk_container_set_border_width(GTK_CONTAINER(gtk_cal_vertical), 15);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(gtk_cal_text_view), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(gtk_cal_text_view), 5);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(gtk_cal_text_view), 2);
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(gtk_cal_text_view), 2);
	// gtk_text_view_set_monospace(GTK_TEXT_VIEW(gtk_cal_text_view), TRUE);
	GtkFileFilter *filter_cal = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter_cal, "*.ics");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(gtk_cal_button_file), filter_cal);
	cb_changed_cal_entry(NULL, NULL); // initialize icalendar
	
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_description_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_button_file_box), gtk_cal_button_file_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_button_file_box), gtk_cal_button_file, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_button_file_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_entry_des, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_start), gtk_cal_label_start, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_start), gtk_cal_date_start, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers_start_time), gtk_cal_time_start_hour, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers_start_time), gtk_cal_label_start_colon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers_start_time), gtk_cal_time_start_minute, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_start), gtk_cal_horizontal_date_pickers_start_time, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_start), gtk_cal_time_zones_dst_start, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_end), gtk_cal_label_end, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_end), gtk_cal_date_end, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers_end_time), gtk_cal_time_end_hour, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers_end_time), gtk_cal_label_end_colon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers_end_time), gtk_cal_time_end_minute, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_end), gtk_cal_horizontal_date_pickers_end_time, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(gtk_cal_vertical_date_pickers_end), gtk_cal_time_zones_dst_end, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers), gtk_cal_vertical_date_pickers_start, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_horizontal_date_pickers), gtk_cal_vertical_date_pickers_end, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_horizontal_date_pickers, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_time_zones, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_cal_text_view_scrolled), gtk_cal_text_view);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_text_view_scrolled, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(gtk_cal_horizontal_buttons), gtk_cal_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_cal_vertical), gtk_cal_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_cal_scrolled), gtk_cal_vertical);
	
	g_signal_connect(G_OBJECT(gtk_cal_button_clear), "clicked", G_CALLBACK(cb_clicked_cal_clear), NULL);
	
	g_signal_connect(G_OBJECT(gtk_cal_button_file), "file-set", G_CALLBACK(cb_file_set_cal), NULL);
	
	g_signal_connect(G_OBJECT(gtk_cal_entry_des), "changed", G_CALLBACK(cb_changed_cal_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_cal_time_start_hour), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(gtk_cal_time_start_minute), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(gtk_cal_time_end_hour), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(gtk_cal_time_end_minute), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	// g_signal_connect(G_OBJECT(gtk_cal_time_zones), "changed", G_CALLBACK(cb_changed_cal_time_zones), NULL);
	
	g_signal_connect(G_OBJECT(gtk_cal_date_start), "day-selected", G_CALLBACK(cb_changed_cal_date), NULL);
	g_signal_connect(G_OBJECT(gtk_cal_date_end), "day-selected", G_CALLBACK(cb_changed_cal_date), NULL);
	
	// g_signal_connect(G_OBJECT(gtk_cal_time_zones_dst_start), "toggled", G_CALLBACK(cb_toggled_cal_dst), NULL);
	// g_signal_connect(G_OBJECT(gtk_cal_time_zones_dst_end), "toggled", G_CALLBACK(cb_toggled_cal_dst), NULL);
	
	return gtk_cal_scrolled;
}