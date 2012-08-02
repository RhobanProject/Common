/*
 * component.h
 *
 *  Created on: 12 mars 2011
 *      Author: hugo
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <string>
#include <map>

#include "Thread.h"
#include "Mutex.h"

#include "Message.h"

using namespace Rhoban;

/*
 * A component can be called
 *
 * Typically, the components are called by clients of the server
 * The server itself is a component
 */
namespace Rhoban
{
    class ServerComponent
    {

        public:
            ServerComponent();

            virtual ~ServerComponent();

            Message * call(Message * msg_in);

            virtual Message * call(Message * msg_in, Message * msg_out)=0;

            Message *safe_call(Message *msg_in, Message *msg_out);

        private:
            Mutex _access;
            void lock(){_access.lock();}
            void unlock(){_access.unlock();}

    };
}


#endif /* COMPONENT_H_ */
