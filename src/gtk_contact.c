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
#include "gtk_contact.h"
#include "vcard_generator.h"

static GtkTextBuffer *gtk_contact_text_buffer = NULL;
static GtkWidget *gtk_contact_button_file = NULL;
static GtkWidget *gtk_contact_entry_first_name = NULL;
static GtkWidget *gtk_contact_entry_last_name = NULL;
static GtkWidget *gtk_contact_entry_title = NULL;
static GtkWidget *gtk_contact_entry_street = NULL;
static GtkWidget *gtk_contact_entry_postal_code = NULL;
static GtkWidget *gtk_contact_entry_city = NULL;
static GtkWidget *gtk_contact_entry_country = NULL;
static GtkWidget *gtk_contact_entry_company = NULL;
static GtkWidget *gtk_contact_entry_email_personal = NULL;
static GtkWidget *gtk_contact_entry_email_business = NULL;
static GtkWidget *gtk_contact_entry_phone_personal = NULL;
static GtkWidget *gtk_contact_entry_phone_mobile = NULL;
static GtkWidget *gtk_contact_entry_phone_business = NULL;
static GtkWidget *gtk_contact_entry_website = NULL;

void cb_clicked_contact_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *vcard_data = NULL;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	
	UNUSED(button);
	UNUSED(data);
	
	gtk_text_buffer_get_start_iter(gtk_contact_text_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(gtk_contact_text_buffer, &end_iter);
	vcard_data = gtk_text_buffer_get_text(gtk_contact_text_buffer, &start_iter, &end_iter, TRUE);
	
	error = qr_render(vcard_data);
	
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
		
		gtk_text_buffer_set_text(gtk_contact_text_buffer, file_contents, -1);
		
		g_free(file_contents);
		g_free(filename);
	}
}

static void cb_changed_contact_entry(GtkEditable *editable, gpointer data)
{
	GString *generated_data = NULL;
	
	UNUSED(editable);
	UNUSED(data);
	
	generated_data = vcard_generate((gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_first_name)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_last_name)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_title)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_street)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_postal_code)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_city)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_country)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_company)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_email_personal)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_email_business)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_phone_personal)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_phone_mobile)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_phone_business)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_contact_entry_website)));
	
	if(generated_data == NULL)
	{
		return;
	}
	
	gtk_text_buffer_set_text(gtk_contact_text_buffer, generated_data->str, -1);
	
	g_string_free(generated_data, TRUE);
}

static void cb_clicked_contact_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_first_name), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_last_name), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_title), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_street), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_postal_code), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_city), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_country), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_company), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_email_personal), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_email_business), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_phone_personal), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_phone_mobile), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_phone_business), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_website), "");
}

