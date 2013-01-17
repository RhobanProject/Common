#ifndef _RHOBAN_APP_H
#define _RHOBAN_APP_H

#include <configfile/ConfigFile.h>
#include <communication/Server.h>
#include <communication/ServerComponent.h>
#include <communication/RemoteClient.h>

using namespace std;

/**
 * A Rhoban App is an application (server or standalone component)
 */
namespace Rhoban
{
    class App
    {
        public:
            App(ConfigFile &config_);
            
            /**
             * Register a component for the application
             */
            void registerComponent(ServerComponent *component);

            /**
             * Runs the server
             */
            void runServer();

            /**
             * Runs as a standalone client
             */
            void runStandalone();

            /**
             * Stops
             */
            void stop();
            
        protected:
            ServerHub hub;
            ConfigFile *config;

            Server *server;
            RemoteClient *client;
    };
}

#endif // _RHOBAN_APP_H
