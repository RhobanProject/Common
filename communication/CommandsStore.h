/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef _COMMANDSSTORE_H
#define _COMMANDSSTORE_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include "MessageBuilder.h"

using namespace std;

namespace Rhoban
{
  class CommandsStore
  {
  public:
    CommandsStore();
    ~CommandsStore();
    MessageBuilder *getBuilder();
  protected:
    MessageBuilder *builder;
  };
}

#endif //_COMMANDSTORE_H
