#include "NetworkManager.h"
#include <DynamicOutput/DynamicOutput.hpp>

using namespace RC;

namespace ArrND::Core::NetworkManager {
	NetworkManager::NetworkManager() {
		//do nothing
		Output::send<LogLevel::Verbose>(STR("NetworkManager initialized"));
	}

	NetworkManager::~NetworkManager() {
		//do nothing
	}

	void NetworkManager::OnUpdate(ENetHost* client)
	{
		ENetEvent event;

		while (enet_host_service(client, &event, 0))
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				exit(0);
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				//event.peer->data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
			}
		}
	}

	void NetworkManager::Run()
	{
		//do nothing
		if (this->InitEnet()) {
			this->ConnectToServer();
		}
	}

	bool NetworkManager::InitClient() {
		this->client = enet_host_create(NULL /* create a client host */,
			1 /* only allow 1 outgoing connection */,
			2 /* allow up 2 channels to be used, 0 and 1 */,
			0 /* assume any amount of incoming bandwidth */,
			0 /* assume any amount of outgoing bandwidth */);
		if (client == NULL) {
			Output::send<LogLevel::Error>(STR("An error occurred while trying to create an ENet client host.\n"));
			return false;
		}
		else {
			return true;
		}

		return false;
	}

	bool NetworkManager::ConnectToServer()
	{

		if (this->InitClient()) {
			ENetAddress address;
			ENetEvent event;
			ENetPeer* peer;

			enet_address_set_host(&address, "localhost");
			address.port = 9500;

			peer = enet_host_connect(this->client, &address, 2, 0);
			if (peer == NULL) {
				Output::send<LogLevel::Verbose>(STR("No available peers for initiating an ENet connection.\n"));
				return false;
			}
			/* Wait up to 5 seconds for the connection attempt to succeed. */
			if (enet_host_service(this->client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
				Output::send<LogLevel::Verbose>(STR("Connection to localhost:8080 succeeded.\n"));
				//return true;
				this->OnUpdate(this->client);
				return true;
			}
			else {
				/* Either the 5 seconds are up or a disconnect event was */
				/* received. Reset the peer in the event the 5 seconds   */
				/* had run out without any significant event.            */
				enet_peer_reset(peer);
				Output::send<LogLevel::Error>(STR("Connection to localhost:8080 failed.\n"));
			}

			return false;
		}
		else {
			return false;
		}

		return false;
		
	}

	bool NetworkManager::InitEnet()
	{
		if (enet_initialize() != 0) {
			fprintf(stderr, "An error occurred while initializing ENet.\n");
			return false;
		}
		else {
			printf("ENet initialized.\n");
			return true;
		}

		return false;
	}
}