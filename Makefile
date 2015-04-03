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

LIBS += `pkg-config --libs gtk+-3.0`
LIBS += `pkg-config --libs libqrencode`

.PHONY: all qr-code-generator init clean

all: qr-code-generator

# Compile main program

qr-code-generator: init bin/obj/qr-code-generator.o bin/obj/gtk_window.o bin/obj/qr_generator.o
	$(CC) bin/obj/qr-code-generator.o bin/obj/gtk_window.o bin/obj/qr_generator.o $(CFLAGS) -o bin/qr-code-generator $(LIBS)

# Initializes directories

init:
	mkdir -p bin
	mkdir -p bin/obj

# C-objects

bin/obj/qr-code-generator.o: src/qr-code-generator.c
	$(CC) $(CFLAGS) -c -o bin/obj/qr-code-generator.o src/qr-code-generator.c $(LIBS)

bin/obj/gtk_window.o: src/gtk_window.c
	$(CC) $(CFLAGS) -c -o bin/obj/gtk_window.o src/gtk_window.c $(LIBS)

bin/obj/qr_generator.o: src/qr_generator.c
	$(CC) $(CFLAGS) -c -o bin/obj/qr_generator.o src/qr_generator.c $(LIBS)

# Clean

clean:
	rm -Rf bin