#pragma once

#include "constants.h"
#include "socklib.h"
#include <sstream>

class UDPClient {
public:
	UDPClient(const char* host, int port, bool include_ids = false)
	{
		/* TODO: IMPLEMENT THIS */
		Socket udp_sock(Socket::Family::INET, Socket::Type::DGRAM);
		// udp_sock.SetTimeout(wait_time);

		// That's it actually
		// Now we can send and recv :)

		std::string msg_to_send("Hello, server! How are you?");
		// Send to this computer, which "127.0.0.1" always refers to.
		Address server_addr("127.0.0.1", 10245);


		char buffer[4096];
		Address msg_from;

		// RecvFrom returns the number of bytes received
		// and creates a new address, which it stores in the
		// last parameter. This address is the address from
		// which the data arrived.
		int nbytes_recvd = -1;
		// while (wait_time < max_wait_time && nbytes_recvd == -1)
		{
			// SendTo() takes an extra parameter as compared to Send():
			// the address to which the data should be sent.
			// The same socket can send data to any address, unlike
			// with TCP, where a socket is bound a single connection.
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

		if (wait_time > max_wait_time)
		{
			std::cout << "Never received a message!! Now I really AM sad :(\n";
			return;
		}

		std::cout << "Received " << nbytes_recvd << " bytes from " << msg_from
			<< ": '";
		std::cout.write(buffer, nbytes_recvd);
		std::cout << "'\n";

	}

	int send_message_by_character(const std::string& str, std::string& result)
	{
		/* TODO: IMPLEMENT THIS */
		std::string msg_to_send(str);


		return -1;
	}
};
