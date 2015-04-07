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
#include "gtk_geo.h"

static GtkWidget *gtk_geo_entry_latitude = NULL;
static GtkWidget *gtk_geo_entry_longitude = NULL;
static GtkWidget *gtk_geo_radio_north = NULL;
static GtkWidget *gtk_geo_radio_south = NULL;
static GtkWidget *gtk_geo_radio_west = NULL;
static GtkWidget *gtk_geo_radio_east = NULL;

void cb_clicked_geo_generate(GtkWidget *button, gpointer data)
{
	gint error = 0;
	gchar *geo_data = NULL;
	gdouble latitude = 0;
	gdouble longitude = 0;
	
	UNUSED(button);
	UNUSED(data);
	
	latitude = gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_geo_entry_latitude));
	longitude = gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_geo_entry_longitude));
	
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_geo_radio_north)))
	{
		latitude *= -1;
	}
	
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_geo_radio_west)))
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

static void cb_clicked_geo_clear(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_geo_entry_latitude), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_geo_entry_longitude), 0);
}

GtkWidget *gtk_geo_qr_new(void)
{
	GtkWidget *gtk_geo_label = gtk_label_new(NULL);
	GtkWidget *gtk_geo_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_geo_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *gtk_geo_label_latitude = gtk_label_new("Latitude");
	GtkWidget *gtk_geo_label_longitude = gtk_label_new("Longitude");
	gtk_geo_entry_latitude = gtk_spin_button_new_with_range(-90, 90, 0.01);
	gtk_geo_entry_longitude = gtk_spin_button_new_with_range(-180, 180, 0.01);
	gtk_geo_radio_north = gtk_radio_button_new_with_label(NULL, "North");
	gtk_geo_radio_south = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(gtk_geo_radio_north), "South");
	gtk_geo_radio_west = gtk_radio_button_new_with_label(NULL, "West");
	gtk_geo_radio_east = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(gtk_geo_radio_west), "East");
	GtkWidget *gtk_geo_button_clear = gtk_button_new_with_label("Clear and reset");
	GtkWidget *gtk_geo_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *gtk_geo_horizontal_coords = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
	GtkWidget *gtk_geo_vertical_coords_latitude = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *gtk_geo_vertical_coords_longitude = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	
	gtk_label_set_markup(GTK_LABEL(gtk_geo_label), "<span size=\"xx-large\">Generate from geolocation</span>");
	gtk_widget_set_halign(gtk_geo_label, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_geo_label_latitude, GTK_ALIGN_START);
	gtk_widget_set_halign(gtk_geo_label_longitude, GTK_ALIGN_START);
	gtk_spin_button_set_digits(GTK_SPIN_BUTTON(gtk_geo_entry_latitude), 5);
	gtk_spin_button_set_digits(GTK_SPIN_BUTTON(gtk_geo_entry_longitude), 5);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(gtk_geo_entry_latitude), 0.01, 1);
	gtk_spin_button_set_increments(GTK_SPIN_BUTTON(gtk_geo_entry_longitude), 0.01, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_geo_entry_latitude), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_geo_entry_longitude), 0);
	gtk_container_set_border_width(GTK_CONTAINER(gtk_geo_vertical), 15);
	
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical), gtk_geo_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_latitude), gtk_geo_label_latitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_latitude), gtk_geo_entry_latitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_latitude), gtk_geo_radio_north, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_latitude), gtk_geo_radio_south, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_longitude), gtk_geo_label_longitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_longitude), gtk_geo_entry_longitude, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_longitude), gtk_geo_radio_west, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical_coords_longitude), gtk_geo_radio_east, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_horizontal_coords), gtk_geo_vertical_coords_latitude, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_horizontal_coords), gtk_geo_vertical_coords_longitude, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical), gtk_geo_horizontal_coords, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(gtk_geo_horizontal_buttons), gtk_geo_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gtk_geo_vertical), gtk_geo_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtk_geo_scrolled), gtk_geo_vertical);
	
	g_signal_connect(G_OBJECT(gtk_geo_button_clear), "clicked", G_CALLBACK(cb_clicked_geo_clear), NULL);
	
	return gtk_geo_scrolled;
}