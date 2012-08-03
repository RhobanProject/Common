/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
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
    union floatAndInt
    {
        float floatValue;
        ui32 intValue;
    };

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
        union floatAndInt fai;
        fai.floatValue = value;

        encode_uint(fai.intValue, buf);
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
        union floatAndInt fai;
        fai.intValue = decode_uint(buf);
        return fai.floatValue;
    }

    double Encodings::decode_double(const char * buf)
    {
        return((double)decode_float(buf));
    }
}
