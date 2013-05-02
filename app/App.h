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
             *
             * If insist is true, the component will try and try to connect
             * to the server until it can reach it
             */
            void runStandalone(bool insist = true);

            /**
             * Stops
             */
            void stop();

            ServerHub hub;
        protected:
            ConfigFile *config;

            Server *server;
            RemoteClient *client;
    };
}

#endif // _RHOBAN_APP_H
