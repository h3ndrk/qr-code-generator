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
#include "vcard_generator.h"
#include "icalendar_generator.h"

static GtkWidget *window = NULL;
static GtkWidget *headerbar = NULL;
static GtkWidget *stack = NULL;
static GtkWidget *drawing = NULL;
static GtkWidget *text_entry = NULL;
static GtkWidget *contact_button_file = NULL;
static GtkWidget *contact_entry_first_name = NULL;
static GtkWidget *contact_entry_last_name = NULL;
static GtkWidget *contact_entry_title = NULL;
static GtkWidget *contact_entry_street = NULL;
static GtkWidget *contact_entry_postal_code = NULL;
static GtkWidget *contact_entry_city = NULL;
static GtkWidget *contact_entry_country = NULL;
static GtkWidget *contact_entry_company = NULL;
static GtkWidget *contact_entry_email_personal = NULL;
static GtkWidget *contact_entry_email_business = NULL;
static GtkWidget *contact_entry_phone_personal = NULL;
static GtkWidget *contact_entry_phone_mobile = NULL;
static GtkWidget *contact_entry_phone_business = NULL;
static GtkWidget *contact_entry_website = NULL;
static GtkTextBuffer *contact_text_buffer = NULL;
static GtkWidget *sms_entry_phone = NULL;
static GtkWidget *sms_entry_text = NULL;
static GtkWidget *sms_label_size = NULL;
static GtkWidget *call_entry = NULL;
static GtkWidget *geo_entry_latitude = NULL;
static GtkWidget *geo_entry_longitude = NULL;
static GtkWidget *geo_radio_north = NULL;
static GtkWidget *geo_radio_south = NULL;
static GtkWidget *geo_radio_west = NULL;
static GtkWidget *geo_radio_east = NULL;
static GtkWidget *cal_entry_des = NULL;
static GtkWidget *cal_date_start = NULL;
static GtkWidget *cal_date_end = NULL;
static GtkWidget *cal_time_start_hour = NULL;
static GtkWidget *cal_time_start_minute = NULL;
static GtkWidget *cal_time_end_hour = NULL;
static GtkWidget *cal_time_end_minute = NULL;
static GtkWidget *cal_button_file = NULL;
static GtkTextBuffer *cal_text_buffer = NULL;
static GtkWidget *mail_entry_to = NULL;
static GtkWidget *mail_entry_subject = NULL;
static GtkTextBuffer *mail_text_buffer = NULL;
static GtkWidget *wlan_entry_ssid = NULL;
static GtkWidget *wlan_entry_psk = NULL;
static GtkWidget *wlan_check_hidden = NULL;
static GtkWidget *wlan_combo_auth = NULL;
static GtkWidget *stack_switcher_sidebar = NULL;

static gboolean cb_drawing(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	guint width = 0;
	guint height = 0;
	guint size = 0;
	guint offset_x = 0;
	guint offset_y = 0;
	GArray *qr_code_data = NULL;
	gint qr_code_size = 0;
	guint x = 0;
	guint y = 0;
	guint index = 0;
	
	UNUSED(data);
	
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);
	
	if(width > height)
	{
		size = height;
		offset_x = (width - height) / 2;
	}
	else
	{
		size = width;
		offset_y = (height - width) / 2;
	}
	
	// white background
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr, offset_x, offset_y, size, size);
	cairo_fill(cr);
	
	qr_code_data = qr_get_pixels();
	qr_code_size = qr_get_size();
	
	// QR code rendering
	if(qr_code_data != NULL)
	{
		for(y = 0; y < (guint)qr_code_size; y++)
		{
			for(x = 0; x < (guint)qr_code_size; x++)
			{
				index = (y * qr_code_size) + x;
				if(g_array_index(qr_code_data, gboolean, index))
				{
					cairo_set_source_rgb(cr, 0, 0, 0);
					cairo_rectangle(cr, offset_x + floor((x + 1) * ((float)size / (qr_code_size + 2))), offset_y + floor((y + 1) * ((float)size / (qr_code_size + 2))), ceil((float)size / (qr_code_size + 2)), ceil((float)size / (qr_code_size + 2)));
					cairo_fill(cr);
				}
			}
		}
	}
	
	return FALSE;
}

static void cb_clicked_text_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	
	UNUSED(button);
	UNUSED(data);
	
	error = qr_render((gchar *)gtk_entry_get_text(GTK_ENTRY(text_entry)));
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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
			GtkWidget *dialog = gtk_message_dialog_new_with_markup(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "<span size=\"x-large\">Error</span>\n\nFailed to generate QR code: Input data is too large.\n\nQR codes have a maximum size of input data. Try to shorten your input text or URL.");
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			break;
		}
	}
}

static void cb_clicked_text_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(text_entry), "");
	gtk_widget_grab_focus(text_entry);
}

static void cb_clicked_contact_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *vcard_data = NULL;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	
	UNUSED(button);
	UNUSED(data);
	
	gtk_text_buffer_get_start_iter(contact_text_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(contact_text_buffer, &end_iter);
	vcard_data = gtk_text_buffer_get_text(contact_text_buffer, &start_iter, &end_iter, TRUE);
	
	error = qr_render(vcard_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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

static void cb_file_set_contact(GtkFileChooserButton *widget, gpointer data)
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
		
		gtk_text_buffer_set_text(contact_text_buffer, file_contents, -1);
		
		g_free(file_contents);
		g_free(filename);
	}
}

