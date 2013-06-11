#include <timing/sleep.h>
#include "App.h"

namespace Rhoban
{
    /**
     * Creating an app
     */
    App::App(ConfigFile &config_) : server(NULL), client(NULL)
    {
        config = &config_;
    }

    App::App() : server(NULL), client(NULL), config(NULL)
    {
    }
    
    /**
     * Register a component for the application
     */
    void App::registerComponent(ServerComponent *component)
    {
        hub.registerComponent(component);

        if (config) {
            component->loadConfig(*config);
        }
    }

    /**
     * Runs the server
     */
    void App::runServer()
    {
        string name;
        int port;

        config->read("server", "port", 7777, port);
        config->read("robot", "name", "Rhoban Robot", name);
        config->help();

        try {
            server = new Server(&hub);
            server->setName(name);
            server->run(port);
        } catch (string err) {
            cout << "App error: " << err << endl;
        }

        stop();
    }

    /**
     * Runs as a standalone client
     */
    void App::runStandalone(bool insist)
    {
        string host;
        int port;
        config->read("client", "host", "127.0.0.1", host);
        config->read("client", "port", 7777, port);
        config->help();

        while (insist) {
            cout << "App: running standalone client" << endl;
            try {
                client = new RemoteClient(&hub);
                client->connectTo(host.c_str(), port, false);
                client->registerComponents();
                client->execute();
            } catch (string err) {
                cout << "App error: " << err << endl;
            }
            sleep_ms(500);

            stop();
        }
    }

    /**
     * Stops
     */
    void App::stop()
    {
        if (server) {
            server->shutdown();
            delete server;
            server = NULL;
        }

        if (client) {
            client->stop();
            delete client;
            client = NULL;
        }
    }
}
