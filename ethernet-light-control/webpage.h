/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich, Thomas
 Remarks:        
 known Problems: none
 Version:        30.03.2009
 Description:    Html Seiten

 Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation ver�ffentlicht, 
 weitergeben und/oder modifizieren, entweder gem�� Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder sp�teren Version. 

 Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, 
 da� es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/
#ifndef _WEBPAGE_H
	#define _WEBPAGE_H

//****************************************************************************
//Dateien und Webseiten am ende dieser Seite in Tabelle eintragen !!!!!!!
//****************************************************************************


//----------------------------------------------------------------------------
//Dazustellende Webseite

PROGMEM char Page1[] = {
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\r\n"
	"<html><head>\r\n"
	"<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">\r\n"
	"<title>AVR mini Webserver : Home</title>\r\n"
	
	"<link rel=\"stylesheet\" type=\"text/css\"  href=\"styles.css\">"

	"</head><body>\r\n"
	"<h1>AVR mini Webserver : Home</h1>\r\n"
	"<div id=\"nav\"><a href=\"index.htm\">Home</a><a href=\"status.htm\">Status</a></div>\r\n"
	"<div id=\"main\"><h2>Gl�ckwunsch!</h2><br>\r\n"

	//Kamerabild anzeigen oder Rakete
	#if USE_CAM
	   "<img src=\"camera.jpg\" width=\"640\" height=\"480\" alt=\"\">"
	#else
	   "<img src=\"raket.gif\" width=\"90\" height=\"92\" alt=\"\">"
	#endif //USE_CAM	
	
	"<br><br>\r\n"
	
	"<p>Du hast es geschafft: Der Webserver l�uft!</p>"
	"<br><hr><br>\r\n"
	
	"<div style=\"width:200px;margin: 0 auto;\">\r\n"
	
	"<div style=\"float:left;width:100px\">\r\n"
	"<form method=\"post\" action=\"index.htm\">\r\n"
	"<input name=\"OUT\" type=\"checkbox\" value=\"A\" %PORTA0>"
	"Port A Pin 0<br>\r\n" 
	"<input name=\"OUT\" type=\"checkbox\" value=\"B\" %PORTA1>"
	"Port A Pin 1<br>\r\n"
	"<input name=\"OUT\" type=\"checkbox\" value=\"C\" %PORTA2>"
	"Port A Pin 2<br>"
	 //Rechner �ber WOL einschalten
    #if USE_WOL
		"<input type=\"checkbox\" name=\"OUT\" value=\"W\">\r\n"
		"Wake On Lan<br>"
	#endif //USE_WOL
	"<br>\r\n"
	"<input name=\"SUB\" value=\"Senden\" type=\"submit\">\r\n"
	"</form></div>\r\n"


	"<div style=\"float:left; width:100px\">\r\n"
	//Eing�nge Abfragen %PINxy x = PORT / y = PIN
	"<img src=\"%PIND7\" alt=\"\"> Port D Pin 7<br>"
	"<img src=\"%PIND6\" alt=\"\"> Port D Pin 6<br>"
	"<img src=\"%PIND5\" alt=\"\"> Port D Pin 5<br>"
	"</div>"

	"</div>\r\n"
	"<div class=\"clear\"></div>\r\n"

	"<br><hr><br>\r\n"
	
	//Balkendiagramm
	"<table border=\"0\" class=\"tempTbl\" align=\"center\" width=\"400px;\">\r\n"
	"<tr>\r\n"
	"<th>Eingang</th>\r\n"
	"<th>Balken</th>\r\n"
	"<th>Temperatur [�C]</th>\r\n"
	"</tr><tr>\r\n"
	
	"<td>AD-Kanal0</td>\r\n"
	"<td style=\"text-align:left\"><img src=\"index-Dateien/balke.gif\" alt=\"\" height=\"10\" width=\"%VA@00\"></td>\r\n"
	"<td>%VA@00</td>\r\n"
	"</tr><tr>\r\n"
	
	"<td>AD-Kanal1</td>\r\n"
	"<td style=\"text-align:left\"><img src=\"index-Dateien/balke.gif\" alt=\"\" height=\"10\" width=\"%VA@01\"></td>\r\n"
	"<td>%VA@01</td>\r\n"
	"</tr><tr>\r\n"
	
	"<td>AD-Kanal2</td>\r\n"
	"<td style=\"text-align:left\"><img src=\"index-Dateien/balke.gif\" alt=\"\" height=\"10\" width=\"%VA@02\"></td>\r\n"
	"<td>%VA@02</td>\r\n"
	"</tr><tr>\r\n"
	
	"<td>AD-Kanal3</td>\r\n"
	"<td style=\"text-align:left\"><img src=\"index-Dateien/balke.gif\" alt=\"\" height=\"10\" width=\"%VA@03\"></td>\r\n"
	"<td>%VA@03</td>\r\n"
	
	"</tr>\r\n"
	"</table>\r\n"
	"</div><div id=\"tail\"><a href=\"http://www.ulrichradig.de\">www.ulrichradig.de</a> Ulrich Radig 2008</div>\r\n"
	"</body></html>"
	"%END"};