static void cb_changed_contact_entry(GtkEditable *editable, gpointer data)
{
	GString *generated_data = NULL;
	
	UNUSED(editable);
	UNUSED(data);
	
	generated_data = vcard_generate((gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_first_name)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_last_name)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_title)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_street)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_postal_code)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_city)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_country)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_company)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_email_personal)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_email_business)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_phone_personal)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_phone_mobile)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_phone_business)), (gchar *)gtk_entry_get_text(GTK_ENTRY(contact_entry_website)));
	
	if(generated_data == NULL)
	{
		return;
	}
	
	gtk_text_buffer_set_text(contact_text_buffer, generated_data->str, -1);
	
	g_string_free(generated_data, TRUE);
}

static void cb_clicked_contact_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(contact_entry_first_name), "First name");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_last_name), "Last name");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_title), "Title");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_street), "Street");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_postal_code), "Postal code");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_city), "City");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_country), "Country");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_company), "Company/organization");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_email_personal), "Email (personal)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_email_business), "Email (business)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_phone_personal), "Phone (personal)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_phone_mobile), "Phone (mobile)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_phone_business), "Phone (business)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_website), "Website");
}

static void cb_clicked_sms_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *sms_data = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	sms_data = g_strdup_printf("SMSTO:%s:%s", (gchar *)gtk_entry_get_text(GTK_ENTRY(sms_entry_phone)), (gchar *)gtk_entry_get_text(GTK_ENTRY(sms_entry_text)));
	
	error = qr_render(sms_data);
	
	g_free(sms_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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

static void cb_clicked_sms_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(sms_entry_phone), "");
	gtk_entry_set_text(GTK_ENTRY(sms_entry_text), "");
}

static void cb_changed_sms_entry(GtkEditable *editable, gpointer data)
{
	gchar *character_data = NULL;
	
	UNUSED(editable);
	UNUSED(data);
	
	character_data = g_strdup_printf("%li character%s", strlen((gchar *)gtk_entry_get_text(GTK_ENTRY(sms_entry_text))), (strlen((gchar *)gtk_entry_get_text(GTK_ENTRY(sms_entry_text)))!=1)?("s"):(""));
	
	gtk_label_set_text(GTK_LABEL(sms_label_size), character_data);
	
	g_free(character_data);
}

static void cb_clicked_call_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *call_data = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	call_data = g_strdup_printf("TEL:%s", (gchar *)gtk_entry_get_text(GTK_ENTRY(call_entry)));
	
	error = qr_render(call_data);
	
	g_free(call_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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

static void cb_clicked_call_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(call_entry), "");
}

static void cb_clicked_geo_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *geo_data = NULL;
	gdouble latitude = 0;
	gdouble longitude = 0;
	
	UNUSED(button);
	UNUSED(data);
	
	latitude = gtk_spin_button_get_value(GTK_SPIN_BUTTON(geo_entry_latitude));
	longitude = gtk_spin_button_get_value(GTK_SPIN_BUTTON(geo_entry_longitude));
	
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(geo_radio_north)))
	{
		latitude *= -1;
	}
	
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(geo_radio_west)))
	{
		longitude *= -1;
	}
	
	geo_data = g_strdup_printf("GEO:%f,%f,0", latitude, longitude);
	
	error = qr_render(geo_data);
	
	g_free(geo_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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

static void cb_clicked_geo_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(geo_entry_latitude), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(geo_entry_longitude), 0);
}

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
	
	UNUSED(editable);
	UNUSED(data);
	
	gtk_calendar_get_date(GTK_CALENDAR(cal_date_start), &year_start, &month_start, &day_start);
	gtk_calendar_get_date(GTK_CALENDAR(cal_date_end), &year_end, &month_end, &day_end);
	
	hour_start = gtk_combo_box_get_active(GTK_COMBO_BOX(cal_time_start_hour));
	minute_start = gtk_combo_box_get_active(GTK_COMBO_BOX(cal_time_start_minute));
	hour_end = gtk_combo_box_get_active(GTK_COMBO_BOX(cal_time_end_hour));
	minute_end = gtk_combo_box_get_active(GTK_COMBO_BOX(cal_time_end_minute));
	
	generated_data = ical_generate((gchar *)gtk_entry_get_text(GTK_ENTRY(cal_entry_des)), year_start, month_start + 1, day_start, hour_start, minute_start, year_end, month_end + 1, day_end, hour_end, minute_end);
	
	if(generated_data == NULL)
	{
		return;
	}
	
	gtk_text_buffer_set_text(cal_text_buffer, generated_data->str, -1);
	
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

static void cb_clicked_cal_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(cal_entry_des), "Entry description");
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_start_hour), 12);
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_start_minute), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_end_hour), 12);
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_end_minute), 0);
}

