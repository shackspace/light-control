Lichtsteuerung + Stromsteuerung
===============================

can-light-control
=================
Dieses Device verbindet:
- Mensch LED+Taster HMI
- Eltako RS485
- CAN-Bus
- Hauptschalter
miteinander.

ethernet-light-control:
=======================
Verbindet Ethernet <=> CAN-Bus und handelt die entsprechenden Protokolle.

can-or-control:
===============
- Hier wird der CAN-Bus mit dem Eltako RS485 von den Optionsräumen verbunden
- Das Hauptschaltersignal wird über den CAN-Bus übermittelt.
- Die Steuerung hat noch einen Eingang(Stromschleife für Optokoppler) vom Münzzähler und kann dann das Warmwasser von der Dusche einschalten
- Es werden 4 Ausgangskanäle über RS485 angesteuert. Alle mit minimalen Zeitverzögerungen ein und aus damit man keine Sicherungen wirft.
  - DLE Dusche
  - DLE Küche
  - Küche (Zeitverzögert ausgeschaltet wegen Spülmaschine)
  - Optionsräume

node-light:
===========
Hier liegt code welcher eine Rest Api anbietet.
Dieser Code wurde hier weitergepflegt:
https://git.shackspace.de/rz/node-light


util:
=====
eltako.py: Mit diesem Script haben wir, ohne damals bestehende Eltako Doku, die erfolgreichen Versuche gestartet mit Hilfe der enocean Funk Protokolle den RS485 Bus erfolgreich zu bespielen.
