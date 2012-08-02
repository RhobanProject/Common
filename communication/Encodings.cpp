#include <iostream>
#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include "Encodings.h"

using namespace std;

namespace Rhoban
{
  void Encodings::encode_int(int value, char * buf)
  {
    value = htonl(value);
    *(int*)buf = value;
  }

  void Encodings::encode_uint(ui32 value, char * buf)
  {
    value = htonl(value);
    *(int*)buf = value;
  }

  void Encodings::encode_float(float value, char * buf)
  {
    ui32 i;
    (*(float*)&i) = value;
    encode_uint(i, buf);
  }

  void Encodings::encode_double(double value, char * buf)
  {
    encode_float((float)value, buf);
  }

  ui32 Encodings::decode_uint(const char * buf)
  {
    return (ui32)ntohl(*(int*)buf);
  }

  int Encodings::decode_int(const char * buf)
  {
    return (int)ntohl(*(int*)buf);
  }

  float Encodings::decode_float(const char * buf)
  {
    ui32 i = decode_uint(buf);
    return *(float*)(&i);
  }

  double Encodings::decode_double(const char * buf)
  {
    return((double)decode_float(buf));
  }
}
