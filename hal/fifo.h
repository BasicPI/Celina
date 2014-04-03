/*****************************************************************************
 * FileName:     fifo.h
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
#ifndef _FIFO
#define _FIFO

/*****************************************************************************
 * Struct:       FIFO
 *
 * Description:  FIFO Header.
 *
 *			                [x]	->last (out)
 *			                [x]
 *			                [x]
 *			                [x]
 *	            first (in)->[ ]
 *
 ****************************************************************************/
typedef struct _fifo
{
	int first;
	int last;
	int size;
	char buf[1];
}FIFO;

void FIFOCreate(char *fifo, int size);
int FIFOWrite(char * fifo, char * buf, int size);
int FIFORead(char * fifo, char *buf, int size);
int FIFOGetFreeBlockSize(FIFO *f);
int FIFOGetNextBlockSize(FIFO *f);
int FIFOReadByte(char *fifo, char *b);
int FIFOWriteByte(char *fifo, char *b);
int FIFOGetQueued(char *fifo);

#endif