GtkWidget *gtk_contact_qr_new(void)
{
	GtkWidget *gtk_contact_label = gtk_label_new(NULL);
	GtkWidget *gtk_contact_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_contact_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_contact_description_label = gtk_label_new("Either vCard file import or manual information entering are supported. Modify the generated vCard at the bottom.");
	GtkWidget *gtk_contact_button_file_label = gtk_label_new("vCard (.vcf) file");
	GtkWidget *gtk_contact_button_file_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	gtk_contact_button_file = gtk_file_chooser_button_new("Select a .vcf (vCard) contact", GTK_FILE_CHOOSER_ACTION_OPEN);
	GtkWidget *gtk_contact_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_contact_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_contact_entry_first_name = gtk_entry_new();
	gtk_contact_entry_last_name = gtk_entry_new();
	gtk_contact_entry_title = gtk_entry_new();
	gtk_contact_entry_street = gtk_entry_new();
	gtk_contact_entry_postal_code = gtk_entry_new();
	gtk_contact_entry_city = gtk_entry_new();
	gtk_contact_entry_country = gtk_entry_new();
	gtk_contact_entry_company = gtk_entry_new();
	gtk_contact_entry_email_personal = gtk_entry_new();
	gtk_contact_entry_email_business = gtk_entry_new();
	gtk_contact_entry_phone_personal = gtk_entry_new();
	gtk_contact_entry_phone_mobile = gtk_entry_new();
	gtk_contact_entry_phone_business = gtk_entry_new();
	gtk_contact_entry_website = gtk_entry_new();
	GtkWidget *gtk_contact_horizontal_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_horizontal_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_horizontal_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_horizontal_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_horizontal_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_horizontal_5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_horizontal_6 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *gtk_contact_text_view_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_contact_text_view = gtk_text_view_new();
	gtk_contact_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_contact_text_view));
	
	gtk_label_set_markup(GTK_LABEL(gtk_contact_label), "<span size=\"xx-large\">Generate from contact</span>");
	gtk_widget_set_halign(gtk_contact_label, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_contact_description_label, GTK_ALIGN_START);
	gtk_label_set_line_wrap(GTK_LABEL(gtk_contact_description_label), TRUE);
	gtk_label_set_justify(GTK_LABEL(gtk_contact_description_label), GTK_JUSTIFY_FILL);
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_first_name), "First name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_last_name), "Last name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_title), "Title");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_street), "Street");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_postal_code), "Postal code");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_city), "City");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_country), "Country");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_company), "Company/organization");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_email_personal), "Email (personal)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_email_business), "Email (business)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_phone_personal), "Phone (personal)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_phone_mobile), "Phone (mobile)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_phone_business), "Phone (business)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_contact_entry_website), "Website");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_first_name), "First name");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_last_name), "Last name");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_title), "Title");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_street), "Street");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_postal_code), "Postal code");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_city), "City");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_country), "Country");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_company), "Company/organization");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_email_personal), "Email (personal)");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_email_business), "Email (business)");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_phone_personal), "Phone (personal)");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_phone_mobile), "Phone (mobile)");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_phone_business), "Phone (business)");
	// gtk_entry_set_text(GTK_ENTRY(gtk_contact_entry_website), "Website");
	gtk_container_set_border_width(GTK_CONTAINER(gtk_contact_vertical), 15);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(gtk_contact_text_view), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(gtk_contact_text_view), 5);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(gtk_contact_text_view), 2);
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(gtk_contact_text_view), 2);
	// gtk_text_view_set_monospace(GTK_TEXT_VIEW(gtk_contact_text_view), TRUE);
	GtkFileFilter *filter_contact = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter_contact, "*.vcf");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(gtk_contact_button_file), filter_contact);
	cb_changed_contact_entry(NULL, NULL); // initialize vcard
	
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_0), gtk_contact_entry_first_name, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_1), gtk_contact_entry_last_name, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_2), gtk_contact_entry_title, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_3), gtk_contact_entry_street, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_4), gtk_contact_entry_postal_code, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_5), gtk_contact_entry_city, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_6), gtk_contact_entry_country, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_0), gtk_contact_entry_company, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_1), gtk_contact_entry_email_personal, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_2), gtk_contact_entry_email_business, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_3), gtk_contact_entry_phone_personal, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_4), gtk_contact_entry_phone_mobile, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_5), gtk_contact_entry_phone_business, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_horizontal_6), gtk_contact_entry_website, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(gtk_contact_horizontal_buttons), gtk_contact_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_description_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_button_file_box), gtk_contact_button_file_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_button_file_box), gtk_contact_button_file, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_button_file_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_0, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_3, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_4, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_5, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_6, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_contact_text_view_scrolled), gtk_contact_text_view);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_text_view_scrolled, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_contact_vertical), gtk_contact_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_contact_scrolled), gtk_contact_vertical);
	
	g_signal_connect(G_OBJECT(gtk_contact_button_file), "file-set", G_CALLBACK(cb_file_set_contact), NULL);
	
	g_signal_connect(G_OBJECT(gtk_contact_button_clear), "clicked", G_CALLBACK(cb_clicked_contact_clear), NULL);
	
	g_signal_connect(G_OBJECT(gtk_contact_entry_first_name), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_last_name), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_title), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_street), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_postal_code), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_city), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_country), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_company), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_email_personal), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_email_business), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_phone_personal), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_phone_mobile), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_phone_business), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	g_signal_connect(G_OBJECT(gtk_contact_entry_website), "changed", G_CALLBACK(cb_changed_contact_entry), NULL);
	
	return gtk_contact_scrolled;
}