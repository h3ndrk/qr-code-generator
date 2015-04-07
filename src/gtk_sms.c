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
#include "gtk_sms.h"

static GtkWidget *gtk_sms_entry_phone = NULL;
static GtkWidget *gtk_sms_entry_text = NULL;
static GtkWidget *gtk_sms_label_size = NULL;

void cb_clicked_sms_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *sms_data = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	sms_data = g_strdup_printf("SMSTO:%s:%s", (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_sms_entry_phone)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_sms_entry_text)));
	
	error = qr_render(sms_data);
	
	g_free(sms_data);
	
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

static void cb_clicked_sms_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(gtk_sms_entry_phone), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_sms_entry_text), "");
}

static void cb_changed_sms_entry(GtkEditable *editable, gpointer data)
{
	gchar *character_data = NULL;
	
	UNUSED(editable);
	UNUSED(data);
	
	character_data = g_strdup_printf("%li character%s", strlen((gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_sms_entry_text))), (strlen((gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_sms_entry_text)))!=1)?("s"):(""));
	
	gtk_label_set_text(GTK_LABEL(gtk_sms_label_size), character_data);
	
	g_free(character_data);
}

GtkWidget *gtk_sms_qr_new(void)
{
	GtkWidget *gtk_sms_label = gtk_label_new(NULL);
	GtkWidget *gtk_sms_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_sms_scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_sms_entry_phone = gtk_entry_new();
	gtk_sms_entry_text = gtk_entry_new();
	gtk_sms_label_size = gtk_label_new("0 characters");
	GtkWidget *gtk_sms_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_sms_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	
	gtk_label_set_markup(GTK_LABEL(gtk_sms_label), "<span size=\"xx-large\">Generate from SMS</span>");
	gtk_widget_set_halign(gtk_sms_label, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_sms_label_size, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_sms_entry_phone), "Recipient phone number");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_sms_entry_text), "SMS Message");
	gtk_container_set_border_width(GTK_CONTAINER(gtk_sms_vertical), 15);
	
	gtk_box_pack_start(GTK_BOX(gtk_sms_vertical), gtk_sms_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_sms_vertical), gtk_sms_entry_phone, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_sms_vertical), gtk_sms_entry_text, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_sms_horizontal_buttons), gtk_sms_label_size, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(gtk_sms_horizontal_buttons), gtk_sms_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_sms_vertical), gtk_sms_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_sms_scrolled), gtk_sms_vertical);
	
	g_signal_connect(G_OBJECT(gtk_sms_button_clear), "clicked", G_CALLBACK(cb_clicked_sms_clear), NULL);
	
	g_signal_connect(G_OBJECT(gtk_sms_entry_text), "changed", G_CALLBACK(cb_changed_sms_entry), NULL);
	
	return gtk_sms_scrolled;
}