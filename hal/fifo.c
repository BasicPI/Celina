/*****************************************************************************
 *  FileName:     fifo.c
 *
 * License/Copyright:
 *
 * Copyright (c) 2007, Jan Vidar Berger, Case Labs, Ltd. All rights reserved.
 * email:janvb@caselaboratories.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *	  this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *	  this list of conditions and the following disclaimer in the documentation
 *	  and/or other materials provided with the distribution.
 *  * Neither the name of the Case Labs, Ltd nor the names of its contributors
 *	  may be used to endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************/

#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Function:     FIFOCreate
 *
 * Description:  Takes any buffer and creates a fifo from it using the buffer
 *               head for fifo queue information.
 *
 * Parameters:   fifo    Buffer to be created.
 *               size    Size of buffer in bytes.
 ****************************************************************************/
void FIFOCreate(char *fifo, int size)
{
	FIFO *f=(FIFO *)fifo;
	f->first = 0;
	f->last = 0;
	f->size = size - (sizeof(int)*3);
}

/*****************************************************************************
  Function:     FIFOWrite

  Description:  Write butes into a fifo queue.

  Parameters:   fifo    ptr to fifo.
                buf     ptr to bytes to be be writtem into the queue.
                size    Number of bytes to be written.

  Return Value: Number of bytes written.
*****************************************************************************/
int FIFOWrite(char * fifo, char * buf, int size)
{
	int c=0;
	FIFO *f=(FIFO *)fifo;
	int x;

	int block = FIFOGetFreeBlockSize(f);

	if(block >= size)
	{
		memcpy(&f->buf[f->first],buf,size);
		x= f->first + size;
		if(x > f->size)
			x=0;
		f->first = x;
		return size;
	}
	else
	{
		for(c=0; c<size; c++)
		{
			f->buf[f->first]=buf[c];
			x= f->first + 1;
			if(x >= f->size)
				x=0;
			if(x==f->last)
				return c-1; /* full queue */
			f->first = x;
		}
		return c;
	}
}

/*****************************************************************************
  Function:     FIFORead

  Description:  Read bytes from a FIFO queue.

  Parameters:   fifo    ptr to fifo.
                buf     ptr at buffer receiving bytes.
                size    max number of bytes to be read.

  Return value: Number of bytes read.
*****************************************************************************/
int FIFORead(char * fifo, char *buf, int size)
{
	FIFO *f=(FIFO *)fifo;
	int b;
	int r;
	int c=0;
	int x;
	if(f->last == f->first)
		return 0; /* empty fifo */

	/* first block transfer */
	b = FIFOGetNextBlockSize(f);
	if(b <= size)
	{
		memcpy(buf, &f->buf[f->last],b);
		x = f->last + b;
		if(x >= f->size)
			x=0;
		f->last=x;
		c+=b;
	}

	/* 2nd optional block transfer */
	if(c < size)
	{
		b = FIFOGetNextBlockSize(f);
		if(b > 0)
		{
			r = min(b, size-c);
			memcpy(&buf[c],&f->buf[f->last],r);
			x = f->last + r;
			if(x >= f->size)
				x=0;
			f->last=x;
			c+=r;
		}
	}
	return c;
}

/*****************************************************************************
  Function:     FIFOGetFreeBlockSize

  Description:  Returns the size of the next continiously free space.

  Parameters:   f   ptr to FIFO struct.

  Return Value  Size of block in bytes.
*****************************************************************************/
int FIFOGetFreeBlockSize(FIFO *f)
{
	if(f->first == f->last)
	{
		f->first=f->last=0;
		return f->size-1;
	}
	else if(f->first >= f->last)
	{
		return f->size - f->first-1; 
	}
	else
	{
		return f->last - f->first-1;
	}
}

/*****************************************************************************
  Function:     FIFOGetNextBlockSize

  Description:  Return the number of continiously bytes in the queue.

  Parameters:   f   ptr to FIFO struct

  Return Value: number of continuous bytes in the queue.
*****************************************************************************/
int FIFOGetNextBlockSize(FIFO *f)
{
	if(f->first == f->last)
	{
		return 0;
	}
	else if(f->first > f->last)
	{
		return f->first - f->last; 
	}
	else
	{
		return f->size - f->last;
	}
}

/*****************************************************************************
  Function:     FIFOReadByte

  Description:  Will read a single byte from the queue. Provides better 
                perfomance than FIFORead on single or small byte xfers.

  Parameters:   fifo    ptr to fifo queue.

                b       ptr to address containing byte.

  Return Value: Number of bytes read.
*****************************************************************************/
int FIFOReadByte(char *fifo, char *b)
{
	FIFO *f=(FIFO *)fifo;
	int x;

    if(f->last == f->first)
		return 0; /* empty fifo */

	*b = f->buf[f->last];
	x = f->last +1;
	if(x >= f->size)
		x=0;
	f->last=x;

	return 1;
}

/*****************************************************************************
  Function:     FIFOWriteByte

  Description:  Write a single byte to the FIFO. Provide better performance
                on single or small block xfers than FIFOWrite.

  Parameters:   fifo    ptr to fifo.
                b       ptr to address receiving byte.

  Return Value: Number of bytes written.
*****************************************************************************/
int FIFOWriteByte(char *fifo, char *b)
{
	FIFO *f=(FIFO *)fifo;
	int x;

	f->buf[f->first]= *b;
	x= f->first +1;
	if(x > f->size)
		x=0;

    if(x==f->last)
		return 0;

    f->first = x;

    return 1;
}

/*****************************************************************************
  Function:     FIFOGetQueued

  Description:  Get bytes in fifo.

  Parameters:   fifo    ptr to fifo.

  Return Value: Bytes in fifo.
*****************************************************************************/
int FIFOGetQueued(char *fifo)
{
	FIFO *f=(FIFO *)fifo;
    if(f->last == f->first)
        return 0;
    else if (f->last < f->first)
        return f->first - f->last;
    else
        return (f->size-f->last) + f->first;
}



