#include "NetworkManager.h"
#include <DynamicOutput/DynamicOutput.hpp>

using namespace RC;

namespace ArrND::Core::Networking {
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

	void NetworkManager::SendMessage(void* data, GameMessage gMessage, bool isReliable)
	{
		if (gMessage == GameMessage::MOVE) {
			ENetPacket* p = enet_packet_create(&data, sizeof(GameMessage), ENET_PACKET_FLAG_RELIABLE);
			this->SendMovementMessage(p);
		}

		ENetPacket* p = enet_packet_create(&data, sizeof(GameMessage), ENET_PACKET_FLAG_RELIABLE);
		//enet_peer_send(this->communicationPeer, 0, p);

	}

	void NetworkManager::SendMovementMessage(ENetPacket* packket)
	{

	}

	void NetworkManager::SendGameMessage(ENetPacket* p, GameMessage gMessage, bool isReliable)
	{
	}

	bool NetworkManager::InitClient() {
		this->clientHost = enet_host_create(NULL, 1, 2, 0, 0);

		if (this->clientHost != NULL) {
			return true;
			Output::send<LogLevel::Verbose>(STR("ENet client host initialized.\n"));
		}

		return false;
	}

	void NetworkManager::ConnectToServer()
	{

		if (this->InitClient()) {
			ENetAddress address;
			ENetEvent event;
			ENetPeer* communicationPeer;

			enet_address_set_host(&address, "localhost");
			address.port = 9500;

			communicationPeer = enet_host_connect(this->clientHost, &address, 2, 0);

			if (communicationPeer == NULL) {
				Output::send<LogLevel::Verbose>(STR("No available peers for initiating an ENet connection.\n"));
			}

			if (enet_host_service(this->clientHost, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
				Output::send<LogLevel::Verbose>(STR("Connection to localhost:8080 succeeded.\n"));
				this->isClientInitialized = true;
				//this->OnUpdate(this->client);
			} else {
				enet_peer_reset(communicationPeer);
				Output::send<LogLevel::Error>(STR("Connection to localhost:8080 failed.\n"));
			}
		}
		
	}

	bool NetworkManager::InitEnet()
	{
		if (enet_initialize() == 0) {
			Output::send<LogLevel::Verbose>(STR("ENet initialized.\n"));
			return true;
		}
		
		return false;
	}
}