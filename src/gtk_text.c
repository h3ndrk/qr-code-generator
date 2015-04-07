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
#include "gtk_text.h"

static GtkWidget *gtk_text_entry = NULL;

void cb_clicked_text_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	
	UNUSED(button);
	UNUSED(data);
	
	error = qr_render((gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_text_entry)));
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
	
	gtk_entry_set_text(GTK_ENTRY(gtk_text_entry), "");
	gtk_widget_grab_focus(gtk_text_entry);
}

GtkWidget *gtk_text_qr_new(void)
{
	GtkWidget *gtk_text_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_text_label = gtk_label_new(NULL);
	gtk_text_entry = gtk_entry_new();
	GtkWidget *gtk_text_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_text_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_text_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	
	gtk_label_set_markup(GTK_LABEL(gtk_text_label), "<span size=\"xx-large\">Generate from text or URL</span>");
	gtk_widget_set_halign(gtk_text_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_text_entry), "Text or URL");
	gtk_container_set_border_width(GTK_CONTAINER(gtk_text_vertical), 15);
	
	gtk_box_pack_end(GTK_BOX(gtk_text_horizontal_buttons), gtk_text_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_text_vertical), gtk_text_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_text_vertical), gtk_text_entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_text_vertical), gtk_text_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_text_scrolled), gtk_text_vertical);
	
	g_signal_connect(G_OBJECT(gtk_text_button_clear), "clicked", G_CALLBACK(cb_clicked_text_clear), NULL);
	
	return gtk_text_scrolled;
}