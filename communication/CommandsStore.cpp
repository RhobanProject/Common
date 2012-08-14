/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <vector>

#include <communication/MessageBuilder.h>

#include "CommandsStore.h"

using namespace std;

namespace Rhoban
{
  CommandsStore::CommandsStore()
  {
    builder=new MessageBuilder;
  }

  CommandsStore::~CommandsStore()
  {
    delete(builder);
  }

  MessageBuilder *CommandsStore::getBuilder()
  {
    return builder;
  }
}
