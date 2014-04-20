/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Authors:        Ulrich Escher, Georg-Johann Lay(http://www.gjlay.de/pub/c-code/fifo.html)
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    non-blocking Fifo Headerfile

 implement a non-blocking fifo. with many funktions

------------------------------------------------------------------------------*/

#include "fifo.h"
#include <avr/pgmspace.h>
#include <string.h>


void fifo_init (fifo_t *f, uint8_t *buffer, const uint8_t size)
{
	memset(buffer,0,size);
	
	f->count = 0;
	f->pread = f->pwrite = buffer;
	f->read2end = f->write2end = f->size = size;
}

uint8_t fifo_get (fifo_t *f)
{
	uint8_t *pread = f->pread;
	uint8_t data = *(pread++);
	uint8_t read2end = f->read2end;
	
	if (--read2end == 0)
	{
		read2end = f->size;
		pread -= read2end;
	}
	
	f->pread = pread;
	f->read2end = read2end;
	
	uint8_t sreg = SREG;
	cli();
	f->count--;
	SREG = sreg;
	
	return data;
}

uint8_t fifo_get_var (fifo_t *f, uint8_t *data, uint8_t len)
{
	if (f->count < len )		return 0;
	uint8_t i;
	for (i=0;i < len; i++) {
		*(data + i) = (uint8_t) fifo_get (f);	
	}
	return 1;
}

uint8_t fifo_read (fifo_t *f)
{
	uint8_t *pread = f->pread;
	uint8_t data = *pread;
	return data;
}


uint8_t fifo_read_var (fifo_t *f, uint8_t *data, uint8_t len)
{
	if (f->count < len )		return 0;
	uint8_t i;
	uint8_t *pread = f->pread;
	uint8_t read2end = f->read2end;
	for (i=0;i < len; i++)
	{
		*(data + i) = *(pread++);
		if (--read2end == 0)
		{
			read2end = f->size;
			pread -= read2end;
		}
	}
	return 1;

}



int fifo_get_count (fifo_t *f)
{
	return (int) f->count;	
}

uint8_t fifo_put (fifo_t *f, const uint8_t data)
{
	if (f->count >= f->size)
		return 0;
		
	uint8_t * pwrite = f->pwrite;
	
	*(pwrite++) = data;
	
	uint8_t write2end = f->write2end;
	
	if (--write2end == 0)
	{
		write2end = f->size;
		pwrite -= write2end;
	}
	
	f->write2end = write2end;
	f->pwrite = pwrite;

	uint8_t sreg = SREG;
	cli();
	f->count++;
	SREG = sreg;
	
	return 1;
}

uint8_t fifo_put_var (fifo_t *f, const uint8_t *data, uint8_t len)
{
	uint8_t i, ret = 0;
	for (i=0;i < len; i++) {
		ret = fifo_put (f, *(data + i));
		if (ret == 0)
			return ret;
	}
	
	return ret;
}

uint8_t fifo_put_var_p (fifo_t *f, const uint8_t *data, uint8_t len)
{
	uint8_t i, ret = 0;
	for (i=0;i < len; i++) {
		ret = fifo_put (f, pgm_read_byte(data++));
		if (ret == 0)
			return ret;
	}
	
	return ret;
}


int fifo_roll (fifo_t *f, uint8_t len)
{
	uint8_t i;
	for (i=0;i<len;i++)
		fifo_put( f, fifo_get(f) ); 

	return 0;
}

uint8_t fifo_transfer (fifo_t *f1, fifo_t *f2, uint8_t len)
{
	uint8_t real_len = f2->count;
	if (real_len < len) len = real_len;

	uint8_t i;
	for (i=0;i<len;i++)
		fifo_put( f1, fifo_get(f2) ); 
	return len;
}
