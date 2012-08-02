/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <main/Command.h>
#include <tests/runTests.h>
#include "TestEncoding.h"
#include "TestMessage.h"

COMMAND_DEFINE(communication_tests, "Run the Rhoban communication tests")
{
    runTests<TestEncoding>("Tests d'encodage");
    runTests<TestMessage>("Tests des messages");
}
