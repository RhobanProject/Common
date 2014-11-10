#include <timing/sleep.h>
#include "App.h"

namespace Rhoban
{
	/**
	 * Creating an app
	 */
	App::App(ConfigFile &config_) : client(NULL)
	{
		config = &config_;
	}

	App::App() : server(NULL), client(NULL), config(NULL), zmq_server(NULL)
	{
	}

	/**
	 * Register a component for the application
	 */
	void App::registerComponent(ui16 type, Callable *component)
	{
		hub.registerComponent(type, component);
		if (config) {
			ServerComponent * c = dynamic_cast<ServerComponent *>(component);
			if (c)
				c->loadConfig(*config);
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

			zmq_server = new Server2(&hub);
			zmq_server->run(port + 1, name);

			server = new Server(&hub);
			server->setName(name);
			server->run(port);

			zmq_server->wait();

		}
		catch (const std::runtime_error err) {
			cout << "App error: " << err.what() << endl;
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
            } catch (const std::runtime_error & err) {
                cout << "App error: " << err.what() << endl;
            }
            ms_sleep(500);

            stop();
        }
    }

    /**
     * Stops
     */
    void App::stop()
    {
		zmq_server->shutdown();

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
