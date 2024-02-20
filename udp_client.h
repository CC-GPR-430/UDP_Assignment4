#pragma once

#include "constants.h"
#include "socklib.h"
#include <sstream>

class UDPClient {
private:
	char* address;
	int port;
	bool include_ids;

public: 
	UDPClient(const char* host, int port, bool include_ids = false)
	{
		this->port = port;//
		this->address = host;//IP ex.123.56.78
		this->include_ids = include_ids;//QA_2
	}

	int send_message_by_character(const std::string& str, std::string& result)
	{
		Socket udp_sock(Socket::Family::INET, Socket::Type::DGRAM);//UDP is DGRAM
		udp_sock.SetTimeout(INITIAL_TIMEOUT);

		Address server_addr(address, port);
		Address msg_from;
		std::string msg_to_send(str);
		char buffer[1];
		int nbytes_recvd = -1;
		float wait_time = INITIAL_TIMEOUT;

		// while (wait_time < max_wait_time && nbytes_recvd == -1)
		{
			size_t nbytes_sent = udp_sock.SendTo(msg_to_send.data(),
				msg_to_send.size(),
				server_addr);

			std::cout << "Sent " << nbytes_sent << " bytes to "
				<< server_addr << "\n";

			// Problem: RecvFrom() receives data from *anyone* -- but
			// we only want data that the server sent back! How can
			// we make sure the data we got back is from the server?
			// Some options:
			//	1.	RecvFrom() tells us the address from which the message
			//		came. Can compare that with the server's address
			//		to see if the data came from the server.
			int nbytes_recvd = udp_sock.RecvFrom(buffer, sizeof(buffer), msg_from);

			// nbytes_recvd == -1 => We did not receive data for some reason
			// nbytes_recvd  > 0  => We received nbytes_recvd bytes.
			if (nbytes_recvd == -1)
			{
				// An error occurred in our receive!
				if (udp_sock.GetLastError() == Socket::Error::SOCKLIB_ETIMEDOUT)
				{
					std::cout << "Timed out. Maybe retrying...\n";
					// We timed out! Increase the timeout time and try again.
					wait_time *= 2;
					udp_sock.SetTimeout(wait_time);
				}
			}
		}

		if (wait_time > MAX_TIMEOUT)
		{
			std::cout << "Never received a message!! Now I really AM sad :(\n";
			return;
		}

		std::cout << "Received " << nbytes_recvd << " bytes from " << msg_from << ": '";
		std::cout.write(buffer, nbytes_recvd) << "'\n";

		return -1;
	}
};
