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
#include "gtk_wlan.h"

static GtkWidget *gtk_wlan_entry_ssid = NULL;
static GtkWidget *gtk_wlan_entry_psk = NULL;
static GtkWidget *gtk_wlan_check_hidden = NULL;
static GtkWidget *gtk_wlan_combo_auth = NULL;

static void cb_changed_wlan_combo(GtkComboBox *widget, gpointer data)
{
	UNUSED(data);
	
	// gtk_stack_set_visible_child_name(GTK_STACK(stack), gtk_combo_box_get_active_id(widget));
	
	if(gtk_combo_box_get_active(widget) == 0)
	{
		gtk_widget_set_sensitive(gtk_wlan_entry_psk, FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(gtk_wlan_entry_psk, TRUE);
	}
}

void cb_clicked_wlan_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *wlan_data = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	if(gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_wlan_combo_auth)) == 0)
	{
		wlan_data = g_strdup_printf("WIFI:T:%s;S:%s;;H:%s;", (gchar *)gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_wlan_combo_auth)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_wlan_entry_ssid)), (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_wlan_check_hidden)))?("true"):("false"));
	}
	else
	{
		wlan_data = g_strdup_printf("WIFI:T:%s;S:%s;P:%s;H:%s;", (gchar *)gtk_combo_box_get_active_id(GTK_COMBO_BOX(gtk_wlan_combo_auth)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_wlan_entry_ssid)), (gchar *)gtk_entry_get_text(GTK_ENTRY(gtk_wlan_entry_psk)), (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_wlan_check_hidden)))?("true"):("false"));
	}
	
	error = qr_render(wlan_data);
	
	g_free(wlan_data);
	
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

static void cb_clicked_wlan_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_entry_set_text(GTK_ENTRY(gtk_wlan_entry_ssid), "");
	gtk_entry_set_text(GTK_ENTRY(gtk_wlan_entry_psk), "");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_wlan_combo_auth), 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_wlan_check_hidden), FALSE);
}

GtkWidget *gtk_wlan_qr_new(void)
{
	GtkWidget *gtk_wlan_label = gtk_label_new(NULL);
	GtkWidget *gtk_wlan_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_wlan_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_wlan_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_wlan_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_wlan_combo_auth = gtk_combo_box_text_new();
	gtk_wlan_entry_ssid = gtk_entry_new();
	gtk_wlan_entry_psk = gtk_entry_new();
	gtk_wlan_check_hidden = gtk_check_button_new_with_label("Hidden SSID");
	
	gtk_label_set_markup(GTK_LABEL(gtk_wlan_label), "<span size=\"xx-large\">Generate from WLAN</span>");
	gtk_widget_set_halign(gtk_wlan_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_wlan_entry_ssid), "WLAN SSID");
	gtk_entry_set_placeholder_text(GTK_ENTRY(gtk_wlan_entry_psk), "WLAN password or pre-shared key");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_wlan_combo_auth), 0, "nopass", "No password");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_wlan_combo_auth), 1, "WEP", "WEP authentication");
	gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(gtk_wlan_combo_auth), 2, "WPA", "WPA authentication");
	gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_wlan_combo_auth), 0);
	gtk_widget_set_sensitive(gtk_wlan_entry_psk, FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(gtk_wlan_vertical), 15);
	
	gtk_box_pack_start(GTK_BOX(gtk_wlan_vertical), gtk_wlan_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_wlan_vertical), gtk_wlan_entry_ssid, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_wlan_vertical), gtk_wlan_combo_auth, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_wlan_vertical), gtk_wlan_entry_psk, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_wlan_vertical), gtk_wlan_check_hidden, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(gtk_wlan_horizontal_buttons), gtk_wlan_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_wlan_vertical), gtk_wlan_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_wlan_scrolled), gtk_wlan_vertical);
	
	g_signal_connect(G_OBJECT(gtk_wlan_button_clear), "clicked", G_CALLBACK(cb_clicked_wlan_clear), NULL);
	
	g_signal_connect(G_OBJECT(gtk_wlan_combo_auth), "changed", G_CALLBACK(cb_changed_wlan_combo), NULL);
	
	return gtk_wlan_scrolled;
}