static void cb_clicked_cal_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *icalendar_data = NULL;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	
	UNUSED(button);
	UNUSED(data);
	
	gtk_text_buffer_get_start_iter(cal_text_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(cal_text_buffer, &end_iter);
	icalendar_data = gtk_text_buffer_get_text(cal_text_buffer, &start_iter, &end_iter, TRUE);
	
	error = qr_render(icalendar_data);
	
	g_free(icalendar_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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
		
		gtk_text_buffer_set_text(cal_text_buffer, file_contents, -1);
		
		g_free(file_contents);
		g_free(filename);
	}
}

static void cb_clicked_mail_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *mail_data = NULL;
	gchar *mail_data_message = NULL;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	
	UNUSED(button);
	UNUSED(data);
	
	gtk_text_buffer_get_start_iter(mail_text_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(mail_text_buffer, &end_iter);
	mail_data_message = gtk_text_buffer_get_text(mail_text_buffer, &start_iter, &end_iter, TRUE);
	
	mail_data = g_strdup_printf("MATMSG:TO:%s;SUB:%s;BODY:%s;;", (gchar *)gtk_entry_get_text(GTK_ENTRY(mail_entry_to)), (gchar *)gtk_entry_get_text(GTK_ENTRY(mail_entry_subject)), mail_data_message);
	
	error = qr_render(mail_data);
	
	g_free(mail_data_message);
	g_free(mail_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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

static void cb_clicked_mail_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(mail_entry_to), "");
	gtk_entry_set_text(GTK_ENTRY(mail_entry_subject), "");
	gtk_text_buffer_set_text(mail_text_buffer, "E-mail message", -1);
}

static void cb_changed_wlan_combo(GtkComboBox *widget, gpointer data)
{
	UNUSED(data);
	
	// gtk_stack_set_visible_child_name(GTK_STACK(stack), gtk_combo_box_get_active_id(widget));
	
	if(gtk_combo_box_get_active(widget) == 0)
	{
		gtk_widget_set_sensitive(wlan_entry_psk, FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(wlan_entry_psk, TRUE);
	}
}

static void cb_clicked_wlan_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *wlan_data = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	if(gtk_combo_box_get_active(GTK_COMBO_BOX(wlan_combo_auth)) == 0)
	{
		wlan_data = g_strdup_printf("WIFI:T:%s;S:%s;;H:%s;", (gchar *)gtk_combo_box_get_active_id(GTK_COMBO_BOX(wlan_combo_auth)), (gchar *)gtk_entry_get_text(GTK_ENTRY(wlan_entry_ssid)), (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wlan_check_hidden)))?("true"):("false"));
	}
	else
	{
		wlan_data = g_strdup_printf("WIFI:T:%s;S:%s;P:%s;H:%s;", (gchar *)gtk_combo_box_get_active_id(GTK_COMBO_BOX(wlan_combo_auth)), (gchar *)gtk_entry_get_text(GTK_ENTRY(wlan_entry_ssid)), (gchar *)gtk_entry_get_text(GTK_ENTRY(wlan_entry_psk)), (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wlan_check_hidden)))?("true"):("false"));
	}
	
	error = qr_render(wlan_data);
	
	g_free(wlan_data);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			gtk_list_box_select_row(GTK_LIST_BOX(stack_switcher_sidebar), gtk_list_box_get_row_at_index(GTK_LIST_BOX(stack_switcher_sidebar), 8));
			
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

static void cb_clicked_wlan_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(wlan_entry_ssid), "");
	gtk_entry_set_text(GTK_ENTRY(wlan_entry_psk), "");
	gtk_combo_box_set_active(GTK_COMBO_BOX(wlan_combo_auth), 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wlan_check_hidden), FALSE);
}

static void cb_selected_stack_switcher(GtkListBox *box, GtkListBoxRow *row, gpointer data)
{
	gchar *stack_names[] = 
	{
		"input_text",
		"input_contact",
		"input_sms",
		"input_call",
		"input_geo",
		"input_cal",
		"input_mail",
		"input_wlan",
		"output_code"
	};
	
	UNUSED(box);
	UNUSED(data);
	
	// printf("Selected index: %i, %s\n", gtk_list_box_row_get_index(row), stack_names[gtk_list_box_row_get_index(row)]);
	gtk_stack_set_visible_child_name(GTK_STACK(stack), stack_names[gtk_list_box_row_get_index(row)]);
}

