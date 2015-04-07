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
#include "gtk_text.h"
#include "gtk_contact.h"
#include "gtk_sms.h"
#include "gtk_call.h"
#include "gtk_geo.h"
#include "gtk_cal.h"
#include "gtk_mail.h"
#include "gtk_wlan.h"
#include "qr_generator.h"

GtkWidget *window = NULL;
static GtkWidget *headerbar = NULL;
static GtkWidget *root_stack = NULL;
static GtkWidget *stack = NULL;
static GtkWidget *drawing = NULL;
static GtkWidget *stack_switcher_sidebar = NULL;
static GtkWidget *button_generate = NULL;
static GtkWidget *button_export = NULL;

void gtk_set_output(void)
{
	gtk_widget_queue_draw(drawing);
	gtk_stack_set_visible_child_name(GTK_STACK(root_stack), "output");
	gtk_button_set_label(GTK_BUTTON(button_generate), "Back");
	gtk_widget_show(button_export);
}

void gtk_set_input(void)
{
	gtk_stack_set_visible_child_name(GTK_STACK(root_stack), "input");
	gtk_button_set_label(GTK_BUTTON(button_generate), "Generate");
	gtk_widget_hide(button_export);
}

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

static void gtk_render_to_path_png(guint size, gchar *path)
{
	GArray *qr_code_data = NULL;
	gint qr_code_size = 0;
	guint x = 0;
	guint y = 0;
	guint index = 0;
	cairo_surface_t *surface = NULL;
	cairo_t *cr = NULL;
	
	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, size, size);
	cr = cairo_create(surface);
	
	// white background
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr, 0, 0, size, size);
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
					cairo_rectangle(cr, floor((x + 1) * ((float)size / (qr_code_size + 2))), floor((y + 1) * ((float)size / (qr_code_size + 2))), ceil((float)size / (qr_code_size + 2)), ceil((float)size / (qr_code_size + 2)));
					cairo_fill(cr);
				}
			}
		}
	}
	
	cairo_surface_write_to_png(surface, path);
	
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
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
		"input_wlan"
	};
	
	UNUSED(box);
	UNUSED(data);
	
	gtk_stack_set_visible_child_name(GTK_STACK(stack), stack_names[gtk_list_box_row_get_index(row)]);
}

static void cb_clicked_generate(GtkWidget *button, gpointer data)
{
	UNUSED(button);
	UNUSED(data);
	
	if(strcmp(gtk_stack_get_visible_child_name(GTK_STACK(root_stack)), "input") == 0)
	{
		switch(gtk_list_box_row_get_index(gtk_list_box_get_selected_row(GTK_LIST_BOX(stack_switcher_sidebar))))
		{
			case 0: { cb_clicked_text_generate(NULL, NULL); break; }
			case 1: { cb_clicked_contact_generate(NULL, NULL); break; }
			case 2: { cb_clicked_sms_generate(NULL, NULL); break; }
			case 3: { cb_clicked_call_generate(NULL, NULL); break; }
			case 4: { cb_clicked_geo_generate(NULL, NULL); break; }
			case 5: { cb_clicked_cal_generate(NULL, NULL); break; }
			case 6: { cb_clicked_mail_generate(NULL, NULL); break; }
			case 7: { cb_clicked_wlan_generate(NULL, NULL); break; }
		}
	}
	else
	{
		gtk_set_input();
	}
}

// static void cb_toggled_cal_dst(GtkToggleButton *togglebutton, gpointer data)
// {
// 	UNUSED(togglebutton);
// 	UNUSED(data);
	
// 	cb_changed_cal_entry(NULL, NULL);
// }

