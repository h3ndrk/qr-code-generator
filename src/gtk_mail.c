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
#include "gtk_mail.h"

static GtkWidget *gtk_mail_entry_to = NULL;
static GtkWidget *gtk_mail_entry_subject = NULL;
static GtkTextBuffer *gtk_mail_text_buffer = NULL;

void cb_clicked_mail_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *mail_data = NULL;
	gchar *mail_data_message = NULL;
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	
	UNUSED(button);
	UNUSED(data);
	
	gtk_text_buffer_get_start_iter(gtk_mail_text_buffer, &start_iter);
	gtk_text_buffer_get_end_iter(gtk_mail_text_buffer, &end_iter);
	mail_data_message = gtk_text_buffer_get_text(gtk_mail_text_buffer, &start_iter, &end_iter, TRUE);
	
	mail_data = g_strdup_printf("MATMSG:TO:%s;SUB:%s;BODY:%s;;", (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_mail_entry_to)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_mail_entry_subject)), mail_data_message);
	
	error = qr_render(mail_data);
	
	g_free(mail_data_message);
	g_free(mail_data);
	
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

static void cb_clicked_mail_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(gtk_mail_entry_to), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_mail_entry_subject), "");
	gtk_text_buffer_set_text(gtk_mail_text_buffer, "E-mail message", -1);
}

GtkWidget *gtk_mail_qr_new(void)
{
	GtkWidget *gtk_mail_label = gtk_label_new(NULL);
	GtkWidget *gtk_mail_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_mail_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_mail_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_mail_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_mail_entry_to = gtk_entry_new();
	gtk_mail_entry_subject = gtk_entry_new();
	GtkWidget *gtk_mail_text_view_scrolled_frame = gtk_frame_new(NULL);
	GtkWidget *gtk_mail_text_view_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_mail_text_view = gtk_text_view_new();
	gtk_mail_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_mail_text_view));
	
	gtk_label_set_markup(GTK_LABEL(gtk_mail_label), "<span size=\"xx-large\">Generate from e-mail</span>");
	gtk_widget_set_halign(gtk_mail_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_mail_entry_to), "Recipient e-mail address");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_mail_entry_subject), "E-mail subject");
	gtk_container_set_border_width(GTK_CONTAINER(gtk_mail_vertical), 15);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(gtk_mail_text_view), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(gtk_mail_text_view), 5);
	gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(gtk_mail_text_view), 2);
	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(gtk_mail_text_view), 2);
	gtk_widget_set_size_request(gtk_mail_text_view_scrolled, -1, 200);
	gtk_text_buffer_set_text(gtk_mail_text_buffer, "E-mail message", -1);
	
	gtk_box_pack_start(GTK_BOX(gtk_mail_vertical), gtk_mail_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_mail_vertical), gtk_mail_entry_to, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_mail_vertical), gtk_mail_entry_subject, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_mail_text_view_scrolled), gtk_mail_text_view);
	gtk_container_add(GTK_CONTAINER(gtk_mail_text_view_scrolled_frame), gtk_mail_text_view_scrolled);
	gtk_box_pack_start(GTK_BOX(gtk_mail_vertical), gtk_mail_text_view_scrolled_frame, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(gtk_mail_horizontal_buttons), gtk_mail_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_mail_vertical), gtk_mail_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_mail_scrolled), gtk_mail_vertical);
	
	g_signal_connect(G_OBJECT(gtk_mail_button_clear), "clicked", G_CALLBACK(cb_clicked_mail_clear), NULL);
	
	return gtk_mail_scrolled;
}