//----------------------------------------------------------------------------
//Status Seite
PROGMEM char Page2[] = {
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\r\n"
	"<html><head>\r\n"
	"<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">\r\n"
	"<title>AVR mini Webserver : Status</title>\r\n"
	
	"<link rel=\"stylesheet\" type=\"text/css\"  href=\"styles.css\">"
	
	"</head><body>"
	"<h1>AVR mini Webserver : Status</h1>\r\n"
	"<div id=\"nav\"><a href=\"index.htm\">Home</a><a href=\"status.htm\">Status</a></div>\r\n"
	"<div id=\"main\">\r\n"
	
	"Compiliert am "__DATE__" um " __TIME__"<br><br>\r\n"
	"Compiliert mit GCC Version "__VERSION__"<br><br>\r\n"
	"Besucher seit Start des Webservers (%VA@09)<br><br>\r\n"

	"</div><div id=\"tail\"><a href=\"http://www.ulrichradig.de\">www.ulrichradig.de</a> Ulrich Radig 2008</div>\r\n"
	"</body></html>\r\n"
	"%END"};
//----------------------------------------------------------------------------
// CSS Styles
PROGMEM char Page3[] = {
	"body{margin: 40px;} "
	"body,p,div,h1,h2,td,th{font-family:sans-serif;font-size:11px}"
	"h1{font-size:20px;color:white; background-color:#96B11C;margin:1px;padding:5px;text-align:center;}"
	"h2{font-size:18px;color:#96B11C}"
	"p,h2{margin:0px0px5px0px}"
	"img{border:0}"
	"hr{color:silver; background-color:silver;border:0;height:1px}"
	"#nav{font-size: 16px; background-color:silver;margin:1px;padding:5px;font-weight:bold}"
	"#nav a, #tail a{color:white; padding:10px;text-decoration:none}"
	"#nav a:hover,#tail a:hover{text-decoration:underline}"
	"#tail{ color:white; background-color:silver;text-align:center;padding:5px}"
	"#main{text-align:center;margin:20px}"
	".clear{clear:left}"
	".tempTbl {table-layout:auto;border-spacing:0}"
	".tempTbl td{text-align:center;padding:7px}"
	".tempTbl th{background-color:silver}"
	"%END"};

	
//----------------------------------------------------------------------------
//Das GIF Bild f�r LED on
PROGMEM char led_on[] = 
	{
	0x47,0x49,0x46,0x38,0x39,0x61,0x11,0x00,0x11,0x00,0xA2,0x07,0x00,0x62,0x62,0x62,
	0x19,0xAD,0x21,0x21,0xE2,0x2C,0xA5,0xA5,0xA5,0x64,0xE6,0x6B,0xA5,0xF3,0xA9,0xD5,
	0xD5,0xD5,0xFF,0xFF,0xFF,0x21,0xF9,0x04,0x01,0x00,0x00,0x07,0x00,0x2C,0x00,0x00,
	0x00,0x00,0x11,0x00,0x11,0x00,0x00,0x03,0x60,0x78,0xAA,0xD6,0xF3,0x66,0xC9,0xD3,
	0xEA,0xBB,0x51,0x56,0x12,0x7A,0x17,0x00,0x30,0x2C,0x4D,0xF1,0x09,0xA8,0xD0,0x85,
	0x59,0x13,0x08,0x44,0x4C,0xA4,0x6A,0x48,0x19,0x2A,0x51,0xEC,0x73,0xBA,0x0E,0x2E,
	0xD8,0xAE,0x10,0xA3,0x05,0x44,0x41,0x1D,0x8F,0x56,0x13,0x0D,0x5E,0xB2,0xA2,0x31,
	0xF4,0x78,0xA1,0x7A,0xCC,0xA3,0xD3,0xCA,0x9C,0x02,0x0C,0x4F,0x6E,0xF7,0x77,0xA8,
	0x76,0x7D,0x36,0x4A,0xE8,0xE4,0xD3,0x8E,0x14,0x83,0xB5,0xC7,0x43,0x9D,0x18,0x42,
	0xF8,0xFC,0x7B,0x52,0x8E,0xE3,0xF7,0x0A,0x09,0x00,0x3B,
	'%','E','N','D'};
	
