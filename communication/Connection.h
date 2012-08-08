/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef __CONNECTION_H_
#define __CONNECTION_H_

namespace Rhoban{
  class Connection;
}

#include <cstdlib>
#include <cstdio>
#include <sockets/TCPClient.h>
#include "Message.h"
#include "Mailbox.h"

using namespace std;

namespace Rhoban
{  
  typedef void sendCallback(Message *);

  class Connection : public TCPClient
  {
  public:
    Connection();
    void sendMessage(Message *message);
    Message *sendMessageRecieve(Message *message, int timeout=100);
    void sendMessageCallback(Message *message, sendCallback *callback);
    void startMailbox();
    Message *getMessage();
    Message *getMessage(Message *message);
  protected:
    Mailbox mailbox;
  };
}

#endif // __CONNECTION_H_
