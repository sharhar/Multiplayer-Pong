#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "String.h"

template < typename T > std::string to_string( const T& n ) {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

bool running = true;
sf::TcpSocket clientSocket1;
sf::TcpSocket clientSocket2;

int p1y = 0;
int p2y = 0;

void sleep(float sec) {
	sf::sleep(sf::seconds(sec));
}

void client1() {
	while(running) {
		char data[100];
		std::size_t received;
		
		if (clientSocket1.receive(data, 100, received) != sf::Socket::Done) {
    		std::cout << "Could not recive data!" << std::endl;
		}

		std::string str(data);
		std::vector<std::string> parts = StringUtils::split(str, ' ');
		if(parts[0].compare(std::string("P")) == 0) {
			int number = StringUtils::parseInt(parts[1]);
			p1y = number;
		}
	}
}


void client2() {
	while(running) {
		char data[100];
		std::size_t received;
		
		if (clientSocket2.receive(data, 100, received) != sf::Socket::Done) {
    		std::cout << "Could not recive data!" << std::endl;
		}

		std::string str(data);
		std::vector<std::string> parts = StringUtils::split(str, ' ');
		if(parts[0].compare(std::string("P")) == 0) {
			int number = StringUtils::parseInt(parts[1]);
			p2y = number;
		}
	}
}

int main() {
	sf::TcpListener listener;

	if(listener.listen(25565) != sf::Socket::Done) {
		std::cout << "Error when trying to bind to port 25565!" << std::endl;
    	return -1;
	}

	std::cout << "Waiting for people to connect!" << std::endl;
	
	if(listener.accept(clientSocket1) != sf::Socket::Done) {
		std::cout << "Error when trying to connect to client!" << std::endl;
    	return -1;
	}

	sf::Thread thread1(&client1);
	if(clientSocket1.send("N 1", 100) != sf::Socket::Done) {
		std::cout << "Could not send data!" << std::endl;
	}

	std::cout << "Player 1 connected!" << std::endl;

	if(listener.accept(clientSocket2) != sf::Socket::Done) {
		std::cout << "Error when trying to connect to client!" << std::endl;
 	  	return -1;
	}

	sf::Thread thread2(&client2);
	if(clientSocket2.send("N 2", 100) != sf::Socket::Done) {
		std::cout << "Could not send data!" << std::endl;
	}

	std::cout << "Player 2 connected!" << std::endl;

	thread2.launch();
	thread1.launch();

	std::cout << "starting main game loop!" << std::endl;

	while(running) {
		//std::cout << "p1 " << p1y << "\tp2 " << p2y << std::endl;

		if(clientSocket1.send(("P " + to_string(p1y) + " " + to_string(p2y)).c_str(), 100) != sf::Socket::Done) {
			std::cout << "Could not send data!" << std::endl;
		}
		if(clientSocket2.send(("P " + to_string(p1y) + " " + to_string(p2y)).c_str(), 100) != sf::Socket::Done) {
			std::cout << "Could not send data!" << std::endl;
		}

		sleep(0.02f);
	}

	return 0;
}