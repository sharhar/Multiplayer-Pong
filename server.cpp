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
sf::Clock CC;

int p1y = 100;
int p2y = 100;
int ballX = 300;
int ballY = 300;
int p1s = 3;
int p2s = 3;
int p1o = 0;
int p2o = 0;

int bsx = 4;
int bsy = 4;

void sleep(float sec) {
	sf::sleep(sf::seconds(sec));
}

int abs(int number) {
	if(number < 0) {
		return -number;
	}

	return number;
}

bool isHitting(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	if(x1 + w1 < x2 || x2 + w2 < x1) {
		return false;
	}
	if(y1 + h1 < y2 || y2 + h2 < y1) {
		return false;
	}

	return true;
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

void game() {
	CC.restart();

	bool started = false;

	std::cout << "String game loop!\n";

	while(running) {
		if(!started) {
			if(CC.getElapsedTime().asSeconds() > 3.0f) {
				started = true;
				p1s = 0;
				p2s = 0;
			}else if(CC.getElapsedTime().asSeconds() > 2.0f) {
				p1s = 1;
				p2s = 1;
			}else if(CC.getElapsedTime().asSeconds() > 1.0f) {
				p1s = 2;
				p2s = 2;
			}
		} else {
			ballX += bsx;
			ballY += bsy;

			if(ballY+20 > 480 || ballY < 0) {
				bsy = -bsy;
			}

			if(isHitting(100, p1y, 15, 100, ballX, ballY, 20, 20)) {
				bsx = abs(bsx);
			}
			if(isHitting(500, p2y, 15, 100, ballX, ballY, 20, 20)) {
				bsx = -abs(bsx);	
			}

			if(ballX < 0) {
				p1s++;
				ballX = 300;
			}
			if(ballX+20 > 640) {
				p2s++;
				ballX = 300;
			}
		}

		sleep(0.02f);
	}
}

int main() {
	sf::TcpListener listener;

	std::cout << "Enter port: ";
	int port = 0;
	std::cin >> port;

	if(listener.listen(port) != sf::Socket::Done) {
		std::cout << "Error when trying to bind to port " << port << "!" << std::endl;
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

	sf::Thread loop(&game);

	thread2.launch();
	thread1.launch();
	loop.launch();

	std::cout << "starting main game loop!" << std::endl;

	while(running) {
		//std::cout << "p1 " << p1y << "\tp2 " << p2y << std::endl;
		std::string message = "U " + to_string(p1y) + " " + to_string(p2y) + " " + to_string(ballX) + " " + to_string(ballY) + " " + to_string(p1s) + " " + to_string(p2s);
		const char* cMessage = message.c_str();

		if(clientSocket1.send(cMessage, 100) != sf::Socket::Done) {
			std::cout << "Could not send data!" << std::endl;
		}
		if(clientSocket2.send(cMessage, 100) != sf::Socket::Done) {
			std::cout << "Could not send data!" << std::endl;
		}

		sleep(0.02f);
	}

	return 0;
}