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
#include <qrencode.h>

#include "gtk_window.h"
#include "qr_generator.h"
#include "vcard_generator.h"

int main(void)
{
	// printf("GTK version: %i.%i.%i\n", gtk_major_version, gtk_minor_version, gtk_micro_version);
	// printf("GLIB version: %i.%i.%i\n", glib_major_version, glib_minor_version, glib_micro_version);
	
	gtk_init(NULL, NULL);
	
	gtk_window_init();
	
	gtk_main();
	
	qr_free();
	
	// qr_initialize_array(16);
	// qr_set_pixel(1, 1, TRUE);
	// printf("%i\n", qr_get_pixel(1, 1));
	// qr_free_array();
	
	// qr_render("Das ist ein Test! Das ist ein Test! Das ist ein Test! Das ist ein Test! Das ist ein Test! Das ist ein Test!");
	
	
	return 0;
}