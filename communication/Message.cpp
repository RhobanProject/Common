/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <string>
#ifndef MSVC
#include <unistd.h>
#endif
#include <iostream>
#include "Message.h"
#include "Header.h"
#include "Buffer.h"

using namespace std;

namespace Rhoban
{
    Message::Message() : Header(), Buffer()
    {
        Buffer::alloc(MSG_MIN_SIZE);
		size = MSG_HEADER_SIZE;
		cursor = MSG_HEADER_SIZE;
    }

    Message::Message(const Message & o) : Header(o), Buffer(o), cursor(o.cursor)
    {

    }

    /*
    void Message::operator=(const Message& o)
    {
    	Header::operator =(o);
    	Buffer::operator =(o);
    	cursor = o.cursor;
    }
    */


    Message::Message(ui32 size, char *buffer) : Header()
    {
        destination = 0;
        command = 0;
        length = 0;
        cursor = MSG_HEADER_SIZE;

        if (buffer) {
            assign(buffer, size, size);
        } else {
            alloc(size);
        }
    }

    Message::~Message()
    {
    }

    void Message::append(const string &value)
    {
        append((unsigned int) value.size());
        append_to_buffer(value.c_str(),value.size());
    }
    
    void Message::append(const char *str)
    {
        append(string(str));
    }

    void Message::append(ui8 value)
    {
        alloc(size + sizeof(ui8));
        buffer[size] = value;
        size += sizeof(ui8);
    }

    void Message::append(bool value)
    {
    	append((ui8) value);
    }

    void Message::append(ui32 value)
    {
        alloc(size + sizeof(ui32));
        Encodings::encode_uint(value, buffer + size);
        size += sizeof(ui32);
    }

    void Message::append(int value)
    {
        alloc(size + sizeof(ui32));
        Encodings::encode_int(value, buffer + size);
        size += sizeof(ui32);
    }

    void Message::append(float value)
    {
        alloc(size + sizeof(ui32));
        Encodings::encode_float(value,buffer + size);
        size += sizeof(ui32);
    }
    void Message::append(double value)
    {
        alloc(size + sizeof(ui32));
        Encodings::encode_double(value, buffer + size);
        size += sizeof(ui32);
    }

    void Message::append(const char * data, ui32 size_)
    {
        append(size_);
        append_to_buffer(data, size_);
    }

    Message & operator<< (Message & msg , ui8 & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , ui32 & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , int & val ){ msg.append(val); return msg; }
    Message & operator<< (Message & msg , float & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , double & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , string & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , vector<double> & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , vector<float> & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , vector<int> & val ) { msg.append(val); return msg; }
    Message & operator<< (Message & msg , vector<unsigned int> & val ) { msg.append(val); return msg; }

#define _APPEND_VECTOR(TYPE) \
    void Message::append(const vector< TYPE > &values) \
    { \
        size_t length = values.size(); \
        append((ui32) length); \
	for (size_t i = 0; i < length; i++) {	\
            append(values[i]); \
        } \
    } 

#define APPEND_VECTOR(TYPE) \
    _APPEND_VECTOR(TYPE) \
    _APPEND_VECTOR(vector< TYPE >) \
    _APPEND_VECTOR(vector< vector< TYPE > >) \
    _APPEND_VECTOR(vector< vector< vector < TYPE > > >)


	void Message::append(const vector< ui8 > &values)
	{ 
	size_t length = values.size(); 
	append((ui32)length); 
	append_to_buffer(values, values.size());
	}

	_APPEND_VECTOR(vector< ui8 >);
	_APPEND_VECTOR(vector< vector< ui8 > >);
	_APPEND_VECTOR(vector< vector< vector < ui8 > > >);
    APPEND_VECTOR(string);
    APPEND_VECTOR(ui32);
    APPEND_VECTOR(int);
    APPEND_VECTOR(double);
    APPEND_VECTOR(float);
    APPEND_VECTOR(bool);

    void Message::read(ui32 &value)
    {
        cursor += sizeof(ui32);
        value = Buffer::read_uint(cursor-sizeof(ui32));
    }

