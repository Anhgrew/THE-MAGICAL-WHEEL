#include "Server.h"
#include "FileReader.h"
#include "Keyword.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

using namespace std;
#define N 2
fd_set fr, fw, fe;
bool full = false;

SOCKET socket_receiver = INVALID_SOCKET;

int clients[N+1];
int current_appearances = 0;

void ProcessNewMessage(int nClientSocket) {
	cout << "Process the new message for client socket: " << nClientSocket << endl;
	char buffer[256] = { 0, };
	int relIn = recv(nClientSocket, buffer, 256, 0);
	if (relIn < 0) {
		current_appearances--;
		// When number of socket == 0 reset server 
		if(current_appearances == 0) full = false;
		cout << "Failed to process new message something went wrong" << endl;
		closesocket(nClientSocket);
		for (int i = 0; i < N; i++) {
			if (clients[i] == nClientSocket) {
				clients[i] = 0;
				break;
			}
		}
	}
	else {
		std::cout << "Message recieved from client " << buffer << endl;
		send(nClientSocket, "Proceed your request, server send back", 39, 0);
		cout << endl << "**********************************";
	}


}


void ProcessNewRequest() {	
	if (FD_ISSET(socket_receiver, &fr)) {
		current_appearances++;
		if (!full && current_appearances == N) {
			full = true;
			int len = sizeof(struct sockaddr);
			int clientSocket = accept(socket_receiver, NULL, &len); // return ID client process
			if (clientSocket > 0) {
				for (int i = 0; i < N; i++)
				{
					if (clients[i] == 0) {
						clients[i] = clientSocket;
						cout << endl;
						cout << " -------> " << clientSocket << endl;
						send(clientSocket, "Got connected !!!\n", 18, 0);
						send(clientSocket, "++++++++\ Let 's start /++++++++\n", 32, 0);
						break;
					}
					else {
						send(clients[i], "++++++++\ Let 's start /++++++++\n", 32, 0);
					}
				}

			}

		}
		else if(current_appearances < N) {
			int len = sizeof(struct sockaddr);
			int clientSocket = accept(socket_receiver, NULL, &len); // return ID client process
			if (clientSocket > 0) {
				for (int i = 0; i < N; i++)
				{
					if (clients[i] == 0) {
						clients[i] = clientSocket;
						cout << " -------> " << clientSocket << endl;
						send(clientSocket, "Got connected !!!\n", 18, 0);
						break;
					}
				}

			}
		}
		else {
			// greater than N socket, accept, send mess and ignore
			current_appearances--;
			int len = sizeof(struct sockaddr);
			int clientSocket = accept(socket_receiver, NULL, &len);
			send(clientSocket, "full", 5, 0);
			closesocket(clientSocket);
			cout << "Users limit exceed, So we rejected socket: "<< clientSocket << endl;
		}


	}	  
	else if (FD_ISSET(socket_receiver, &fe)) {
		cout << "Exception !" << endl;
	}
	else if (FD_ISSET(socket_receiver, &fw)) {
		cout << "Ready to write !" << endl;
	}
	
	for (int i = 0; i < N; i++)
	{
		if (FD_ISSET(clients[i], &fr)) {
			ProcessNewMessage(clients[i]);
		}
	}
	
}

