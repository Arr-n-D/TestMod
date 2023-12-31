#include <DynamicOutput/DynamicOutput.hpp>
#include "../GameMessage/network_enums.h"
#include "NetworkManager.h"


namespace ArrND::Core::Networking {
	NetworkManager::NetworkManager(AActor* Player) {
		Output::send<LogLevel::Verbose>(STR("NetworkManager initialized"));
		this->Player = Player;
	}

	NetworkManager::~NetworkManager() {
		//do nothing
	}

	void NetworkManager::OnUpdate()
	{
		ENetEvent event;

		while (enet_host_service(this->clientHost, &event, 0))
		{
			switch (event.type)
			{
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
				this->isCommunicationEstablished = false;
				Output::send<LogLevel::Verbose>(STR("Disconnection succeeded\n"));
				/* Reset the peer's client information. */
				event.peer->data = NULL;
			}
		}
	}

	void NetworkManager::Run()
	{
		if (this->InitEnet()) {
			this->ConnectToServer();
		}
	}

	void NetworkManager::SendGameMessage(const char* data, size_t sizeOfMessage, GameMessage gMessage, bool isReliable)
	{
		Output::send<LogLevel::Verbose>(STR("Sending message to server with size supposedly {}\n"), sizeOfMessage);
		
		ENetPacket* p;
		if (gMessage == GameMessage::MOVE) {
			p = enet_packet_create(data, sizeOfMessage, ENET_PACKET_FLAG_UNSEQUENCED);
			this->SendMovementMessage(p);
		}

		if (isReliable) {
			 p = enet_packet_create(data, sizeOfMessage, ENET_PACKET_FLAG_RELIABLE);
		}
		else {
			 p = enet_packet_create(data, sizeOfMessage, ENET_PACKET_FLAG_UNSEQUENCED);
		}

		this->SendGameMessage(p, gMessage, isReliable);		

	}

	void NetworkManager::SendMovementMessage(ENetPacket* packet)
	{
		enet_peer_send(this->communicationPeer, Channels::MOVEMENT, packet);
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
			//ENetPeer* communicationPeer;

			enet_address_set_host(&address, "localhost");
			address.port = 9500;

			this->communicationPeer = enet_host_connect(this->clientHost, &address, 2, 0);

			if (this->communicationPeer == NULL) {
				Output::send<LogLevel::Verbose>(STR("No available peers for initiating an ENet connection.\n"));
			}

			if (enet_host_service(this->clientHost, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
				Output::send<LogLevel::Verbose>(STR("Connection to localhost:8080 succeeded.\n"));
				this->isCommunicationEstablished = true;
				//this->OnUpdate(this->client);
			} else {
				enet_peer_reset(this->communicationPeer);
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