#ifndef THREADEDCOMPONENT_H
#define THREADEDCOMPONENT_H

#include "ServerComponent.h"
#include "Thread.h"

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
