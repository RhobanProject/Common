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
