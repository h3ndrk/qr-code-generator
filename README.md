# QR-Code generator

Simple graphical user interface for generating QR-Codes.

## Requirements

* GTK (3.16, to expand compatibility remove `gtk_text_view_set_monospace()` from `gtk_cal.c` and `gtk_contact.c`)
* [libqrencode](https://github.com/fukuchi/libqrencode)

## Features

* Simple and clean UI
* QR code generation for many input sources (listed below)
* vCard and iCalendar file import
* Export QR code as PNG file with changeable size
* Editable generated contact and calendar QR code data

## Supported input sources

* Text
* URL
* Contact (name, address, company, e-mail, phone number, website)
* SMS (recipient, message)
* Call/Phone number
* Geolocation (latitude, longitude)
* Calendar event (date, time, description)
* E-Mail (recipient, message)
* WLAN access information (SSID, passphrase, encryption, etc.)

## Screenshots

### Text/URL

![Text/URL](screenshots/text.png)

### Contact

![Contact](screenshots/contact.png)

### SMS

![SMS](screenshots/sms.png)

### Call/Phone number

![Call/Phone number](screenshots/call.png)

### Geolocation

![Geolocation](screenshots/geo.png)

### Calendar

![Calendar](screenshots/cal.png)

### E-Mail

![E-Mail](screenshots/mail.png)

### WLAN

![WLAN](screenshots/wlan.png)

### Generated QR-Code

![Generated QR-Code](screenshots/code.png)

### Export QR-Code

![Export QR-Code](screenshots/export.png)

## License

lastest GPL-license (see LICENSE-file)

Copyright (C) 2015 NIPE-SYSTEMS, [http://www.nipe-systems.de](http://www.nipe-systems.de)
