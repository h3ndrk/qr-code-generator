# Copyright (C) 2015 NIPE-SYSTEMS
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc

CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += `pkg-config --cflags gtk+-3.0`
CFLAGS += `pkg-config --cflags libqrencode`

LIBS += -lm
LIBS += `pkg-config --libs gtk+-3.0`
LIBS += `pkg-config --libs libqrencode`

.PHONY: all qr_code_generator init clean

all: qr_code_generator

# Compile main program

qr_code_generator: init bin/obj/qr_code_generator.o bin/obj/gtk_window.o bin/obj/qr_generator.o bin/obj/vcard_generator.o bin/obj/icalendar_generator.o bin/obj/gtk_text.o bin/obj/gtk_contact.o bin/obj/gtk_sms.o bin/obj/gtk_call.o bin/obj/gtk_geo.o bin/obj/gtk_cal.o bin/obj/gtk_mail.o bin/obj/gtk_wlan.o
	$(CC) bin/obj/qr_code_generator.o bin/obj/gtk_window.o bin/obj/qr_generator.o bin/obj/vcard_generator.o bin/obj/icalendar_generator.o bin/obj/gtk_text.o bin/obj/gtk_contact.o bin/obj/gtk_sms.o bin/obj/gtk_call.o bin/obj/gtk_geo.o bin/obj/gtk_cal.o bin/obj/gtk_mail.o bin/obj/gtk_wlan.o $(CFLAGS) -o bin/qr-code-generator $(LIBS)

# Initializes directories

init:
	mkdir -p bin
	mkdir -p bin/obj

# C-objects

bin/obj/qr_code_generator.o: src/qr_code_generator.c
	$(CC) $(CFLAGS) -c -o bin/obj/qr_code_generator.o src/qr_code_generator.c $(LIBS)

bin/obj/gtk_window.o: src/gtk_window.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_window.o src/gtk_window.c $(LIBS)

bin/obj/gtk_text.o: src/gtk_text.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_text.o src/gtk_text.c $(LIBS)

bin/obj/gtk_contact.o: src/gtk_contact.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_contact.o src/gtk_contact.c $(LIBS)

bin/obj/gtk_sms.o: src/gtk_sms.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_sms.o src/gtk_sms.c $(LIBS)

bin/obj/gtk_call.o: src/gtk_call.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_call.o src/gtk_call.c $(LIBS)

bin/obj/gtk_geo.o: src/gtk_geo.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_geo.o src/gtk_geo.c $(LIBS)

bin/obj/gtk_cal.o: src/gtk_cal.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_cal.o src/gtk_cal.c $(LIBS)

bin/obj/gtk_mail.o: src/gtk_mail.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_mail.o src/gtk_mail.c $(LIBS)

bin/obj/gtk_wlan.o: src/gtk_wlan.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_wlan.o src/gtk_wlan.c $(LIBS)

bin/obj/qr_generator.o: src/qr_generator.c
	$(CC) $(CFLAGS) -c -o bin/obj/qr_generator.o src/qr_generator.c $(LIBS)

bin/obj/vcard_generator.o: src/vcard_generator.c
	$(CC) $(CFLAGS) -c -o bin/obj/vcard_generator.o src/vcard_generator.c $(LIBS)

bin/obj/icalendar_generator.o: src/icalendar_generator.c
	$(CC) $(CFLAGS) -c -o bin/obj/icalendar_generator.o src/icalendar_generator.c $(LIBS)

# Clean

clean:
	rm -Rf bin