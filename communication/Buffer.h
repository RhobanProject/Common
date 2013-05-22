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
 * Buffer.h
 *
 *  Created on: 21 mars 2011
 *      Author: hugo
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <vector>
#include <memory.h>
#include <string>
#include "types.h"
#include "Encodings.h"

using namespace std;

#define BUFFER_INI_SIZE 64

namespace Rhoban
{
    /*!
     * The buffer structure used by messages
     */
    class Buffer
    {
        public:
            //place where the data is stored
            //we do not use a string because
            //strings are not compatible with system calls like writing from a socket
            char *buffer;

            Buffer();

            /* copy constructor */
            Buffer(const Buffer&);

            Buffer & operator=(const Buffer& o);

            Buffer(char * buf, ui32 siz);

            void alloc(ui32 new_size);

            virtual ~Buffer();

            /*!
             * assigns a new char * to the buffer
             * no copy is performed, the pointer is used as the pointer's buffer
             */
            void assign(char * data, ui32 data_siz, ui32 buffer_siz);

            /*!
             * write data in the buffer
             * a copy is performed
             */
            void write(const char * data, ui32 siz);
            void write(string str);

            /*!
             * clears the buffer
             */
            virtual void clear();

            /*!
             * Following methods are used by the msg class
             */

            /**
              @brief read int from content base on offset
              @param offset offset to read the int
              */
            ui32 read_uint(ui32 offset);
            int read_int(ui32 offset);

            bool read_bool(ui32 offset);

            /**
              @brief read a float from the internal buffer with an offset
              */
            float read_float(ui32 offset);

            /**
              @brief read a string from content base on offet and with a size
              a copy is performed
              @param offset offset to read the int
              @param size size of the string
              @return the string readed
              */
            string read_string(ui32 siz, ui32 offset);

            /**
              @brief read a char array from content base on offet and with a size
              a copy is performed
              @param offset offset to read the int
              @param size size of the string
              @return the string readed
              */
            vector<ui8> read_array(ui32 siz, ui32 offset);
            void read_array(ui32 siz, ui32 offset, vector<ui8> & data);

            ui32 getSize();
            void setSize(ui32 size);
            char *getBuffer();

        protected:
            //size of the data stored in the buffer
            ui32 size;

            //total length of the buffer, in bytes
            ui32 buffer_size;

            //remembers whether the char * was created by the buffer itself
            //or came from elsewhere.
            //in the first case on destroy the buffer will clean after itself
            bool owned;

            /*!
             * append data to the buffer
             * a copy is performed
             */
            void append_to_buffer(const char * data, ui32 siz);
    };
}

#endif /* BUFFER_H_ */
