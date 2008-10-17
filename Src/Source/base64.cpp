/*
Copyright (c) 2003 Z3R0 One. All Rights Reserved.

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation 
files (the "Software"), to deal in the Software without 
restriction, including without limitation the rights to use, 
copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom 
the Software is furnished to do so, subject to the following 
conditions: 

   The above copyright notice and this permission notice 
   shall be included in all copies or substantial portions 
   of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Original Author & Project Manager: Z3R0 One (z3r0_one@bbnk.dhs.org)
Project Start Date: October 18, 2000
*/
#include "base64.h"
#include <SupportDefs.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//UmF5bW9uZCBDLiBSb2RnZXJzIGlzIGEgaGVybyB0byBtb3N0IHBlb3BsZQo=
const char *base64::BASE64ARRAY="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64::e2s(unsigned char *in,unsigned char *out) 
{
	int ba[]={1,2,4,8,16,32,64,128};
	bool bits[24];
	int pos=0;
	for (int i=0; i<3; i++) {
		for (int j=7; j>=0; j--) {
			bits[pos++]=in[i]&ba[j];
		}
	}
	pos=0;
	uint8 n;
	for (int i=0; i<4; i++) {
		n=0;
		for (int j=5; j>=0; j--) {
			n|=bits[pos++]*ba[j];
		}
		out[i]=BASE64ARRAY[n];
	}
}
uint8 base64::findb64char(char which) 
{
	uint8 num=0;
	for (uint8 i=0; i<strlen(BASE64ARRAY); i++) {
		if (BASE64ARRAY[i]==which) {
			num=i;
			break;
		}
	}
	return num;
}
void base64::s2e(unsigned char *in, unsigned char *out)
{
	int ba[]={1,2,4,8,16,32,64,128};
	bool bits[24];
	int pos=0;
	uint8 n;
	for (int i=0; i<3; i++)
		if (in[i]=='=')
			in[i]='\0';
	for (int i=0; i<4; i++) {
		n=findb64char(in[i]);
		for (int8 j=5; j>=0; j--) {
			bits[pos++]=n&ba[j];
		}
	}
	pos=0;
	for (int i=0; i<3; i++) {
		n=0;
		for (int j=7; j>=0; j--) {
			n|=bits[pos++]*ba[j];
		}
		out[i]=n;	
	}
	
}
/**
\internal Pass in chunks of data in multiples of 4 bytes as input until the last chunk
is ready to be processed. And if it so happens that the input data is exactly divisible by 4, with or without
the padding character(s), then this function should complete successfully.

\internal if the data size is a multiple of 3 and there is no padding character, we can't be
sure that all the data is there... we have to assume that it is.
*/

void base64::decode(void *in, uint32 ilen, void *out, uint32 *olen) 
{
	uint32 b=ilen/4, q=ilen%4;
	uint32 ioff=0;
	uint32 ooff=0;
	if (q==0) {
		for (uint32 i=0; i<b; i++) {
			s2e(((unsigned char*)in)+ioff, ((unsigned char*)out)+ooff);
			ioff+=4;
			ooff+=3;
		}
		*olen=ooff;
	} else {
		//this needs to be handled, especially for messages divisible by 3...
		printf("Base-64 Decode - Remainder: %ld\n",q);
	}
	
}
/**
\internal Pass in data in multiples of 3 bytes; This should ensure that the padding character '=' is not included
too early. 
*/
void base64::encode(void *in, uint32 ilen,void *out,uint32 *olen) 
{
	uint32 b=ilen/3, q=ilen%3;
	uint32 ioff=0;
	uint32 ooff=0;
	for (uint32 i=0; i<b; i++) {
		e2s(((unsigned char*)in)+ioff,((unsigned char*)out)+ooff);
		ioff+=3;
		ooff+=4;
	}
	if (q!=0) {
		unsigned char *output=(unsigned char*)out;
		unsigned char ibuff[10],obuff[10];
		memset(ibuff,0,10);
		memset(obuff,0,10);
		memcpy(ibuff,(unsigned char*)in+ioff,q);
		e2s(ibuff,obuff);
		if (q==1) {
			memcpy(output+ooff,obuff,3);
			ooff+=3;
			memcpy(output+ooff,"==\0",3);
			ooff+=2;
		} else {
			memcpy(output+ooff,obuff,4);
			ooff+=4;
			memcpy(output+ooff,"=\0",1);
			ooff+=1;
		}
	}
	memset((unsigned char*)out+ooff,0,*olen-ooff);
	*olen=ooff;
}
uint32 base64::expectedencodedsize(uint32 in_size)
{
	uint32 size=0;
	size=(uint32)ceil(in_size*(4.0/3.0))+10;
	return size;
}

uint32 base64::expecteddecodedsize(uint32 in_size)
{
	uint32 size=0;
	size=(uint32)ceil(in_size*0.75);
	return size;
}