    ui32 Message::read_uint(void)
    {
        ui32 value;
        read(value);

        return value;
    }

    void Message::read(int &value)
    {
        cursor += sizeof(ui32);
        value = Buffer::read_int(cursor-sizeof(ui32));
    }

    int Message::read_int()
    {
        int value;
        read(value);

        return value;
    }

    void Message::read(float &value)
    {
        cursor += sizeof(float);
        value = Buffer::read_float(cursor-sizeof(float));
    }

    float Message::read_float()
    {
        float value;
        read(value);

        return value;
    }

    void Message::read(double &value)
    {
        value = (double)read_float();
    }

    double Message::read_double(void)
    {
        double value;
        read(value);

        return value;
    }

    void Message::read(string &value)
    {
        ui32 length = read_uint();
        cursor += length;
        value = Buffer::read_string(length,cursor-length);
    }

    string Message::read_string(void)
    {
        string value;
        read(value);

        return value;
    }

    void Message::read(bool &value)
    {
        cursor += sizeof(char);
        value = Buffer::read_bool(cursor - sizeof(char));
    }

    void Message::read(vector<bool>::reference value)
    {
        value = read_bool();
    }

    bool Message::read_bool()
    {
        bool value;
        read(value);

        return value;
    }

    void Message::read(vector<ui8> &value)
    {
        ui32 length = read_uint();
        cursor += length;
        Buffer::read_array(length, cursor- length, value);
    }

    ui8 Message::read_ui8(void)
    {
    	ui8 value;
    	read(value);
    	return value;
    }

    void Message::read(ui8 &value)
    {
    	if(cursor < size)
    	{
    		value = buffer[cursor++];
    	}
    	    else
    	      throw string("Buffer too small to read such a string");
    }

    vector<ui8> Message::read_array(void)
    {
        vector<ui8> value;
        read(value);

        return value;
    }

    void Message::read(vector< vector<ui8> > &value)
    {
        int length = read_uint();
        value.resize(length);
        for (int i = 0; i < length; i++) {
            read(value[i]);
        }
    }

    vector< vector<ui8> > Message::read_array_array()
    {
        vector< vector<ui8> > value;
        read(value);

        return value;
    }

#define _READ_ARRAY(TYPE, FUNCTION) \
    vector< TYPE > Message:: FUNCTION##_array () \
    { \
        vector< TYPE > values; \
        int length = read_uint(); \
        for (int i = 0; i < length; i++) { \
            values.push_back( FUNCTION () ); \
        } \
        return values; \
    } \
    \
    void Message::read(vector< TYPE > &value) { \
        int length = read_uint(); \
        value.resize(length); \
        for (int i = 0; i < length; i++) { \
            read(value[i]); \
        } \
    }

#define READ_ARRAY(TYPE, FUNCTION) \
    _READ_ARRAY(TYPE, FUNCTION ) \
    _READ_ARRAY(vector< TYPE >, FUNCTION ## _array) \
    _READ_ARRAY(vector< vector< TYPE > >, FUNCTION ## _array_array)

    READ_ARRAY(string, read_string);
    READ_ARRAY(ui32, read_uint);
    READ_ARRAY(int, read_int);
    READ_ARRAY(float, read_float);
    READ_ARRAY(double, read_double);
    READ_ARRAY(bool, read_bool);


    void Message::write_header(){
    	length = size - MSG_HEADER_SIZE;
    	Header::write_header(buffer);
    }

    void Message::clear()
    {
        Header::clear();
        Buffer::clear();

		size = MSG_HEADER_SIZE;
		cursor = MSG_HEADER_SIZE;
    }

	void Message::init()
	{
		size = MSG_HEADER_SIZE;
		cursor = MSG_HEADER_SIZE;
	}
    char* Message::getRaw()
    {
        write_header();
        return buffer;
    }

    void Message::print()
    {
        printf(" { ");
        printf("Destination : %d, Command : %d, UID : %d, Length : %d }\n",
                destination, command, uid, length);
    }

    void Message::rawPrint()
    {
        for (unsigned int i = 0; i < size; i++) {
            printf("%02X ", buffer[i]);
        }

        printf("\n");
    }
}

