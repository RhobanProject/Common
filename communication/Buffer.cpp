/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/*
 * Buffer.cpp
 *
 *  Created on: 21 mars 2011
 *      Author: hugo
 */
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

#include "Buffer.h"
#include "Encodings.h"
#include "Message.h"

#include "util.h"


namespace Rhoban
{
Buffer::Buffer(): buffer(0), size(0), buffer_size(0), owned(true)
{
	//alloc(MSG_HEADER_SIZE + BUFFER_INI_SIZE);
}

/* copy constructor */
Buffer::Buffer(const Buffer& o) : buffer(NULL), size(o.size), buffer_size(o.buffer_size), owned(true)
{
	buffer = (char *) malloc(buffer_size);
	if (buffer == NULL)
		throw string("Buffer:Constructor failed, Out of memory");
	memcpy(buffer,o.buffer,buffer_size);
}

Buffer & Buffer::operator=(const Buffer& o)
{
	if(this != &o)
	{
		size = o.size;
		alloc(size);
		memcpy(buffer, o.buffer, size);
	}
	return *this;
}

Buffer::Buffer(char * buf, size_t siz): buffer(buf), size(0), buffer_size(siz), owned(false)
{
}

Buffer::~Buffer()
{
	if(owned && buffer)
	{
		free(buffer);
		buffer = 0;
	}
}

void Buffer::alloc(size_t new_size)
{
	if(new_size <= buffer_size)
		return ;
	if(new_size>MSG_MAX_SIZE)
	{
		ostringstream os;
		os << new_size;
		throw string("Message too large "+os.str());
	}
	if(!buffer || !owned)
	{
		buffer = (char *)malloc(new_size * sizeof(char));
		owned  = true;
	}
	else
		buffer = (char *)realloc(buffer, (new_size + MSG_HEADER_SIZE) * sizeof(char) + 32);
	if(buffer)
	{
		buffer_size = new_size;
	}
	else
	{
		ostringstream os;
		os << "Could not allocate buffer of size " << new_size;
		throw os.str();
	}
}


void Buffer::assign(char * data, size_t data_siz, size_t buffer_siz)
{
	if(buffer && owned)
		free(buffer);
	owned = false;
	buffer = data;
	buffer_size=buffer_siz;
	size=data_siz;
}

ui32 Buffer::read_uint(ui32 offset)
{
	if(offset + sizeof(ui32) <= size)
		return Encodings::decode_uint((const char *) buffer+offset);
	else
		throw string("buffer too small to read uint at this offset");
}

int Buffer::read_int(ui32 offset)
{
	if(offset + sizeof(int) <=size)
		return Encodings::decode_int((const char *) buffer+offset);
	else
		throw string("buffer too small to read int at this offset");
}

bool Buffer::read_bool(ui32 offset)
{
	if( (offset + sizeof(char)) <=size)
		return (buffer[offset] != 0);
	else
		throw string("buffer too small to read bool at this offset");
}

float Buffer::read_float(ui32 offset)
{
	if(offset + sizeof(int) <=size)
		return Encodings::decode_float(buffer+offset);
	else
		throw string("buffer too small to read float at this offset");
}

void Buffer::write(const char * data, size_t siz)
{
	alloc(siz);
	memcpy(buffer,data,siz);
	size=siz;
}

void Buffer::append_to_buffer(const char * data, size_t siz)
{
	alloc(size+siz);
	memcpy(buffer+size,data,siz);
	size+=siz;
}

void Buffer::write(string str)
{
	write(str.c_str(), str.size());
}

void Buffer::clear()
{
	size = 0;
}

string Buffer::read_string(ui32 siz, ui32 offset)
{
	if(offset+siz<= size)
		return string(buffer + offset,siz);
	else
		throw string("Buffer too small ") + my_itoa(size) + " " + string("to read such a string of size ") + my_itoa(siz) + string("offfset ") + my_itoa(offset);
}

void Buffer::read_array(ui32 siz, ui32 offset, vector<ui8> & data)
{
	if(offset+siz<= size)
	{
		data.resize(siz);
		for(uint i=0;i<siz;i++)
			data[i] = buffer[offset+i];
	}
	else
		throw string("Buffer too small to read such a string");
}

vector<ui8> Buffer::read_array(ui32 siz, ui32 offset)
{
	vector<ui8> res;
	read_array(siz,offset,res);
	return res;
}

size_t Buffer::getSize()
{
	return size;
}

void Buffer::setSize(size_t size)
{
	this->size = size;
}

char *Buffer::getBuffer()
{
	return buffer;
}

}