void gtk_window_init(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 612, 512);
	gtk_window_set_title(GTK_WINDOW(window), "QR-Code Generator");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *stack_switcher_sidebar_box_text = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_text = gtk_label_new("Text/URL");
	GtkWidget *stack_switcher_sidebar_box_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_contact = gtk_label_new("Contact");
	GtkWidget *stack_switcher_sidebar_box_sms = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_sms = gtk_label_new("SMS");
	GtkWidget *stack_switcher_sidebar_box_call = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_call = gtk_label_new("Call/Phone number");
	GtkWidget *stack_switcher_sidebar_box_geo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_geo = gtk_label_new("Geolocation");
	GtkWidget *stack_switcher_sidebar_box_cal = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_cal = gtk_label_new("Calendar event");
	GtkWidget *stack_switcher_sidebar_box_mail = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_mail = gtk_label_new("E-Mail");
	GtkWidget *stack_switcher_sidebar_box_wlan = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_wlan = gtk_label_new("WLAN");
	GtkWidget *stack_switcher_sidebar_box_code = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *stack_switcher_sidebar_label_code = gtk_label_new(NULL);
	stack_switcher_sidebar = gtk_list_box_new();
	GtkWidget *root_horizontal = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	headerbar = gtk_header_bar_new();
	stack = gtk_stack_new();
	drawing = gtk_drawing_area_new();
	GtkWidget *text_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *text_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *text_label = gtk_label_new(NULL);
	text_entry = gtk_entry_new();
	GtkWidget *text_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *text_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *text_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *contact_label = gtk_label_new(NULL);
	GtkWidget *contact_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *contact_scrolled = gtk_scrolled_window_new(NULL, NULL);
	contact_button_file = gtk_file_chooser_button_new("Select a .vcf (vCard) contact", GTK_FILE_CHOOSER_ACTION_OPEN);
	GtkWidget *contact_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *contact_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *contact_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	contact_entry_first_name = gtk_entry_new();
	contact_entry_last_name = gtk_entry_new();
	contact_entry_title = gtk_entry_new();
	contact_entry_street = gtk_entry_new();
	contact_entry_postal_code = gtk_entry_new();
	contact_entry_city = gtk_entry_new();
	contact_entry_country = gtk_entry_new();
	contact_entry_company = gtk_entry_new();
	contact_entry_email_personal = gtk_entry_new();
	contact_entry_email_business = gtk_entry_new();
	contact_entry_phone_personal = gtk_entry_new();
	contact_entry_phone_mobile = gtk_entry_new();
	contact_entry_phone_business = gtk_entry_new();
	contact_entry_website = gtk_entry_new();
	GtkWidget *contact_horizontal_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_horizontal_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_horizontal_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_horizontal_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_horizontal_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_horizontal_5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_horizontal_6 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *contact_text_view = gtk_text_view_new();
	contact_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(contact_text_view));
	GtkWidget *sms_label = gtk_label_new(NULL);
	GtkWidget *sms_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *sms_scrolled = gtk_scrolled_window_new(NULL, NULL);
	sms_entry_phone = gtk_entry_new();
	sms_entry_text = gtk_entry_new();
	sms_label_size = gtk_label_new("0 characters");
	GtkWidget *sms_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *sms_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *sms_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *call_label = gtk_label_new(NULL);
	GtkWidget *call_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *call_scrolled = gtk_scrolled_window_new(NULL, NULL);
	call_entry = gtk_entry_new();
	GtkWidget *call_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *call_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *call_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *geo_label = gtk_label_new(NULL);
	GtkWidget *geo_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *geo_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *geo_label_latitude = gtk_label_new("Latitude");
	GtkWidget *geo_label_longitude = gtk_label_new("Longitude");
	geo_entry_latitude = gtk_spin_button_new_with_range(-90, 90, 0.01);
	geo_entry_longitude = gtk_spin_button_new_with_range(-180, 180, 0.01);
	geo_radio_north = gtk_radio_button_new_with_label(NULL, "North");
	geo_radio_south = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(geo_radio_north), "South");
	geo_radio_west = gtk_radio_button_new_with_label(NULL, "West");
	geo_radio_east = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(geo_radio_west), "East");
	GtkWidget *geo_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *geo_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *geo_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *geo_horizontal_coords = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	GtkWidget *geo_vertical_coords_latitude = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *geo_vertical_coords_longitude = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *cal_label = gtk_label_new(NULL);
	GtkWidget *cal_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *cal_scrolled = gtk_scrolled_window_new(NULL, NULL);
	cal_entry_des = gtk_entry_new();
	cal_date_start = gtk_calendar_new();
	cal_date_end = gtk_calendar_new();
	GtkWidget *cal_label_start = gtk_label_new("Start date and time");
	GtkWidget *cal_label_end = gtk_label_new("End date and time");
	GtkWidget *cal_label_start_colon = gtk_label_new(":");
	GtkWidget *cal_label_end_colon = gtk_label_new(":");
	cal_time_start_hour = gtk_combo_box_text_new();
	cal_time_start_minute = gtk_combo_box_text_new();
	cal_time_end_hour = gtk_combo_box_text_new();
	cal_time_end_minute = gtk_combo_box_text_new();
	GtkWidget *cal_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *cal_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *cal_horizontal_date_pickers = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	GtkWidget *cal_vertical_date_pickers_start = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *cal_vertical_date_pickers_end = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *cal_horizontal_date_pickers_start_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *cal_horizontal_date_pickers_end_time = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *cal_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	cal_button_file = gtk_file_chooser_button_new("Select a .ics (iCalendar) file", GTK_FILE_CHOOSER_ACTION_OPEN);
	GtkWidget *cal_text_view = gtk_text_view_new();
	cal_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(cal_text_view));
	GtkWidget *mail_label = gtk_label_new(NULL);
	GtkWidget *mail_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *mail_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *mail_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *mail_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *mail_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	mail_entry_to = gtk_entry_new();
	mail_entry_subject = gtk_entry_new();
	GtkWidget *mail_text_view = gtk_text_view_new();
	mail_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mail_text_view));
	GtkWidget *wlan_label = gtk_label_new(NULL);
	GtkWidget *wlan_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *wlan_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *wlan_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *wlan_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *wlan_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	wlan_combo_auth = gtk_combo_box_text_new();
	wlan_entry_ssid = gtk_entry_new();
	wlan_entry_psk = gtk_entry_new();
	wlan_check_hidden = gtk_check_button_new_with_label("Hidden SSID");
	
	gtk_header_bar_set_title(GTK_HEADER_BAR(headerbar), "QR-Code Generator");
	gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(headerbar), FALSE);
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
	
	gtk_label_set_markup(GTK_LABEL(text_label), "<span size=\"xx-large\">Generate from text or URL</span>");
	gtk_widget_set_halign(text_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Text or URL");
	gtk_container_set_border_width(GTK_CONTAINER(text_vertical), 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(text_button_generate), "suggested-action");
	
	gtk_label_set_markup(GTK_LABEL(contact_label), "<span size=\"xx-large\">Generate from contact</span>");
	gtk_widget_set_halign(contact_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_first_name), "First name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_last_name), "Last name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_title), "Title");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_street), "Street");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_postal_code), "Postal code");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_city), "City");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_country), "Country");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_company), "Company/organization");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_email_personal), "Email (personal)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_email_business), "Email (business)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_phone_personal), "Phone (personal)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_phone_mobile), "Phone (mobile)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_phone_business), "Phone (business)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(contact_entry_website), "Website");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_first_name), "First name");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_last_name), "Last name");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_title), "Title");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_street), "Street");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_postal_code), "Postal code");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_city), "City");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_country), "Country");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_company), "Company/organization");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_email_personal), "Email (personal)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_email_business), "Email (business)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_phone_personal), "Phone (personal)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_phone_mobile), "Phone (mobile)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_phone_business), "Phone (business)");
	gtk_entry_set_text(GTK_ENTRY(contact_entry_website), "Website");
	gtk_container_set_border_width(GTK_CONTAINER(contact_vertical), 15);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(contact_text_view), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(contact_text_view), 5);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(contact_text_view), 2);
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(contact_text_view), 2);
	// gtk_text_view_set_monospace(GTK_TEXT_VIEW(contact_text_view), TRUE);
	GtkFileFilter *filter_contact = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter_contact, "*.vcf");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(contact_button_file), filter_contact);
	gtk_style_context_add_class(gtk_widget_get_style_context(contact_button_generate), "suggested-action");
	cb_changed_contact_entry(NULL, NULL); // initialize vcard
	
	gtk_label_set_markup(GTK_LABEL(sms_label), "<span size=\"xx-large\">Generate from SMS</span>");
	gtk_widget_set_halign(sms_label, GTK_ALIGN_START);
	gtk_widget_set_halign(sms_label_size, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(sms_entry_phone), "Recipient phone number");
	gtk_entry_set_placeholder_text(GTK_ENTRY(sms_entry_text), "SMS Message");
	gtk_container_set_border_width(GTK_CONTAINER(sms_vertical), 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(sms_button_generate), "suggested-action");
	
	gtk_label_set_markup(GTK_LABEL(call_label), "<span size=\"xx-large\">Generate from call/phone number</span>");
	gtk_widget_set_halign(call_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(call_entry), "Phone number");
	gtk_container_set_border_width(GTK_CONTAINER(call_vertical), 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(call_button_generate), "suggested-action");
	
	gtk_label_set_markup(GTK_LABEL(geo_label), "<span size=\"xx-large\">Generate from geolocation</span>");
	gtk_widget_set_halign(geo_label, GTK_ALIGN_START);
	gtk_widget_set_halign(geo_label_latitude, GTK_ALIGN_START);
	gtk_widget_set_halign(geo_label_longitude, GTK_ALIGN_START);
	gtk_spin_button_set_digits(GTK_SPIN_BUTTON(geo_entry_latitude), 5);
	gtk_spin_button_set_digits(GTK_SPIN_BUTTON(geo_entry_longitude), 5);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(geo_entry_latitude), 0.01, 1);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(geo_entry_longitude), 0.01, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(geo_entry_latitude), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(geo_entry_longitude), 0);
	gtk_container_set_border_width(GTK_CONTAINER(geo_vertical), 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(geo_button_generate), "suggested-action");
	
	gtk_label_set_markup(GTK_LABEL(cal_label), "<span size=\"xx-large\">Generate from calendar event</span>");
	gtk_widget_set_halign(cal_label, GTK_ALIGN_START);
	gtk_widget_set_halign(cal_label_start, GTK_ALIGN_START);
	gtk_widget_set_halign(cal_label_end, GTK_ALIGN_START);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_hour), 23, "23", "23");
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_start_hour), 12);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 23, "23", "23");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 24, "24", "24");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 25, "25", "25");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 26, "26", "26");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 27, "27", "27");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 28, "28", "28");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 29, "29", "29");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 30, "30", "30");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 31, "31", "31");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 32, "32", "32");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 33, "33", "33");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 34, "34", "34");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 35, "35", "35");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 36, "36", "36");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 37, "37", "37");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 38, "38", "38");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 39, "39", "39");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 40, "40", "40");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 41, "41", "41");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 42, "42", "42");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 43, "43", "43");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 44, "44", "44");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 45, "45", "45");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 46, "46", "46");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 47, "47", "47");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 48, "48", "48");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 49, "49", "49");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 50, "50", "50");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 51, "51", "51");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 52, "52", "52");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 53, "53", "53");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 54, "54", "54");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 55, "55", "55");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 56, "56", "56");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 57, "57", "57");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 58, "58", "58");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_start_minute), 59, "59", "59");
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_start_minute), 0);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_hour), 23, "23", "23");
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_end_hour), 12);
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 0, "0", "00");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 1, "1", "01");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 2, "2", "02");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 3, "3", "03");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 4, "4", "04");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 5, "5", "05");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 6, "6", "06");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 7, "7", "07");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 8, "8", "08");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 9, "9", "09");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 10, "10", "10");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 11, "11", "11");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 12, "12", "12");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 13, "13", "13");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 14, "14", "14");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 15, "15", "15");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 16, "16", "16");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 17, "17", "17");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 18, "18", "18");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 19, "19", "19");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 20, "20", "20");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 21, "21", "21");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 22, "22", "22");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 23, "23", "23");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 24, "24", "24");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 25, "25", "25");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 26, "26", "26");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 27, "27", "27");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 28, "28", "28");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 29, "29", "29");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 30, "30", "30");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 31, "31", "31");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 32, "32", "32");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 33, "33", "33");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 34, "34", "34");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 35, "35", "35");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 36, "36", "36");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 37, "37", "37");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 38, "38", "38");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 39, "39", "39");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 40, "40", "40");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 41, "41", "41");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 42, "42", "42");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 43, "43", "43");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 44, "44", "44");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 45, "45", "45");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 46, "46", "46");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 47, "47", "47");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 48, "48", "48");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 49, "49", "49");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 50, "50", "50");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 51, "51", "51");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 52, "52", "52");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 53, "53", "53");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 54, "54", "54");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 55, "55", "55");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 56, "56", "56");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 57, "57", "57");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 58, "58", "58");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(cal_time_end_minute), 59, "59", "59");
	gtk_combo_box_set_active(GTK_COMBO_BOX(cal_time_end_minute), 0);
	gtk_entry_set_placeholder_text(GTK_ENTRY(cal_entry_des), "Event description");
	gtk_entry_set_text(GTK_ENTRY(cal_entry_des), "Event description");
	gtk_container_set_border_width(GTK_CONTAINER(cal_vertical), 15);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(cal_text_view), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(cal_text_view), 5);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(cal_text_view), 2);
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(cal_text_view), 2);
	// gtk_text_view_set_monospace(GTK_TEXT_VIEW(cal_text_view), TRUE);
	GtkFileFilter *filter_cal = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter_cal, "*.ics");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(cal_button_file), filter_cal);
	gtk_style_context_add_class(gtk_widget_get_style_context(cal_button_generate), "suggested-action");
	cb_changed_cal_entry(NULL, NULL); // initialize icalendar
	
	gtk_label_set_markup(GTK_LABEL(mail_label), "<span size=\"xx-large\">Generate from e-mail</span>");
	gtk_widget_set_halign(mail_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(mail_entry_to), "Recipient e-mail address");
	gtk_entry_set_placeholder_text(GTK_ENTRY(mail_entry_subject), "E-mail subject");
	gtk_container_set_border_width(GTK_CONTAINER(mail_vertical), 15);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(mail_text_view), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(mail_text_view), 5);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(mail_text_view), 2);
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(mail_text_view), 2);
	gtk_text_buffer_set_text(mail_text_buffer, "E-mail message", -1);
	gtk_style_context_add_class(gtk_widget_get_style_context(mail_button_generate), "suggested-action");
	
	gtk_label_set_markup(GTK_LABEL(wlan_label), "<span size=\"xx-large\">Generate from WLAN</span>");
	gtk_widget_set_halign(wlan_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(wlan_entry_ssid), "WLAN SSID");
	gtk_entry_set_placeholder_text(GTK_ENTRY(wlan_entry_psk), "WLAN password or pre-shared key");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(wlan_combo_auth), 0, "nopass", "No password");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(wlan_combo_auth), 1, "WEP", "WEP authentication");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(wlan_combo_auth), 2, "WPA", "WPA authentication");
	gtk_combo_box_set_active(GTK_COMBO_BOX(wlan_combo_auth), 0);
	gtk_widget_set_sensitive(wlan_entry_psk, FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(wlan_vertical), 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(wlan_button_generate), "suggested-action");
	
	gtk_box_pack_start(GTK_BOX(text_horizontal_buttons), text_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_horizontal_buttons), text_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vertical), text_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vertical), text_entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vertical), text_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(text_scrolled), text_vertical);
	
	gtk_box_pack_start(GTK_BOX(contact_horizontal_0), contact_entry_first_name, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_1), contact_entry_last_name, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_2), contact_entry_title, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_3), contact_entry_street, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_4), contact_entry_postal_code, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_5), contact_entry_city, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_6), contact_entry_country, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_0), contact_entry_company, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_1), contact_entry_email_personal, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_2), contact_entry_email_business, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_3), contact_entry_phone_personal, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_4), contact_entry_phone_mobile, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_5), contact_entry_phone_business, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_6), contact_entry_website, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_buttons), contact_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_horizontal_buttons), contact_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_button_file, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_0, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_3, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_4, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_5, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_6, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_text_view, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(contact_vertical), contact_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(contact_scrolled), contact_vertical);
	
	gtk_box_pack_start(GTK_BOX(sms_vertical), sms_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(sms_vertical), sms_entry_phone, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(sms_vertical), sms_entry_text, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(sms_horizontal_buttons), sms_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(sms_horizontal_buttons), sms_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(sms_horizontal_buttons), sms_label_size, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(sms_vertical), sms_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(sms_scrolled), sms_vertical);
	
	gtk_box_pack_start(GTK_BOX(call_vertical), call_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(call_vertical), call_entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(call_horizontal_buttons), call_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(call_horizontal_buttons), call_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(call_vertical), call_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(call_scrolled), call_vertical);
	
	gtk_box_pack_start(GTK_BOX(geo_vertical), geo_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_latitude), geo_label_latitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_latitude), geo_entry_latitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_latitude), geo_radio_north, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_latitude), geo_radio_south, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_longitude), geo_label_longitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_longitude), geo_entry_longitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_longitude), geo_radio_west, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical_coords_longitude), geo_radio_east, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_horizontal_coords), geo_vertical_coords_latitude, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(geo_horizontal_coords), geo_vertical_coords_longitude, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical), geo_horizontal_coords, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_horizontal_buttons), geo_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_horizontal_buttons), geo_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(geo_vertical), geo_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(geo_scrolled), geo_vertical);
	
	gtk_box_pack_start(GTK_BOX(cal_vertical), cal_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical), cal_button_file, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical), cal_entry_des, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical_date_pickers_start), cal_label_start, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical_date_pickers_start), cal_date_start, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers_start_time), cal_time_start_hour, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers_start_time), cal_label_start_colon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers_start_time), cal_time_start_minute, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical_date_pickers_start), cal_horizontal_date_pickers_start_time, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical_date_pickers_end), cal_label_end, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical_date_pickers_end), cal_date_end, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers_end_time), cal_time_end_hour, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers_end_time), cal_label_end_colon, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers_end_time), cal_time_end_minute, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical_date_pickers_end), cal_horizontal_date_pickers_end_time, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers), cal_vertical_date_pickers_start, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_date_pickers), cal_vertical_date_pickers_end, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical), cal_horizontal_date_pickers, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical), cal_text_view, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_buttons), cal_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_horizontal_buttons), cal_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(cal_vertical), cal_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(cal_scrolled), cal_vertical);
	
	gtk_box_pack_start(GTK_BOX(mail_vertical), mail_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mail_vertical), mail_entry_to, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mail_vertical), mail_entry_subject, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mail_vertical), mail_text_view, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mail_horizontal_buttons), mail_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mail_horizontal_buttons), mail_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(mail_vertical), mail_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(mail_scrolled), mail_vertical);
	
	gtk_box_pack_start(GTK_BOX(wlan_vertical), wlan_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_vertical), wlan_entry_ssid, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_vertical), wlan_combo_auth, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_vertical), wlan_entry_psk, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_vertical), wlan_check_hidden, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_horizontal_buttons), wlan_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_horizontal_buttons), wlan_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(wlan_vertical), wlan_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(wlan_scrolled), wlan_vertical);
	
	gtk_stack_add_titled(GTK_STACK(stack), text_scrolled, "input_text", "Text/URL");
	gtk_stack_add_titled(GTK_STACK(stack), contact_scrolled, "input_contact", "Contact");
	gtk_stack_add_titled(GTK_STACK(stack), sms_scrolled, "input_sms", "SMS");
	gtk_stack_add_titled(GTK_STACK(stack), call_scrolled, "input_call", "Call/Phone number");
	gtk_stack_add_titled(GTK_STACK(stack), geo_scrolled, "input_geo", "Geolocation");
	gtk_stack_add_titled(GTK_STACK(stack), cal_scrolled, "input_cal", "Calendar event");
	gtk_stack_add_titled(GTK_STACK(stack), mail_scrolled, "input_mail", "E-Mail");
	gtk_stack_add_titled(GTK_STACK(stack), wlan_scrolled, "input_wlan", "WLAN");
	gtk_stack_add_titled(GTK_STACK(stack), drawing, "output_code", "Generated QR code");
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
	gtk_stack_set_visible_child_name(GTK_STACK(stack), "input_text");
	
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_text), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_text, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_text), stack_switcher_sidebar_label_text, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_text, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_contact), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_contact, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_contact), stack_switcher_sidebar_label_contact, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_contact, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_sms), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_sms, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_sms), stack_switcher_sidebar_label_sms, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_sms, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_call), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_call, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_call), stack_switcher_sidebar_label_call, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_call, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_geo), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_geo, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_geo), stack_switcher_sidebar_label_geo, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_geo, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_cal), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_cal, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_cal), stack_switcher_sidebar_label_cal, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_cal, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_mail), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_mail, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_mail), stack_switcher_sidebar_label_mail, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_mail, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_wlan), 10);
	gtk_widget_set_halign(stack_switcher_sidebar_label_wlan, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_wlan), stack_switcher_sidebar_label_wlan, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_wlan, -1);
	gtk_container_set_border_width(GTK_CONTAINER(stack_switcher_sidebar_box_code), 10);
	gtk_label_set_markup(GTK_LABEL(stack_switcher_sidebar_label_code), "<b>Generated QR code</b>");
	gtk_widget_set_halign(stack_switcher_sidebar_label_code, GTK_ALIGN_START);
	gtk_box_pack_start(GTK_BOX(stack_switcher_sidebar_box_code), stack_switcher_sidebar_label_code, TRUE, TRUE, 0);
	gtk_list_box_insert(GTK_LIST_BOX(stack_switcher_sidebar), stack_switcher_sidebar_box_code, -1);
	gtk_box_pack_start(GTK_BOX(root_horizontal), stack_switcher_sidebar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(root_horizontal), stack, TRUE, TRUE, 0);
	
	g_signal_connect(G_OBJECT(drawing), "draw", G_CALLBACK(cb_drawing), NULL);
	g_signal_connect(G_OBJECT(text_button_generate), "clicked", G_CALLBACK(cb_clicked_text_generate), NULL);
	g_signal_connect(G_OBJECT(contact_button_generate), "clicked", G_CALLBACK(cb_clicked_contact_generate), NULL);
	g_signal_connect(G_OBJECT(sms_button_generate), "clicked", G_CALLBACK(cb_clicked_sms_generate), NULL);
	g_signal_connect(G_OBJECT(call_button_generate), "clicked", G_CALLBACK(cb_clicked_call_generate), NULL);
	g_signal_connect(G_OBJECT(geo_button_generate), "clicked", G_CALLBACK(cb_clicked_geo_generate), NULL);
	g_signal_connect(G_OBJECT(cal_button_generate), "clicked", G_CALLBACK(cb_clicked_cal_generate), NULL);
	g_signal_connect(G_OBJECT(mail_button_generate), "clicked", G_CALLBACK(cb_clicked_mail_generate), NULL);
	g_signal_connect(G_OBJECT(wlan_button_generate), "clicked", G_CALLBACK(cb_clicked_wlan_generate), NULL);
	g_signal_connect(G_OBJECT(contact_button_file), "file-set", G_CALLBACK(cb_file_set_contact), NULL);
	g_signal_connect(G_OBJECT(cal_button_file), "file-set", G_CALLBACK(cb_file_set_cal), NULL);
	g_signal_connect(G_OBJECT(text_button_clear), "clicked", G_CALLBACK(cb_clicked_text_clear), NULL);
	g_signal_connect(G_OBJECT(contact_button_clear), "clicked", G_CALLBACK(cb_clicked_contact_clear), NULL);
	g_signal_connect(G_OBJECT(sms_button_clear), "clicked", G_CALLBACK(cb_clicked_sms_clear), NULL);
	g_signal_connect(G_OBJECT(call_button_clear), "clicked", G_CALLBACK(cb_clicked_call_clear), NULL);
	g_signal_connect(G_OBJECT(geo_button_clear), "clicked", G_CALLBACK(cb_clicked_geo_clear), NULL);
	g_signal_connect(G_OBJECT(cal_button_clear), "clicked", G_CALLBACK(cb_clicked_cal_clear), NULL);
	g_signal_connect(G_OBJECT(mail_button_clear), "clicked", G_CALLBACK(cb_clicked_mail_clear), NULL);
	g_signal_connect(G_OBJECT(wlan_button_clear), "clicked", G_CALLBACK(cb_clicked_wlan_clear), NULL);
	g_signal_connect(G_OBJECT(contact_entry_first_name), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_last_name), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_title), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_street), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_postal_code), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_city), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_country), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_company), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_email_personal), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_email_business), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_phone_personal), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_phone_mobile), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_phone_business), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(contact_entry_website), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(cal_entry_des), "changed", G_CALLBACK(cb_changed_cal_entry), NULL);
	g_signal_connect(G_OBJECT(cal_time_start_hour), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(cal_time_start_minute), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(cal_time_end_hour), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(cal_time_end_minute), "changed", G_CALLBACK(cb_changed_cal_time), NULL);
	g_signal_connect(G_OBJECT(cal_date_start), "day-selected", G_CALLBACK(cb_changed_cal_date), NULL);
	g_signal_connect(G_OBJECT(cal_date_end), "day-selected", G_CALLBACK(cb_changed_cal_date), NULL);
	g_signal_connect(G_OBJECT(sms_entry_text), "changed", G_CALLBACK(cb_changed_sms_entry), NULL);
	g_signal_connect(G_OBJECT(wlan_combo_auth), "changed", G_CALLBACK(cb_changed_wlan_combo), NULL);
	g_signal_connect(G_OBJECT(stack_switcher_sidebar), "row-activated", G_CALLBACK(cb_selected_stack_switcher), NULL);
	
	gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
	gtk_container_add(GTK_CONTAINER(window), root_horizontal);
	
	gtk_widget_show_all(window);
}