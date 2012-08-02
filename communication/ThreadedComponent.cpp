/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include "ThreadedComponent.h"

ThreadedComponent::ThreadedComponent()
{
    start(0);
    resume_thread();
};

ThreadedComponent::~ThreadedComponent()
{
    kill();
};
