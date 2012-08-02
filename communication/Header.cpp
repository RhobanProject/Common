#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <string>
#include <unistd.h>
#include <iostream>

#include "Header.h"
#include "Buffer.h"

using namespace std;

namespace Rhoban
{
  ui32 Header::_uid = 0;

  Header::Header()
  {
    clear();
    uid = _uid++;
  }

  void Header::read_header(const char * raw_header)
  {
    uid = Encodings::decode_uint(raw_header + uid_offset); // Unique message id
    destination = Encodings::decode_uint(raw_header + destination_offset);; // Destination of message
    command = Encodings::decode_uint(raw_header + command_offset); // Command of message
    length = Encodings::decode_uint(raw_header + length_offset); // Content length of the message
  }

  void Header::write_header(char * raw_header)
  {
    Encodings::encode_uint(uid, (char*) raw_header + uid_offset);
    Encodings::encode_uint(destination, (char*) raw_header + destination_offset);
    Encodings::encode_uint(command, (char*) raw_header + command_offset);
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
  void Header::setDestination(ui32 destination)
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