static void cb_clicked_export(GtkWidget *button, gpointer data)
{
	gint result = 0;
	gchar *filename = NULL;
	GtkWidget *export_box = NULL;
	GtkWidget *export_size_label = NULL;
	GtkWidget *export_size = NULL;
	GtkWidget *file_chooser_dialog = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	export_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	export_size_label = gtk_label_new("QR code size (in pixel):");
	export_size = gtk_spin_button_new_with_range(0, 2048, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(export_size), 512);
	gtk_box_pack_start(GTK_BOX(export_box), export_size_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(export_box), export_size, FALSE, FALSE, 0);
	gtk_widget_show_all(export_box);
	
	file_chooser_dialog = gtk_file_chooser_dialog_new("Save file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(file_chooser_dialog), export_box);
	result = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog));
	
	if(result == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog));
		
		gtk_render_to_path_png(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(export_size)), filename);
		
		g_free(filename);
	}
	
	gtk_widget_destroy(file_chooser_dialog);
}

void gtk_window_init(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 720, 570);
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
	stack_switcher_sidebar = gtk_list_box_new();
	GtkWidget *stack_switcher_sidebar_scrolled = gtk_scrolled_window_new(NULL, NULL);
	button_generate = gtk_button_new_with_label("Generate");
	button_export = gtk_button_new_with_label("Export");
	GtkWidget *root_horizontal_pane = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	headerbar = gtk_header_bar_new();
	stack = gtk_stack_new();
	root_stack = gtk_stack_new();
	drawing = gtk_drawing_area_new();
	
	GtkWidget *gtk_text = gtk_text_qr_new();
	GtkWidget *gtk_contact = gtk_contact_qr_new();
	GtkWidget *gtk_sms = gtk_sms_qr_new();
	GtkWidget *gtk_call = gtk_call_qr_new();
	GtkWidget *gtk_geo = gtk_geo_qr_new();
	GtkWidget *gtk_cal = gtk_cal_qr_new();
	GtkWidget *gtk_mail = gtk_mail_qr_new();
	GtkWidget *gtk_wlan = gtk_wlan_qr_new();
	
	gtk_header_bar_set_title(GTK_HEADER_BAR(headerbar), "QR-Code Generator");
	gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(headerbar), FALSE);
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
	gtk_style_context_add_class(gtk_widget_get_style_context(button_generate), "suggested-action");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button_generate);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button_export);
	
	gtk_stack_add_titled(GTK_STACK(stack), gtk_text, "input_text", "Text/URL");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_contact, "input_contact", "Contact");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_sms, "input_sms", "SMS");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_call, "input_call", "Call/Phone number");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_geo, "input_geo", "Geolocation");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_cal, "input_cal", "Calendar event");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_mail, "input_mail", "E-Mail");
	gtk_stack_add_titled(GTK_STACK(stack), gtk_wlan, "input_wlan", "WLAN");
	gtk_stack_add_titled(GTK_STACK(root_stack), root_horizontal_pane, "input", "Input");
	gtk_stack_add_titled(GTK_STACK(root_stack), drawing, "output", "Output");
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
	gtk_stack_set_visible_child_name(GTK_STACK(stack), "input_text");
	gtk_stack_set_transition_type(GTK_STACK(root_stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
	gtk_stack_set_visible_child_name(GTK_STACK(root_stack), "input");
	
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
	gtk_paned_set_position(GTK_PANED(root_horizontal_pane), 175);
	gtk_container_add(GTK_CONTAINER(stack_switcher_sidebar_scrolled), stack_switcher_sidebar);
	gtk_paned_pack1(GTK_PANED(root_horizontal_pane), stack_switcher_sidebar_scrolled, FALSE, FALSE);
	gtk_paned_pack2(GTK_PANED(root_horizontal_pane), stack, TRUE, FALSE);
	
	g_signal_connect(G_OBJECT(drawing), "draw", G_CALLBACK(cb_drawing), NULL);
	g_signal_connect(G_OBJECT(button_generate), "clicked", G_CALLBACK(cb_clicked_generate), NULL);
	g_signal_connect(G_OBJECT(button_export), "clicked", G_CALLBACK(cb_clicked_export), NULL);
	g_signal_connect(G_OBJECT(stack_switcher_sidebar), "row-activated", G_CALLBACK(cb_selected_stack_switcher), NULL);
	
	gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
	gtk_container_add(GTK_CONTAINER(window), root_stack);
	
	gtk_widget_show_all(window);
	gtk_widget_hide(button_export);
}