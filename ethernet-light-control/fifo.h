/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Authors:        Ulrich Escher, Georg-Johann Lay(http://www.gjlay.de/pub/c-code/fifo.html)
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    non-blocking Fifo

 implement a non-blocking fifo. with many funktions

------------------------------------------------------------------------------*/

#ifndef _FIFO_H_
#define _FIFO_H_


#include <avr/io.h>
#include <avr/interrupt.h>


typedef struct
{
	uint8_t count;       // # Zeichen im Puffer
	uint8_t size;                 // Puffer-Größe
	uint8_t *pread;               // Lesezeiger
	uint8_t *pwrite;              // Schreibzeiger
	uint8_t read2end, write2end;  // # Zeichen bis zum Überlauf Lese-/Schreibzeiger
} fifo_t;

extern void fifo_init (fifo_t*, uint8_t* buf, const uint8_t size);
extern int fifo_get_count (fifo_t*);


extern uint8_t fifo_put (fifo_t*, const uint8_t data);
extern uint8_t fifo_put_var (fifo_t *f, const uint8_t *data, uint8_t len);
extern uint8_t fifo_put_var_p (fifo_t *f, const uint8_t *data, uint8_t len);
#define fifo_put_var_P(__f,__s,__i)         fifo_put_var_p(__f,PSTR(__s),__i)

extern uint8_t fifo_get (fifo_t*);
extern uint8_t fifo_get_var (fifo_t *f, uint8_t *data, uint8_t len);
extern uint8_t fifo_read (fifo_t*);
extern uint8_t fifo_read_var (fifo_t *f, uint8_t *data, uint8_t len);


extern int fifo_roll (fifo_t*,uint8_t len);
extern uint8_t fifo_transfer (fifo_t *f1, fifo_t *f2, uint8_t len);


#endif /* _FIFO_H_ */