//----------------------------------------------------------------------------
//Das GIF Bild f�r den LED off
PROGMEM char led_off[] =
	{
	0x47,0x49,0x46,0x38,0x39,0x61,0x11,0x00,0x11,0x00,0xA2,0x07,0x00,0x57,0x56,0x57,
	0x69,0x69,0x69,0x7E,0x7E,0x7E,0x90,0x91,0x91,0xB0,0xB0,0xB0,0xC8,0xC8,0xC8,0xE7,
	0xE7,0xE7,0xFF,0xFF,0xFF,0x21,0xF9,0x04,0x01,0x00,0x00,0x07,0x00,0x2C,0x00,0x00,
	0x00,0x00,0x11,0x00,0x11,0x00,0x00,0x03,0x6F,0x78,0xAA,0x56,0x45,0xB0,0xAC,0x79,
	0x4C,0x73,0x70,0x68,0x39,0xAD,0x19,0x01,0x20,0x06,0x42,0x49,0x2C,0x56,0x41,0x6A,
	0x43,0x19,0x90,0x01,0x67,0x05,0x03,0x44,0x68,0xA5,0xF0,0x0A,0x55,0xD1,0x12,0x8E,
	0x47,0xCB,0xA5,0x03,0xAA,0x6A,0x85,0xC6,0x2D,0xA7,0xA3,0x39,0x00,0x48,0xCC,0xB0,
	0x34,0x82,0x40,0x6B,0x99,0xA9,0x40,0x04,0xB0,0x0A,0x58,0x38,0x66,0x15,0xC4,0x2C,
	0xBB,0x00,0x02,0x9F,0xCE,0x9C,0x23,0x75,0x0F,0x2D,0x12,0x5B,0x07,0x08,0x28,0x0A,
	0x2E,0x79,0x3B,0xF4,0x56,0x10,0x76,0x7C,0x75,0x7C,0x27,0x13,0x78,0x2F,0x5C,0x55,
	0x14,0x0B,0x37,0x21,0x24,0x84,0x0B,0x09,0x00,0x3B,
	'%','E','N','D'};
	
//----------------------------------------------------------------------------
//Das GIF Bild f�r den Balken
PROGMEM char bild_balken[] = 
	{
	0x47,0x49,0x46,0x38,0x39,0x61,0x02,0x00,0x0A,0x00,0xA2,0x00,0x00,0xA2,0xA5,0xED,
	0x84,0x8F,0xE5,0x66,0x7C,0xDD,0x49,0x60,0xD6,0x21,0x44,0xDA,0xFE,0x01,0x02,0x00,
	0x00,0x00,0x00,0x00,0x00,0x21,0xF9,0x04,0x05,0x14,0x00,0x05,0x00,0x2C,0x00,0x00,
	0x00,0x00,0x02,0x00,0x0A,0x00,0x00,0x03,0x0A,0x48,0x34,0x23,0x12,0x01,0x48,0x00,
	0x1D,0x53,0x09,0x00,0x3B,'%','E','N','D'};

