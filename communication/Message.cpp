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
#include <unistd.h>
#include <iostream>
#include "Message.h"
#include "Header.h"
#include "Buffer.h"

using namespace std;

namespace Rhoban
{
    Message::Message() : Header()
    {
        alloc(MSG_MIN_SIZE);
        init();
    }

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

    void Message::append(ui8 value)
    {
        alloc(size + sizeof(ui8));
        buffer[size] = value;
        size += sizeof(ui8);
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
    void Message::append(vector< TYPE > &values) \
    { \
        int length = values.size(); \
        append((ui32) length); \
        for (int i = 0; i < length; i++) { \
            append(values[i]); \
        } \
    } 

#define APPEND_VECTOR(TYPE) \
    _APPEND_VECTOR(TYPE) \
    _APPEND_VECTOR(vector< TYPE >) \
    _APPEND_VECTOR(vector< vector< TYPE > >) \
    _APPEND_VECTOR(vector< vector< vector < TYPE > > >)


    APPEND_VECTOR(ui8);
    APPEND_VECTOR(string);
    APPEND_VECTOR(ui32);
    APPEND_VECTOR(int);
    APPEND_VECTOR(double);
    APPEND_VECTOR(float);

    ui32 Message::read_uint(void)
    {
        cursor += sizeof(ui32);
        return Buffer::read_uint(cursor-sizeof(ui32));
    }

    int Message::read_int(void)
    {
        cursor += sizeof(ui32);
        return Buffer::read_int(cursor-sizeof(ui32));
    }

    float Message::read_float(void)
    {
        cursor += sizeof(float);
        return Buffer::read_float(cursor-sizeof(float));
    }

    double Message::read_double(void)
    {
        return (double) read_float();
    }

    string Message::read_string(void)
    {
        ui32 length = read_uint();
        cursor += length;
        return Buffer::read_string(length,cursor-length);
    }

    vector<ui8> Message::read_array(void)
    {
        ui32 length = read_uint();
        cursor += length;
        return Buffer::read_array(length, cursor- length);
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

    void Message::init()
    {
        size = MSG_HEADER_SIZE;
        cursor = MSG_HEADER_SIZE;
    }

    void Message::clear()
    {
        Header::clear();
        Buffer::clear();

        init();
    }

    char* Message::getRaw()
    {
        length = size - MSG_HEADER_SIZE;
        write_header(buffer);
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