int main()
{			
	// Create keyword for game turn
	srand((int)time(0));
	FileReader* file = new FileReader("database.txt", ios::in);
	vector<Keyword*> keyword_list = file->getListKeyWord();
	file->~FileReader();  

	Keyword* selected_keyword = keyword_list[rand() % keyword_list.size()];
	std::cout << selected_keyword->keyword << " + " << selected_keyword->description << std::endl;

	//Create new name of connected user => must change to send mess to user
	string name = "Anhgrew111";
	FileReader* user_file = new FileReader("users.txt", ios::in);
	string registration_message = user_file->registrationOrLogin(name);
	cout << registration_message << endl;
	
	
	
	WSADATA ws;

	// DWORD thread;	//Kết quả trả về của thread

	 //Socket dùng để lắng nghe từ client

	SOCKET socket_notification = INVALID_SOCKET;		//socket để gửi notification
	
	struct sockaddr_in receiver, notifier; //Địa chỉ của server

	int res;

	// Initialize Winsock

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		cout << endl << "The WSA failed !!!";
		exit(EXIT_FAILURE);
	}
	else {
		cout << endl << "The WSA init successfully" << endl;
	}

	memset(&receiver, 0, sizeof(receiver));		//Initialize sockaddr
	receiver.sin_family = AF_INET;			//Server IP's type = IPv4
	receiver.sin_addr.s_addr = INADDR_ANY;	//Can connect to any interface
	receiver.sin_port = htons(9090);			//The port number is 9090

	memset(&notifier, 0, sizeof(notifier));		//Initialize sockaddr
	notifier.sin_family = AF_INET;			//Server IP's type = IPv4
	notifier.sin_addr.s_addr = INADDR_ANY;	//Can connect to any interface
	notifier.sin_port = htons(9091);			//The port number is 9091

	//Create the server's socket
	socket_receiver = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_receiver == INVALID_SOCKET)
	{
		printf("Can't create the server's receiver socket!\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	//Create the server's socket
	socket_notification = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_notification == INVALID_SOCKET)
	{
		printf("Can't create the server's notification socket!\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	// About the blocking vs non blocking socket
	// optval = 0 mean blocking =0 mean non blocking

	u_long optval = 0;
	res = ioctlsocket(socket_receiver, FIONBIO, &optval);

	if (res != 0) {
		cout << "Non block failed !!!" << endl;
	}
	else {
		cout << "Non block passed !!!" << endl;
	}

	//Bind the socket to the port (port 9090)
	//printf("Bind()\n");

	int optVal = 0;
	int nLen = sizeof(optVal);

	res = setsockopt(socket_receiver, SOL_SOCKET, SO_REUSEADDR,(const char*)&nLen, nLen);

	if (res < 0)
	{
		cout << endl << "The setsockopt failed";
	}



	res = bind(socket_receiver, (const sockaddr*)&receiver, sizeof(receiver));

	if (res == SOCKET_ERROR)
	{
		printf("Can't bind the server's socket!\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	res = bind(socket_notification, (const sockaddr*)&notifier, sizeof(notifier));

	if (res == SOCKET_ERROR)
	{
		printf("Can't bind the server's socket!\n");
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	//Listen for the connection from clients

	res = listen(socket_receiver, N);

	if (res < 0) {
		cout << endl << "The listening failed to local port !!!";
		exit(EXIT_FAILURE);
	}
	else {
		cout << endl << "Listening successfully to the local port" << endl;
	}

	res = listen(socket_notification, N);

	if (res < 0) {
		cout << endl << "The listening failed to local port !!!";
		exit(EXIT_FAILURE);
	}
	else {
		cout << endl << "Listening successfully to the local port" << endl;
	}
	
	struct timeval tv;

	tv.tv_sec = 3;
	tv.tv_usec = 0;

	int nMaxFd = socket_receiver + 1;
	
	while (1) {
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(socket_receiver, &fr);

		FD_SET(socket_receiver, &fe);

		for (int i = 0; i < N; i++)
		{
			if (clients[i] != 0) {
				FD_SET(clients[i], &fr);
				FD_SET(clients[i], &fe);
			}
		}

		cout << endl << "Before: " << fr.fd_count;

		// Keep waiting for new req and pro as per req



		res = select(nMaxFd + 1, &fr, &fw, &fe, &tv);

		if (res > 0 && current_appearances <= N) {
			cout << "Success >><<" << endl;
			ProcessNewRequest();
		}
		else if (res == 0) {
			cout << " Nothing on port !!!" << endl;
		}
		else {
			cout << "Errors or user limit exceed" << endl;
			getchar();
			cout << "Press any key to exit" << endl;
			exit(EXIT_FAILURE);
		}
		cout << endl << "After: " << fr.fd_count << endl;
	}
	   

	//Close socket
	closesocket(socket_receiver);
	closesocket(socket_notification);
	WSACleanup();
	return 0;
}
