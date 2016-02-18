//#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

int main() {
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("localhost", 53000);
	
	if (status != sf::Socket::Done) {
    	std::cout << "Could not connect to server!" << std::endl;
    	return -1;
    }

	char dataIn[100];
	std::size_t received;

	// TCP socket:
	if (socket.receive(dataIn, 100, received) != sf::Socket::Done)
	{
	    // error...
	}
	std::cout << "Received " << received << " bytes" << std::endl;
	std::cout << "Data is " << dataIn << std::endl;

	char data[100] = "Hello from client!";

	// TCP socket:
	if (socket.send(data, 100) != sf::Socket::Done)
	{
	    // error...
	}

	return 0;
}