/*
 * encodings.h
 *
 * Created on: 21 mars 2011
 * Author: hugo
 */

#ifndef ENCODINGS_H_
#define ENCODINGS_H_

#include <iostream>
#include "types.h"

using namespace std;

namespace Rhoban{
  class Encodings{
  public:
    /*!
     * encode and decode values to and from a buffer
     */
    static void encode_int(int value, char * buf);
    static void encode_uint(ui32 value, char * buf);
    static void encode_float(float value, char * buf);
    static void encode_double(double value, char * buf);

    /*!
     * todo: find a better encoding,
     * this one wont be properly decoded on every architecture
     */

    static ui32 decode_uint(const char * buf);
    static int decode_int(const char * buf);
    static float decode_float(const char * buf);
    static double decode_double(const char * buf);
  };
}

#endif /* ENCODINGS_H_ */
