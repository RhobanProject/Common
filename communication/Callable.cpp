#include "Callable.h"

namespace Rhoban
{
    Message *Callable::call(Message *msg_in)
    { 
        return call(msg_in, new Message()); 
    };
}