//----------------------------------------------------------------------------
//Das GIF Bild f�r die Rakete
PROGMEM char bild_rakete[] = 
	{
	0x47,0x49,0x46,0x38,0x39,0x61,0x5A,0x00,0x5C,0x00,0xA2,0x04,0x00,0x79,0x79,0xD8,
	0xF6,0xF4,0xE1,0xC9,0xC9,0xE0,0x9C,0x98,0xD8,0xFF,0xFF,0xFF,0xF4,0xF6,0xB0,0xF9,
	0xFD,0x7C,0xA6,0xA4,0x8E,0x21,0xF9,0x04,0x01,0x00,0x00,0x04,0x00,0x2C,0x00,0x00,
	0x00,0x00,0x5A,0x00,0x5C,0x00,0x00,0x03,0xFF,0x48,0xBA,0xDC,0xFE,0x2E,0x2C,0x49,
	0x82,0xA5,0xB3,0x5A,0x21,0xB4,0x96,0x18,0x06,0x8D,0x64,0x69,0x7A,0x8D,0x85,0xA6,
	0x5C,0xA5,0xA8,0x8F,0x78,0xCE,0xB4,0x29,0xBF,0x99,0x30,0x0C,0x54,0x1F,0xD7,0xC0,
	0xE0,0x0B,0x36,0xC4,0x65,0x08,0x03,0xC0,0xC0,0x18,0xE8,0x4C,0x6E,0xC2,0x28,0xE9,
	0xC2,0x2C,0x10,0x61,0x80,0x2C,0x2E,0xB0,0x93,0x7A,0x69,0xA0,0x4B,0x58,0x61,0x4D,
	0x55,0xB2,0xCB,0x85,0x40,0x09,0x83,0x7E,0xDF,0x9F,0xDE,0x6E,0x27,0x08,0x5F,0xD6,
	0xEC,0x57,0xB2,0xFE,0x72,0xC2,0x83,0x6E,0x16,0x56,0x20,0x49,0x3B,0x59,0x7C,0x61,
	0x78,0x3C,0x15,0x49,0x79,0x4D,0x7C,0x7F,0x80,0x44,0x2F,0x05,0x65,0x5C,0x4A,0x0B,
	0x7B,0x4F,0x8A,0x99,0x68,0x12,0x6B,0x7E,0x91,0x33,0x62,0x2E,0x13,0x95,0x9F,0x8E,
	0x4D,0x59,0x22,0x8A,0x1B,0x9E,0x4D,0x7B,0x6E,0x66,0xA2,0x71,0x22,0x01,0x95,0x05,
	0x67,0x75,0x03,0x3A,0xAF,0x13,0x9A,0x04,0x78,0x1D,0x97,0x69,0xA5,0xB4,0x53,0x62,
	0xB6,0xB8,0x16,0x00,0xBB,0x59,0x9E,0x4F,0x3B,0x20,0x6B,0x3C,0xAA,0xBC,0xC6,0x47,
	0xC7,0x0C,0x17,0x83,0x20,0x15,0xCB,0x01,0xAB,0xC1,0x07,0x07,0x90,0xDF,0x05,0x2D,
	0x15,0x3A,0x20,0x87,0xA3,0xB2,0x52,0x82,0xA7,0x21,0x05,0x06,0xA7,0x85,0xF1,0x76,
	0xB7,0xDD,0x1A,0x75,0xE8,0xE5,0xB6,0x93,0xB0,0x45,0x0A,0x03,0xCE,0x14,0xBD,0x41,
	0xB1,0xE4,0xA9,0x08,0x60,0xA0,0x5E,0x08,0x7C,0x43,0x64,0xF8,0x78,0x57,0x63,0xE2,
	0xB9,0x65,0x94,0xE8,0xD5,0x3B,0xD3,0x0C,0xFF,0x1C,0x88,0x86,0x95,0x9E,0xDC,0xF2,
	0xA1,0xA0,0x0E,0xAC,0x39,0x73,0x9C,0x48,0x8C,0x71,0x65,0x24,0xA1,0x45,0xFA,0x1C,
	0x6A,0xA8,0x04,0x12,0x15,0x00,0x8F,0xB7,0x40,0xE6,0xA2,0x60,0xA5,0x9B,0x0E,0x43,
	0x4A,0x9E,0x3D,0xE3,0x45,0xD1,0x86,0x3E,0x66,0x8E,0x34,0x26,0xD3,0xB8,0xF1,0xD2,
	0x81,0x53,0xDF,0x1A,0xCA,0xBC,0x50,0xAE,0x97,0xD0,0x43,0x1C,0x1A,0x35,0x0B,0x18,
	0x45,0x9F,0x95,0xA1,0xFD,0x6A,0x4E,0x90,0xBA,0x33,0xCB,0x01,0x7D,0x51,0x1D,0x3E,
	0xD2,0x2A,0x94,0x68,0x8F,0x4B,0x5B,0x6D,0x40,0x00,0x78,0x50,0x11,0x9B,0x9C,0x21,
	0x3F,0x8A,0x4D,0x38,0x08,0x97,0xD5,0xAB,0xBC,0x4C,0x4E,0xE8,0x40,0xCD,0xDC,0x08,
	0x2A,0xDB,0x20,0xE0,0x6A,0x64,0x88,0x17,0xCD,0x41,0x15,0xC8,0x5A,0xD8,0x03,0x4E,
	0xC0,0x01,0xB6,0x87,0xDA,0x20,0xA9,0xC3,0x81,0x83,0x12,0x95,0x46,0xBE,0xE8,0x63,
	0x2C,0x54,0x80,0xC6,0x53,0x4C,0xCB,0x91,0x13,0x40,0xD6,0x0A,0x1E,0x34,0x90,0x30,
	0xF0,0xA0,0x16,0xB8,0x5D,0x9C,0x6C,0x53,0x04,0xFE,0x6A,0x86,0xF9,0xA9,0xCE,0xD4,
	0xE5,0x68,0xD6,0x3B,0x35,0xD4,0x1B,0x15,0x09,0x85,0xB0,0x66,0x8E,0xE0,0x42,0x56,
	0xB2,0xE3,0x5B,0x92,0x70,0xE9,0xDD,0xFA,0xF7,0x63,0x72,0x95,0x5E,0xA7,0x21,0xD5,
	0x44,0x95,0xA7,0x7B,0x22,0xB9,0x23,0x73,0xC9,0x1D,0x39,0x00,0x58,0xCD,0xD0,0x30,
	0x05,0x29,0xB5,0x3D,0xF6,0xEC,0xCF,0x86,0x25,0x4E,0xFE,0xB9,0xD1,0x73,0x85,0x2B,
	0xE6,0xC6,0x53,0x78,0x61,0x55,0x60,0x61,0xFF,0xEB,0xB5,0xD7,0x5E,0x01,0xC1,0x55,
	0x72,0x40,0x7C,0x0F,0x55,0x03,0x56,0x2F,0x1B,0xE0,0x82,0x13,0x18,0x5E,0x61,0x24,
	0xCC,0x1C,0x43,0xAD,0x67,0xA1,0x81,0x3D,0x3D,0xA5,0x9D,0x1C,0x48,0xF0,0x32,0x14,
	0x01,0x99,0x71,0xE0,0xA0,0x3C,0x15,0xCD,0x34,0xA2,0x77,0x57,0x79,0x72,0xDA,0x7A,
	0x67,0x79,0x15,0x5C,0x38,0xF1,0xBD,0x70,0xDE,0x6B,0x5B,0x09,0x13,0xC0,0x59,0x27,
	0x3A,0xF7,0x03,0x37,0x23,0xFE,0x44,0x5F,0x85,0x02,0x36,0x74,0xA0,0x69,0xFE,0x3C,
	0xF5,0xD5,0x87,0xC8,0x79,0x86,0x20,0x6C,0xEF,0xED,0x03,0xCF,0x7E,0x12,0xD6,0x87,
	0xC6,0x79,0x41,0xD6,0x43,0x8E,0x95,0x37,0x59,0x71,0xA0,0x50,0x14,0x50,0x83,0x20,
	0x35,0x16,0xF8,0x06,0x99,0x6E,0x42,0xC4,0x54,0x53,0x2C,0xAE,0xB0,0x56,0xE5,0x63,
	0x00,0xB4,0xA8,0x15,0x63,0xEA,0xB4,0xE5,0xDD,0x2D,0xEF,0x8D,0x51,0xD4,0x2C,0x89,
	0xE5,0x44,0x96,0x5D,0x42,0x05,0x28,0x5C,0x43,0x9C,0x31,0x56,0x07,0x82,0x48,0x3C,
	0x33,0xD9,0x56,0x4D,0x32,0x57,0x62,0x0A,0xF1,0x90,0x55,0x08,0x5B,0xA6,0x91,0x85,
	0x9D,0x90,0xE8,0x30,0xE6,0xCA,0x2B,0x9C,0x30,0x68,0x59,0x4F,0xC6,0x55,0xF4,0xDC,
	0x0D,0x66,0x1A,0x30,0x64,0x56,0x66,0x31,0x45,0xA0,0x98,0x5A,0xC6,0x08,0x17,0x1A,
	0x25,0x11,0x95,0x84,0x86,0x26,0x71,0xF5,0x9C,0x19,0x50,0xE6,0xF8,0xC4,0x41,0x05,
	0x60,0xB6,0xCA,0x80,0xA6,0xB2,0x45,0xC1,0xA3,0x99,0x75,0x07,0x63,0x76,0x6D,0xF0,
	0x38,0xA2,0x93,0xDB,0x1C,0x7B,0xAC,0x18,0xFF,0x8F,0xF9,0x08,0x54,0xA4,0x41,0x12,
	0x78,0x95,0x0B,0xB1,0x14,0x32,0x0C,0x1E,0x46,0xDA,0xE1,0x20,0xAF,0x24,0xEA,0x37,
	0xA2,0x80,0xCA,0xC2,0xD8,0x99,0xB5,0x7E,0x0A,0xE3,0x8B,0x74,0xC1,0x3C,0x6A,0x15,
	0x91,0x3D,0x05,0xD8,0xDA,0x98,0x4F,0xB0,0xF4,0xA7,0x4E,0x96,0xB1,0xAB,0x88,0x56,
	0xBE,0xED,0x69,0xDF,0x67,0xBF,0x02,0x10,0x4C,0x8A,0x86,0x6E,0xBB,0x26,0xB2,0xC5,
	0xCA,0xEA,0x15,0x48,0x34,0x66,0x7B,0xD9,0xB8,0x87,0x9C,0xF6,0x9A,0x35,0x54,0xEC,
	0x01,0x70,0xA9,0x55,0x0E,0x47,0x0A,0x9E,0x57,0x14,0x01,0x25,0x8D,0x6D,0x1A,0x59,
	0x18,0x8D,0xCB,0xC0,0x09,0xA6,0x0B,0x1C,0x6F,0xD5,0xC4,0xA4,0xD1,0x96,0x81,0x5B,
	0x62,0xCC,0xD9,0x52,0x52,0x85,0xA7,0x56,0xA2,0x6B,0x96,0xD2,0xF2,0x4B,0x4D,0x29,
	0x34,0x3A,0xE6,0x2E,0xAF,0xC9,0xCC,0x45,0xE6,0x0F,0x71,0xB2,0x01,0xD2,0x01,0xF5,
	0xE8,0x6A,0xE4,0x96,0xE2,0xC4,0xF2,0x72,0x71,0xA7,0x55,0xDC,0xCD,0xC5,0x9E,0x7E,
	0xCA,0xCE,0x56,0xB8,0x10,0x6D,0x40,0xCE,0x05,0xBE,0xF9,0xB2,0x13,0x34,0xB2,0xEB,
	0x1A,0x39,0x4D,0x3E,0x8D,0x4F,0xCF,0x2D,0xD7,0x9A,0x23,0x9A,0xB8,0x0C,0xFD,0x2A,
	0xBB,0x48,0xC7,0x05,0x0A,0x47,0x66,0xDD,0x62,0xD9,0x96,0x55,0x85,0x1D,0x61,0xB8,
	0x0B,0x35,0xD7,0xE8,0xC0,0x1E,0x1F,0xF8,0xCF,0x85,0xB8,0xE6,0x3A,0x25,0xD7,0xE7,
	0xAD,0x1A,0xA9,0x6B,0x6E,0x55,0x6D,0xF5,0x80,0x03,0xF3,0xC7,0x6C,0xDA,0x55,0x1A,
	0x59,0x29,0xD8,0x57,0xAE,0xD9,0xC8,0x4F,0xFF,0xBF,0xFC,0x95,0x2F,0xE5,0x94,0x43,
	0x5E,0x31,0x7B,0x39,0xCE,0x1A,0xD1,0xB1,0x55,0x3D,0xF2,0xC8,0xDC,0xE0,0xA2,0xE3,
	0x09,0x26,0x2A,0x68,0x1E,0xA0,0x88,0xA7,0xFE,0x9D,0xB2,0xE3,0x52,0xBB,0x7C,0xEB,
	0x20,0x73,0x73,0x6E,0x64,0xB4,0x5B,0x32,0x13,0x1B,0x5C,0x44,0x46,0xDB,0xD0,0xA1,
	0xF9,0xE2,0x5D,0x82,0x8E,0x79,0x67,0x44,0xD3,0xE2,0x03,0x8A,0xAB,0xCE,0x22,0x0A,
	0x5C,0x6E,0xAA,0xE7,0xC3,0xC7,0xE7,0xA0,0xE8,0xDE,0x00,0xB1,0xD0,0xE3,0x6B,0x0E,
	0x78,0x39,0x72,0xD0,0xC3,0xC8,0xC3,0x8A,0x93,0x12,0x0D,0x5F,0xC3,0x0F,0xDE,0x89,
	0x8D,0x32,0x02,0x37,0xEE,0xF1,0x33,0x24,0x2F,0x81,0x94,0x98,0xEF,0xD5,0x5F,0x95,
	0x56,0xD0,0x0E,0x47,0xB0,0x7E,0x2C,0x5F,0xF4,0xB9,0x90,0x1A,0xAA,0x1F,0xAB,0xD4,
	0x60,0x9E,0x7C,0xE9,0xEF,0x46,0x47,0x6B,0x5B,0x9B,0xF2,0x67,0x1D,0xA9,0xF9,0xCC,
	0x7F,0xFF,0x93,0x0A,0x76,0xE6,0x86,0xA2,0xD8,0xF0,0xE2,0x55,0x01,0x5A,0x8D,0xA9,
	0x74,0x37,0xAF,0xD9,0x89,0xED,0x2D,0xA3,0xEB,0xE0,0xFF,0xD0,0x91,0x1D,0x67,0xD0,
	0x81,0x01,0xAD,0xE0,0xDD,0xBC,0x8E,0x55,0x8F,0x08,0x86,0x4E,0x6F,0xCA,0x8A,0xE1,
	0x63,0x36,0x28,0xB9,0x09,0x85,0x42,0x7A,0x11,0x74,0xA1,0x0C,0xF1,0xB6,0xBD,0xBB,
	0xD1,0x0E,0x4A,0x8C,0x1B,0x1A,0x1B,0x38,0x10,0x86,0x86,0xE5,0x50,0x87,0xA8,0xB9,
	0xCF,0xA7,0x22,0x20,0x3A,0xBD,0x85,0x2A,0x48,0x30,0x6A,0xDD,0xD4,0x76,0x77,0x44,
	0x1D,0xE6,0xAD,0x76,0xEA,0x63,0x94,0x9E,0xFF,0x2A,0x76,0x89,0xD6,0xA5,0xEB,0x33,
	0x3B,0x63,0x4F,0x15,0x6B,0x92,0x3D,0xA8,0xE1,0xC9,0x04,0xD4,0xAB,0x92,0x30,0x4A,
	0x02,0x17,0x03,0x02,0x50,0x7F,0x22,0x1C,0xA1,0xCA,0x7E,0x16,0xAF,0xC3,0xEC,0x2D,
	0x75,0x92,0x61,0x15,0x7A,0xD8,0x60,0x21,0xC9,0x51,0x6E,0x8C,0x3A,0xE1,0x8A,0xF6,
	0x0E,0x96,0xBA,0xB4,0x11,0xCB,0x50,0x87,0x2A,0x5C,0x0C,0x75,0xF7,0xB9,0x6F,0xB5,
	0x26,0x7B,0x92,0xE8,0xA0,0xB2,0x84,0xF8,0x93,0x14,0xD5,0x41,0x77,0xC1,0xD9,0x54,
	0x1A,0xB9,0xB5,0x37,0x8F,0x94,0x08,0x88,0x18,0xE9,0xA4,0x22,0x2A,0xF9,0x8C,0x00,
	0x2E,0x90,0x54,0x03,0x10,0x8E,0x6B,0xE2,0x08,0x99,0x5A,0x91,0xD1,0x1D,0x3E,0xB4,
	0xC4,0x4C,0xC8,0xD2,0x16,0x54,0x4A,0x4E,0x75,0x6E,0x42,0x5F,0xEC,0x82,0x17,0x2D,
	0xF6,0x31,0x05,0x31,0x3E,0xC3,0x62,0x06,0xB6,0x68,0x00,0x5D,0x71,0x86,0x98,0xAA,
	0x4B,0x65,0x01,0x86,0x62,0x96,0xDC,0xB1,0x92,0x24,0xFB,0x21,0x1B,0x4B,0x7A,0xE6,
	0x32,0x64,0x4E,0xE9,0x2A,0x6E,0x04,0x49,0x47,0x96,0x59,0x1A,0x7D,0xCC,0xED,0x99,
	0x47,0x70,0xA0,0x1D,0xA1,0xD3,0x80,0x15,0x82,0x02,0x33,0x8E,0xE1,0x1D,0x6B,0x6A,
	0x36,0x00,0x03,0x62,0xC8,0x91,0x3A,0x99,0x26,0x30,0x73,0x13,0x10,0x64,0xDE,0x88,
	0x07,0x80,0xB1,0x4C,0xE4,0x8A,0x47,0x0E,0x62,0x5A,0x89,0x8C,0x2B,0x7C,0x48,0x44,
	0x44,0x63,0x21,0xDF,0xD1,0xC6,0x3F,0x37,0x99,0x9D,0x75,0xB8,0xF5,0x48,0x41,0xF0,
	0x0A,0x68,0x80,0x30,0x91,0x64,0x54,0x71,0xFF,0x23,0x25,0x5C,0xAE,0x9D,0x3B,0x0B,
	0xE3,0xBB,0x4C,0x35,0x04,0x55,0xCE,0x11,0x0E,0x66,0xBB,0x03,0x0F,0x86,0x64,0xAD,
	0xDD,0xC5,0xB1,0x3A,0x13,0x35,0xD5,0x3C,0x66,0xF8,0xD1,0xAE,0x18,0xAB,0x58,0x5B,
	0x39,0x44,0x7C,0xBC,0xE6,0x27,0x86,0x02,0x54,0xA5,0x5B,0x98,0xA1,0x20,0x07,0x79,
	0x94,0xB7,0xB4,0x93,0x18,0x36,0x62,0x21,0x4A,0x6B,0x52,0x50,0x33,0x85,0xA4,0x14,
	0x11,0x9A,0xA7,0x24,0x66,0x75,0x12,0xEF,0x0C,0xAA,0x4D,0x2B,0x5A,0x61,0x41,0xFF,
	0xD4,0x0D,0x43,0x42,0xB3,0x20,0x4F,0x22,0x9B,0xE9,0xD0,0xA3,0x20,0xA8,0x02,0x34,
	0xA5,0x60,0x25,0x63,0x27,0x03,0x91,0x45,0x79,0x02,0x73,0x21,0xDF,0xA3,0x86,0x2A,
	0x1D,0xFA,0x55,0xB1,0x4A,0xC6,0x14,0x81,0x0C,0x8D,0x5C,0xFF,0x10,0x02,0x1B,0xFD,
	0x44,0x4F,0x55,0x0D,0xAB,0x5A,0xB6,0x98,0x8B,0x8C,0x58,0x2C,0x04,0xDA,0x88,0xC0,
	0x1A,0x61,0x11,0x16,0x85,0xAC,0x68,0xAC,0xB3,0x94,0x89,0xF2,0xA0,0x12,0x58,0xA4,
	0x12,0x22,0x58,0x2D,0xC0,0x2B,0xB3,0xC4,0x2A,0xD9,0xC4,0xCE,0xF1,0xA5,0x2B,0x39,
	0x06,0x7A,0x04,0x73,0xBB,0xBD,0x4A,0x96,0x1B,0x62,0x01,0x01,0x46,0xB6,0x70,0x14,
	0x45,0x95,0x69,0x1B,0x5D,0xB2,0xD6,0x27,0x22,0x1B,0xA1,0x5B,0xED,0xF5,0x1C,0x8A,
	0xBD,0x88,0x2C,0x6B,0x51,0xD6,0x13,0x1C,0xE7,0x24,0xA0,0xE9,0x8E,0x89,0x64,0xE9,
	0xD0,0xCB,0xE2,0xE5,0xB6,0x58,0x75,0x62,0x63,0x7B,0xE1,0x96,0xFE,0x8D,0xCD,0xB5,
	0x1F,0xFD,0x6D,0xB2,0xBA,0xE5,0xC4,0xDA,0x5B,0xE6,0x46,0x1D,0x88,0xE8,0x5F,0x08,
	0x79,0xEB,0x15,0x5F,0x22,0xE6,0xB8,0x2B,0x13,0x45,0x35,0x41,0xF8,0x96,0x9E,0xC2,
	0x90,0xB1,0xCA,0xA3,0x4B,0x4B,0x03,0x2B,0x50,0x63,0x40,0x41,0x10,0x9A,0xB9,0x88,
	0x75,0x49,0x6B,0xC6,0xC6,0x9A,0x96,0x4C,0xE8,0x15,0x6F,0x5F,0x6F,0x75,0xD6,0xF6,
	0xBA,0x77,0xA9,0xA0,0x02,0x6F,0x47,0xEB,0xA9,0xD4,0xFB,0x9E,0x96,0x89,0x06,0x0B,
	0xB0,0x73,0xFD,0x6B,0x94,0xF3,0xF6,0x97,0xC0,0x8D,0xDD,0xA9,0x7D,0x11,0x4C,0x5E,
	0xE4,0x21,0x38,0x01,0x00,0x3B,   '%','E','N','D'};
	
//Nur Dateiname und Pointer eintragen	
WEBPAGE_ITEM WEBPAGE_TABLE[] = // Befehls-Tabelle
{
	{"index.htm",Page1},
	{"status.htm",Page2},
	{"styles.css",Page3},
	#if USE_CAM
	{"camera.jpg",bild_balken},
	#endif //USE_CAM
	{"raket.gif",bild_rakete},
	{"balke.gif",bild_balken},
	{"ledon.gif",led_on},
	{"ledoff.gif",led_off},
	{NULL,NULL} 
};

#endif //_WEBPAGE_H,0x00
