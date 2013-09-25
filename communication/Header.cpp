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

#include "Header.h"
#include "Buffer.h"

using namespace std;

#define ANSWER_BIT (1<<31)

namespace Rhoban
{
    ui32 Header::_uid = 0;

    Header::Header()
    {
        clear();
        uid = _uid++;
        answer = false;
        incoming = false;
    }

    void Header::read_header(const char * raw_header)
    {
        uid = Encodings::decode_uint(raw_header + uid_offset); // Unique message id
        source = Encodings::decode_ushort(raw_header + source_offset);; // Destination of message
        destination = Encodings::decode_ushort(raw_header + destination_offset);; // Destination of message
        command = Encodings::decode_uint(raw_header + command_offset); // Command of message
        length = Encodings::decode_uint(raw_header + length_offset); // Content length of the message

        answer = (command & ANSWER_BIT)!=0;
        command &= ~ANSWER_BIT;
    }

    void Header::write_header(char * raw_header)
    {
        ui32 acommand = command;

        if (answer) {
            acommand |= ANSWER_BIT;
        }

        Encodings::encode_uint(uid, (char*) raw_header + uid_offset);
        Encodings::encode_ushort(source, (char*) raw_header + source_offset);
        Encodings::encode_ushort(destination, (char*) raw_header + destination_offset);
        Encodings::encode_uint(acommand, (char*) raw_header + command_offset);
        Encodings::encode_uint(length, (char*) raw_header + length_offset);
    }

    void Header::clear()
    {
        uid = 0;
        destination = 0;
        command = 0;
        length = 0;
    }

    ui32 Header::getUid()
    {
        return uid;
    }
    ui32 Header::getDestination()
    {
        return destination;
    }
    ui32 Header::getCommand()
    {
        return command;
    }
    ui32 Header::getLength()
    {
        return length;
    }
    void Header::setUid(ui32 uid)
    {
        this->uid = uid;
    }
    void Header::setDestination(ui16 destination)
    {
        this->destination = destination;
    }
    void Header::setCommand(ui32 command)
    {
        this->command = command;
    }
    void Header::setLength(ui32 length)
    {
        this->length=length;
    }

}
