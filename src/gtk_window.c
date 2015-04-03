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

static GtkWidget *window = NULL;
static GtkWidget *headerbar = NULL;
static GtkWidget *stack = NULL;
static GtkWidget *drawing = NULL;
static GtkWidget *text_entry = NULL;
static GtkWidget *other_button_contact = NULL;
static GtkWidget *other_entry_contact_first_name = NULL;
static GtkWidget *other_entry_contact_last_name = NULL;
static GtkWidget *other_entry_contact_title = NULL;
static GtkWidget *other_entry_contact_street = NULL;
static GtkWidget *other_entry_contact_postal_code = NULL;
static GtkWidget *other_entry_contact_city = NULL;
static GtkWidget *other_entry_contact_country = NULL;
static GtkWidget *other_entry_contact_company = NULL;
static GtkWidget *other_entry_contact_email_personal = NULL;
static GtkWidget *other_entry_contact_email_business = NULL;
static GtkWidget *other_entry_contact_phone_personal = NULL;
static GtkWidget *other_entry_contact_phone_mobile = NULL;
static GtkWidget *other_entry_contact_phone_business = NULL;
static GtkWidget *other_entry_contact_website = NULL;

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

static void cb_clicked_other_generate_contact(GtkWidget *button, gpointer data)
{
	gint error = 0;
	GString *generated_data = NULL;
	
	UNUSED(button);
	UNUSED(data);
	
	generated_data = vcard_generate((gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_first_name)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_last_name)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_title)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_street)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_postal_code)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_city)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_country)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_company)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_email_personal)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_email_business)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_phone_personal)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_phone_mobile)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_phone_business)), (gchar *)gtk_entry_get_text(GTK_ENTRY(other_entry_contact_website)));
	
	if(generated_data == NULL)
	{
		return;
	}
	
	printf("%s", generated_data->str);
	
	error = qr_render(generated_data->str);
	
	g_string_free(generated_data, TRUE);
	
	switch(error)
	{
		case ERR_NO_ERROR:
		{
			gtk_widget_queue_draw(drawing);
			gtk_stack_set_visible_child_name(GTK_STACK(stack), "output_code");
			
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

static void cb_file_set_other_contact(GtkFileChooserButton *widget, gpointer data)
{
	gchar *filename = NULL;
	
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
		printf("%s\n", filename);
		
		g_free(filename);
	}
}

void gtk_window_init(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 512, 512);
	gtk_window_set_title(GTK_WINDOW(window), "QR-Code Generator");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	headerbar = gtk_header_bar_new();
	stack = gtk_stack_new();
	drawing = gtk_drawing_area_new();
	GtkWidget *switcher = gtk_stack_switcher_new();
	GtkWidget *text_button_generate = gtk_button_new_with_label("Generate QR code");
	GtkWidget *text_button_clear = gtk_button_new_with_label("Clear");
	GtkWidget *text_label = gtk_label_new(NULL);
	text_entry = gtk_entry_new();
	GtkWidget *text_scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *text_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *text_horizontal_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	GtkWidget *other_label_contact = gtk_label_new(NULL);
	GtkWidget *other_vertical = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	GtkWidget *other_scrolled = gtk_scrolled_window_new(NULL, NULL);
	other_button_contact = gtk_file_chooser_button_new("Select a .vcf (vCard) other", GTK_FILE_CHOOSER_ACTION_OPEN);
	GtkWidget *other_button_generate_contact = gtk_button_new_with_label("Generate QR code");
	GtkWidget *other_button_clear_contact = gtk_button_new_with_label("Clear");
	GtkWidget *other_horizontal_buttons_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	other_entry_contact_first_name = gtk_entry_new();
	other_entry_contact_last_name = gtk_entry_new();
	other_entry_contact_title = gtk_entry_new();
	other_entry_contact_street = gtk_entry_new();
	other_entry_contact_postal_code = gtk_entry_new();
	other_entry_contact_city = gtk_entry_new();
	other_entry_contact_country = gtk_entry_new();
	other_entry_contact_company = gtk_entry_new();
	other_entry_contact_email_personal = gtk_entry_new();
	other_entry_contact_email_business = gtk_entry_new();
	other_entry_contact_phone_personal = gtk_entry_new();
	other_entry_contact_phone_mobile = gtk_entry_new();
	other_entry_contact_phone_business = gtk_entry_new();
	other_entry_contact_website = gtk_entry_new();
	GtkWidget *other_horizontal_0_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *other_horizontal_1_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *other_horizontal_2_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *other_horizontal_3_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *other_horizontal_4_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *other_horizontal_5_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *other_horizontal_6_contact = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	
	gtk_header_bar_set_custom_title(GTK_HEADER_BAR(headerbar), switcher);
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));
	
	gtk_label_set_markup(GTK_LABEL(text_label), "<span size=\"xx-large\">Generate from text or URL</span>");
	gtk_widget_set_halign(text_label, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Text or URL");
	gtk_container_set_border_width(GTK_CONTAINER(text_vertical), 15);
	gtk_style_context_add_class(gtk_widget_get_style_context(text_button_generate), "suggested-action");
	gtk_style_context_add_class(gtk_widget_get_style_context(other_button_generate_contact), "suggested-action");
	
	gtk_label_set_markup(GTK_LABEL(other_label_contact), "<span size=\"xx-large\">Generate from contact</span>");
	gtk_widget_set_sensitive(other_button_contact, FALSE);
	gtk_widget_set_halign(other_label_contact, GTK_ALIGN_START);
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_first_name), "First name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_last_name), "Last name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_title), "Title");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_street), "Street");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_postal_code), "Postal code");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_city), "City");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_country), "Country");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_company), "Company/organization");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_email_personal), "Email (personal)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_email_business), "Email (business)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_phone_personal), "Phone (personal)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_phone_mobile), "Phone (mobile)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_phone_business), "Phone (business)");
	gtk_entry_set_placeholder_text(GTK_ENTRY(other_entry_contact_website), "Website");
	gtk_container_set_border_width(GTK_CONTAINER(other_vertical), 15);
	GtkFileFilter *filter_contact = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter_contact, "*.vcf");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(other_button_contact), filter_contact);
	
	gtk_box_pack_start(GTK_BOX(text_horizontal_buttons), text_button_generate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_horizontal_buttons), text_button_clear, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vertical), text_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vertical), text_entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(text_vertical), text_horizontal_buttons, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(text_scrolled), text_vertical);
	
	gtk_box_pack_start(GTK_BOX(other_horizontal_0_contact), other_entry_contact_first_name, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_1_contact), other_entry_contact_last_name, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_2_contact), other_entry_contact_title, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_3_contact), other_entry_contact_street, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_4_contact), other_entry_contact_postal_code, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_5_contact), other_entry_contact_city, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_6_contact), other_entry_contact_country, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_0_contact), other_entry_contact_company, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_1_contact), other_entry_contact_email_personal, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_2_contact), other_entry_contact_email_business, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_3_contact), other_entry_contact_phone_personal, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_4_contact), other_entry_contact_phone_mobile, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_5_contact), other_entry_contact_phone_business, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_6_contact), other_entry_contact_website, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_buttons_contact), other_button_generate_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_horizontal_buttons_contact), other_button_clear_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_label_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_button_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_0_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_1_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_2_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_3_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_4_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_5_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_6_contact, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(other_vertical), other_horizontal_buttons_contact, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(other_scrolled), other_vertical);
	
	gtk_stack_add_titled(GTK_STACK(stack), text_scrolled, "input_text", "Text/URL");
	gtk_stack_add_titled(GTK_STACK(stack), other_scrolled, "input_other", "Other");
	gtk_stack_add_titled(GTK_STACK(stack), drawing, "output_code", "Generated QR code");
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
	
	g_signal_connect(G_OBJECT(drawing), "draw", G_CALLBACK(cb_drawing), NULL);
	g_signal_connect(G_OBJECT(text_button_generate), "clicked", G_CALLBACK(cb_clicked_text_generate), NULL);
	g_signal_connect(G_OBJECT(other_button_generate_contact), "clicked", G_CALLBACK(cb_clicked_other_generate_contact), NULL);
	g_signal_connect(G_OBJECT(other_button_contact), "file-set", G_CALLBACK(cb_file_set_other_contact), NULL);
	g_signal_connect(G_OBJECT(text_button_clear), "clicked", G_CALLBACK(cb_clicked_text_clear), NULL);
	
	gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
	gtk_container_add(GTK_CONTAINER(window), stack);
	
	gtk_widget_show_all(window);
	
	// GtkWidget *root_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	// GtkWidget *vertical_box_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	// GtkWidget *vertical_box_left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	// GtkWidget *text_view = gtk_text_view_new();
	// drawing = gtk_drawing_area_new();
	// GtkWidget *button = gtk_button_new_with_label("Encode");
	// buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	
	// gtk_container_set_border_width(GTK_CONTAINER(vertical_box_left), 15);
	// gtk_container_set_border_width(GTK_CONTAINER(vertical_box_right), 15);
	// gtk_widget_set_size_request(drawing, 256, 256);
	// g_signal_connect(G_OBJECT(drawing), "draw", G_CALLBACK(cb_drawing), NULL);
	// g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(cb_clicked), NULL);
	
	// root_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	// right_vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	// scrolled_left = gtk_scrolled_window_new(NULL, NULL);
	// gtk_container_set_border_width(GTK_CONTAINER(right_vertical_box), 15);
	// scrolled_right = gtk_scrolled_window_new(NULL, NULL);
	// button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	// GtkWidget *button = gtk_button_new_with_label("Refresh");
	// GtkWidget *button2 = gtk_button_new_with_label("Fullscreen");
	// drawing = gtk_drawing_area_new();
	// gtk_widget_set_size_request(drawing, 256, 256);
	// g_signal_connect(G_OBJECT(drawing), "draw", G_CALLBACK(cb_drawing), NULL);
	// gtk_box_pack_start(GTK_BOX(root_box), scrolled_left, TRUE, TRUE, 0);
	// gtk_box_pack_start(GTK_BOX(root_box), scrolled_right, TRUE, TRUE, 0);
	// gtk_box_pack_start(GTK_BOX(right_vertical_box), drawing, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(right_vertical_box), button_box, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(button_box), button2, FALSE, FALSE, 0);
	// gtk_container_add(GTK_CONTAINER(scrolled_right), right_vertical_box);
	
	// // GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	// // GtkWidget *radio = gtk_radio_button_new_with_label(NULL, "Das ist ein Test!!!");
	// // GtkWidget *radio2 = gtk_radio_button_new_with_label(NULL, "Das ist ein Test!!!");
	// // gtk_box_pack_start(GTK_BOX(box), radio, FALSE, FALSE, 0);
	// // gtk_box_pack_start(GTK_BOX(box), radio2, FALSE, FALSE, 0);
	// // gtk_container_add(GTK_CONTAINER(scrolled_left), root_box);
	
	// gtk_box_pack_start(GTK_BOX(vertical_box_left), text_view, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(vertical_box_left), button, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(vertical_box_right), drawing, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(root_box), vertical_box_left, TRUE, TRUE, 0);
	// gtk_box_pack_start(GTK_BOX(root_box), vertical_box_right, TRUE, TRUE, 0);
	
	// gtk_container_add(GTK_CONTAINER(window), root_box);
}