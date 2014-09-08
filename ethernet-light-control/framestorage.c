/*----------------------------------------------------------------------------
 Copyright:      Stefan Krupop  mailto: mail@stefankrupop.de
 Author:         Stefan Krupop
 Remarks:        
 known Problems: none
 Version:        17.01.2009
 Description:    Implementation des ArtNet-Protokolls für DMX-Übertragung über Ethernet

 Dieses Programm ist freie Software. Sie können es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation veröffentlicht, 
 weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder späteren Version. 

 Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, 
 daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/
#include "can2udp.h"
#include "enocean.h"
#include "shackbus.h"
#include "framestorage.h"

#ifdef USE_CANFRAMESTORAGE

#include <stdio.h>
#include <avr/interrupt.h>

#include "networkcard/enc28j60.h"
#include <string.h>

#include "can.h"


can_frame_t canframestorage_data[CANFRAMESTORAGE_DATA_SIZE];


// ----------------------------------------------------------------------------
// initialization canframestorage_init()
void canframestorage_init(void) {
    uint8_t i;
    for (i=0;i<CANFRAMESTORAGE_DATA_SIZE;i++) {
        canframestorage_item_clear(i);
    }


 return;
}



// ----------------------------------------------------------------------------
// clear item
void canframestorage_item_clear(uint8_t unit_id) {
    memset(&canframestorage_data[unit_id],0,sizeof(can_frame_t));    
    return;
}


// ----------------------------------------------------------------------------
// find first free item
uint8_t canframestorage_item_next(void) {
    uint8_t i;
    for (i=0;i<CANFRAMESTORAGE_DATA_SIZE;i++) {
        if (canframestorage_data[i].state == 0)
            return i;
    }
    return 255;
}





#endif
