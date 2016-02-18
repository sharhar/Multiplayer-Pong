#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>

bool running = true;
sf::TcpSocket clientSocket1;
sf::TcpSocket clientSocket2;

void client1() {
	while(running) {
		char data[100];
		std::size_t received;
		
		if (clientSocket1.receive(data, 100, received) != sf::Socket::Done) {
    		std::cout << "Could not recive data!" << std::endl;
		}

		std::cout << "Received '" << data << "' from client 1" << std::endl;
	}
}


void client2() {
	while(running) {
		char data[100];
		std::size_t received;
		
		if (clientSocket2.receive(data, 100, received) != sf::Socket::Done) {
    		std::cout << "Could not recive data!" << std::endl;
		}

		std::cout << "Received '" << data << "' from client 2" << std::endl;
	}
}

int main() {
	sf::TcpListener listener;

	if(listener.listen(53000) != sf::Socket::Done) {
		std::cout << "Error when trying to bind to port 53000!" << std::endl;
    	return -1;
	}
	
	if(listener.accept(clientSocket1) != sf::Socket::Done) {
		std::cout << "Error when trying to connect to client!" << std::endl;
    	return -1;
	}

	sf::Thread thread1(&client1);
	if(clientSocket1.send("RC-1", 100) != sf::Socket::Done) {
		std::cout << "Could not send data!" << std::endl;
	}

	if(listener.accept(clientSocket2) != sf::Socket::Done) {
		std::cout << "Error when trying to connect to client!" << std::endl;
 	  	return -1;
	}

	sf::Thread thread2(&client2);
	if(clientSocket2.send("RC-2", 100) != sf::Socket::Done) {
		std::cout << "Could not send data!" << std::endl;
	}

	thread1.launch();
	thread2.launch();

	while(running) {

	}

	return 0;
}