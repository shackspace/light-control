can-or-control
==============

Der Code ist ein Fork von can-light-control welches wiederum ein fork von ethernet-light-control war.--
Unnützer Code ist über die Compilerflags auskommentiert.  


Auf der Platine ist ein Bootloader und die Applikation geflasht.  
Man kann auch ohne Bootloader updaten, dann aber bitte keinen "erase" machen sondern nur die applikation überschreiben, dann bleibt der Bootloader weiter drauf.  
Im makefile wird bei "make programm" richtig geflasht. (avrdude Option -D)

Steuerung der Zeiten:  
https://github.com/shackspace/light-control/blob/b45d291892bda02c7a972620c65fcc054511f11a/can-or-control/shackbus.c#L216  
Beispiel Küche:  
power_mgt_set_wait_off(3, 90 * 60); //Sekunden
