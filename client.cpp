//#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "String.h"
#include <iostream>


template < typename T > std::string to_string( const T& n ) {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

sf::TcpSocket socket;
bool running = true;
int y = 100;
int player1Y = 100;
int player2Y = 100;
int ballX = 300;
int ballY = 300;
int p1Score = 0;
int p2Score = 0;
int num = 0;
bool send = false;


void sleep(float sec) {
	sf::sleep(sf::seconds(sec));
}

void game() {
	sf::RenderWindow window(sf::VideoMode(640, 480), ("Player " + to_string(num)), sf::Style::Default, sf::ContextSettings());
	window.setVerticalSyncEnabled(true);

	glClearColor(0.1f, 0.1f, 0.1f, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	bool focused = false;

	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return;		
	}

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setPosition(300, 30);

	while (running) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
                socket.disconnect();
            } else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            } else if (event.type == sf::Event::LostFocus) {
                focused = false;
            } else if (event.type == sf::Event::GainedFocus) {
                focused = true;
            } 
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_QUADS);
        {
        	glVertex2f(100, player1Y    );
        	glVertex2f(100, player1Y+100);
        	glVertex2f(115, player1Y+100);
        	glVertex2f(115, player1Y    );

        	glVertex2f(500, player2Y    );
        	glVertex2f(500, player2Y+100);
        	glVertex2f(515, player2Y+100);
        	glVertex2f(515, player2Y    );

			glVertex2f(ballX     , ballY   );
        	glVertex2f(ballX     , ballY+20);
        	glVertex2f(ballX + 20, ballY+20);
        	glVertex2f(ballX + 20, ballY   );
        }
        glEnd();

        int speed = 5;

        if(focused) {
        	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
	        	send = true;
	        	y += speed;
	        }
	        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
	        	send = true;
	        	y -= speed;
	        }
        }


		text.setString(to_string(p1Score) + " | " + to_string(p2Score));
		
		window.pushGLStates();
        window.draw(text);
		window.popGLStates();
        
        window.display();
    }
}

void listen() {
	while(running) {
		char data[100];
		std::size_t received;
		
		if (socket.receive(data, 100, received) != sf::Socket::Done) {
			running = false;
    		std::cout << "Could not recive data!" << std::endl;
		}

		std::string str(data);
		std::vector<std::string> parts = StringUtils::split(str, ' ');
		if(parts[0].compare(std::string("N")) == 0) {
			int number = StringUtils::parseInt(parts[1]);
			std::cout << "Number is: " << number << std::endl;
			num = number;
		} else if(parts[0].compare(std::string("U")) == 0) {
			int number1 = StringUtils::parseInt(parts[1]);
			int number2 = StringUtils::parseInt(parts[2]);
			int number3 = StringUtils::parseInt(parts[3]);
			int number4 = StringUtils::parseInt(parts[4]);
			int number5 = StringUtils::parseInt(parts[5]);
			int number6 = StringUtils::parseInt(parts[6]);
			player1Y = number1;
			player2Y = number2;
			ballX    = number3;
			ballY    = number4;
			p1Score  = number5;
			p2Score  = number6;
		}
	}
}

int main() {
	sf::Thread loop(&game);

	std::cout << "Enter IP: ";
	std::string address = "";
	std::cin >> address;

	std::cout << "Enter port: ";
	int port = 0;
	std::cin >> port;

	sf::Socket::Status status = socket.connect(address, port);
	
	if (status != sf::Socket::Done) {
    	std::cout << "Could not connect to server!" << std::endl;
    	return -1;
    }

	sf::Thread lis(&listen);
	
	char data[100];
	std::size_t received;
	
	if (socket.receive(data, 100, received) != sf::Socket::Done) {
		running = false;
   		std::cout << "Could not recive data!" << std::endl;
	}

	std::string str(data);
	std::vector<std::string> parts = StringUtils::split(str, ' ');
	
	int number = StringUtils::parseInt(parts[1]);
	std::cout << "Number is: " << number << std::endl;
	num = number;

	lis.launch();
	loop.launch();

	while(running) {
		if(send) {
			std::string message = "P " + to_string(y);

			if (socket.send(message.c_str(), 100) != sf::Socket::Done) {

			}
		}

		sleep(0.02f);
	}
	return 0;
}