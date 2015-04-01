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
#include <gtk/gtk.h>

static GtkWidget *window = NULL;
static GtkWidget *root_box = NULL;
static GtkWidget *right_vertical_box = NULL;
static GtkWidget *scrolled_left = NULL;
static GtkWidget *scrolled_right = NULL;
static GtkWidget *drawing = NULL;
static GtkWidget *button_box = NULL;

gboolean cb_drawing(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	guint width, height;
  GdkRGBA color;

  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);
  cairo_arc (cr,
             width / 2.0, height / 2.0,
             MIN (width, height) / 2.0,
             0, 2 * G_PI);

  gtk_style_context_get_color (gtk_widget_get_style_context (widget),
                               0,
                               &color);
  gdk_cairo_set_source_rgba (cr, &color);

  cairo_fill (cr);
	
	return FALSE;
}

void gtk_window_init(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 870, 520);
	gtk_window_set_title(GTK_WINDOW(window), "QR-Code Generator");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	root_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	right_vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
	scrolled_left = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_set_border_width(GTK_CONTAINER(right_vertical_box), 15);
	scrolled_right = gtk_scrolled_window_new(NULL, NULL);
	button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget *button = gtk_button_new_with_label("Refresh");
	GtkWidget *button2 = gtk_button_new_with_label("Fullscreen");
	GtkWidget *label = gtk_label_new("Lorem ipsum dolor sit amet, consetetur\nsadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et\ndolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam\net justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea t\nkimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit \nmet, consetetur sadipscing elitr, sed diam nonumy eirm\nd tempor invidunt ut labore et dolore magna aliquyam erat, sed diam v\nluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\nclita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor s\nt amet.Lorem ipsum dolor sit amet, consetetur sadipscing\nelitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore mag\na aliquyam erat, sed diam voluptua. At vero eos et accusam et justo d\no dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanc\nus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, conset\ntur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labor\n et dolore magna aliquyam erat, sed diam voluptua. At vero eos et acc\n sam et justo duo dolores et ea rebum. Stet clita kasd gube\ngren, no sea takimata sanctus est Lorem ipsum dolor sit amet.Lorem ip\num dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmo\n tempor invidunt ut labore et dolore magna aliquyam erat, sed diam vo\n uptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\n clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor \n it amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, se\n  diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquy\n  m erat, sed diam voluptua. At vero eos et accusam et justo duo dolor\n  s et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus es\n   Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet, consetetur \n   adipscing elitr, sed diam nonumy eirmod tempor invidunt ut labor\n   Lorem ipsum dolor sit amet, consetetur\nsadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et\ndolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam\net justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea t\nkimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit \nmet, consetetur sadipscing elitr, sed diam nonumy eirm\nd tempor invidunt ut labore et dolore magna aliquyam erat, sed diam v\nluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\nclita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor s\nt amet.Lorem ipsum dolor sit amet, consetetur sadipscing\nelitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore mag\na aliquyam erat, sed diam voluptua. At vero eos et accusam et justo d\no dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanc\nus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, conset\ntur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labor\n et dolore magna aliquyam erat, sed diam voluptua. At vero eos et acc\n sam et justo duo dolores et ea rebum. Stet clita kasd gube\ngren, no sea takimata sanctus est Lorem ipsum dolor sit amet.Lorem ip\num dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmo\n tempor invidunt ut labore et dolore magna aliquyam erat, sed diam vo\n uptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\n clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor \n it amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, se\n  diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquy\n  m erat, sed diam voluptua. At vero eos et accusam et justo duo dolor\n  s et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus es\n   Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet, consetetur \n   adipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore \n   Lorem ipsum dolor sit amet, consetetur\nsadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et\ndolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam\net justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea t\nkimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit \nmet, consetetur sadipscing elitr, sed diam nonumy eirm\nd tempor invidunt ut labore et dolore magna aliquyam erat, sed diam v\nluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\nclita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor s\nt amet.Lorem ipsum dolor sit amet, consetetur sadipscing\nelitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore mag\na aliquyam erat, sed diam voluptua. At vero eos et accusam et justo d\no dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanc\nus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, conset\ntur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labor\n et dolore magna aliquyam erat, sed diam voluptua. At vero eos et acc\n sam et justo duo dolores et ea rebum. Stet clita kasd gube\ngren, no sea takimata sanctus est Lorem ipsum dolor sit amet.Lorem ip\num dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmo\n tempor invidunt ut labore et dolore magna aliquyam erat, sed diam vo\n uptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\n clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor \n it amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, se\n  diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquy\n  m erat, sed diam voluptua. At vero eos et accusam et justo duo dolor\n  s et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus es\n   Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet, consetetur \n   adipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore \n   Lorem ipsum dolor sit amet, consetetur\nsadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et\ndolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam\net justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea t\nkimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit \nmet, consetetur sadipscing elitr, sed diam nonumy eirm\nd tempor invidunt ut labore et dolore magna aliquyam erat, sed diam v\nluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\nclita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor s\nt amet.Lorem ipsum dolor sit amet, consetetur sadipscing\nelitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore mag\na aliquyam erat, sed diam voluptua. At vero eos et accusam et justo d\no dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanc\nus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, conset\ntur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labor\n et dolore magna aliquyam erat, sed diam voluptua. At vero eos et acc\n sam et justo duo dolores et ea rebum. Stet clita kasd gube\ngren, no sea takimata sanctus est Lorem ipsum dolor sit amet.Lorem ip\num dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmo\n tempor invidunt ut labore et dolore magna aliquyam erat, sed diam vo\n uptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\n clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor \n it amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, se\n  diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquy\n  m erat, sed diam voluptua. At vero eos et accusam et justo duo dolor\n  s et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus es\n   Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet, consetetur \n   adipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore \n   Lorem ipsum dolor sit amet, consetetur\nsadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et\ndolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam\net justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea t\nkimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit \nmet, consetetur sadipscing elitr, sed diam nonumy eirm\nd tempor invidunt ut labore et dolore magna aliquyam erat, sed diam v\nluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\nclita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor s\nt amet.Lorem ipsum dolor sit amet, consetetur sadipscing\nelitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore mag\na aliquyam erat, sed diam voluptua. At vero eos et accusam et justo d\no dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanc\nus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, conset\ntur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labor\n et dolore magna aliquyam erat, sed diam voluptua. At vero eos et acc\n sam et justo duo dolores et ea rebum. Stet clita kasd gube\ngren, no sea takimata sanctus est Lorem ipsum dolor sit amet.Lorem ip\num dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmo\n tempor invidunt ut labore et dolore magna aliquyam erat, sed diam vo\n uptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet\n clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor \n it amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, se\n  diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquy\n  m erat, sed diam voluptua. At vero eos et accusam et justo duo dolor\n  s et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus es\n   Lorem ipsum dolor sit amet.Lorem ipsum dolor sit amet, consetetur \n   adipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore ");
	gtk_container_set_border_width(GTK_CONTAINER(label), 15);
	drawing = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing, 256, 256);
	g_signal_connect(G_OBJECT(drawing), "draw", G_CALLBACK(cb_drawing), NULL);
	gtk_box_pack_start(GTK_BOX(root_box), scrolled_left, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(root_box), scrolled_right, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(right_vertical_box), drawing, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(right_vertical_box), button_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(button_box), button2, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(scrolled_right), right_vertical_box);
	gtk_container_add(GTK_CONTAINER(scrolled_left), label);
	
	// GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	// GtkWidget *radio = gtk_radio_button_new_with_label(NULL, "Das ist ein Test!!!");
	// GtkWidget *radio2 = gtk_radio_button_new_with_label(NULL, "Das ist ein Test!!!");
	// gtk_box_pack_start(GTK_BOX(box), radio, FALSE, FALSE, 0);
	// gtk_box_pack_start(GTK_BOX(box), radio2, FALSE, FALSE, 0);
	// gtk_container_add(GTK_CONTAINER(scrolled_left), root_box);
	
	gtk_container_add(GTK_CONTAINER(window), root_box);
	
	gtk_widget_show_all(window);
}