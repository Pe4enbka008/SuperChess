/*
This file servers as an example of how to use Pipe.h file.
It is recommended to use the following code in your project, 
in order to read and write information from and to the Backend
*/

#include "Pipe.h"
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <chrono>

using std::cout;
using std::endl;
using std::string;



#include "Game.h"

void main()
{
	Game game;
	game.main_loop();
	game.~Game();
} // main



void example()
{
	// Setting-Up:
	srand(time_t(NULL));

	Pipe pipe_socket;
	bool isConnect = pipe_socket.connect();

	string ans;
	while (!isConnect)  // RECONNECT LOOP
	{
		cout << "cant connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		std::cin >> ans;

		if (ans == "0")
		{
			cout << "trying connect again.." << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			isConnect = pipe_socket.connect();
		} // if
		else
		{
			pipe_socket.close();
			return;
		} // else
	} // while


	char msgToGraphics[1024];
	// msgToGraphics should contain the board string accord the protocol
	// YOUR CODE

	strcpy_s(msgToGraphics, "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0"); // just example...
	pipe_socket.sendMessageToGraphics(msgToGraphics);   // send the board string


	string msgFromGraphics = pipe_socket.getMessageFromGraphics();	// get message from graphics
	while (msgFromGraphics != "quit") // main loop
	{


		cout << msgFromGraphics << endl;
		string coords_from = msgFromGraphics.substr(0, 2); // from, length
		string coords_to = msgFromGraphics.substr(2, 2); // from, length


		/******* JUST FOR EREZ DEBUGGING ******/
		int r = rand() % 10; // just for debugging......
		msgToGraphics[0] = (char)(1 + '0');
		msgToGraphics[1] = 0;
		/******* JUST FOR EREZ DEBUGGING ******/



		strcpy_s(msgToGraphics, "YOUR CODE"); // msgToGraphics should contain the result of the operation
		pipe_socket.sendMessageToGraphics(msgToGraphics);   	// return result to graphics		
		msgFromGraphics = pipe_socket.getMessageFromGraphics();	// get message from graphics
	} // while

	pipe_socket.close();
} // example

