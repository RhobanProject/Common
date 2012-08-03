/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef THREADEDCOMPONENT_H
#define THREADEDCOMPONENT_H

#include <threading/Thread.h>

#include "ServerComponent.h"

namespace Rhoban
{
    /*
     * A threaded component is a component living in a thread
     */

    class ThreadedComponent : public Thread , public ServerComponent
    {
        public:
            ThreadedComponent();
            virtual ~ThreadedComponent();
    };
}

#endif // THREADEDCOMPONENT_H
