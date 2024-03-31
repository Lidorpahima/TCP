#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
using namespace std;

map<string, int> sockets; // Map to store client names and their respective sockets
pthread_mutex_t sockets_mutex = PTHREAD_MUTEX_INITIALIZER; // Will use later to lock 

void *client_handler(void *socket_desc) { // Function to handle client connections
    int sock = *(int *)socket_desc; // Get the socket descriptor
    char client_message[2000]; //initialize a buffer to store messages
    string client_name; //store the client name

    // Get client name
    if (recv(sock, client_message, 2000, 0) > 0) { // Receive client name
        client_name = string(client_message).substr(0, string(client_message).find("\n")); 
        pthread_mutex_lock(&sockets_mutex); // Lock the mutex
        sockets[client_name] = sock; // Add the client to the map
        pthread_mutex_unlock(&sockets_mutex); // Unlock the mutex
        cout << client_name << " connected." << endl; // Print message to console to indicate client has connected
    }

    while (true) {// Loop to keep the connection open
        memset(client_message, 0, 2000); // Clear the message buffer
        if (recv(sock, client_message, 2000, 0) <= 0) {// Receive message from client
            cout << client_name << " disconnected." << endl;//let the server know that the client has disconnected
            break;
        }

        string message(client_message);
        if (message.find("exit") != string::npos) {
            cout << "Exit received from " << client_name << endl;//let the server know that the client has disconnected
            break;
        }

        size_t pos = message.find(" "); //will use to find the recipient
        if (pos != string::npos) { // If there is a space in the message
            string recipient = message.substr(0, pos);//so the recipient is the first word
            string msg = message.substr(pos + 1);//and the message is the rest

            pthread_mutex_lock(&sockets_mutex);//lock the mutex
            if (sockets.find(recipient) != sockets.end()) {//if the recipient is in the map
                send(sockets[recipient], msg.c_str(), msg.length(), 0);//send the message to the recipient
                cout << "Message from " << client_name << " to " << recipient << ": " << msg << endl;//print the message to the console that the message was sent
            } else {
                cout << "Recipient " << recipient << " not found." << endl;//print to the console that the recipient was not found
            }
            pthread_mutex_unlock(&sockets_mutex);//unlock the mutex
        }
    }

    
    close(sock); // Close the socket
    pthread_mutex_lock(&sockets_mutex);//lock the mutex
    sockets.erase(client_name);//remove the client from the map
    pthread_mutex_unlock(&sockets_mutex);//unlock the mutex
    delete (int *)socket_desc;//delete the socket descriptor

    return 0;
}

int main(int argc, char *argv[]) {
    int server_sock, client_sock, *new_sock; // Initialize server and client sockets
    struct sockaddr_in server, client;// Initialize server and client addresses
    pthread_t thread_id; // Initialize thread id

    if (argc != 2) { //check if the user has entered the correct number of arguments
        cerr << "Usage: " << argv[0] << " <port>" << endl; // Print usage message to console
        return 1; // return error code
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0); // Create new socket
    if (server_sock == -1) {//check if the socket was created successfully
        cerr << "Could not create socket" << endl;//print error message to console
        return 1;
    }

    server.sin_family = AF_INET;//set the address family
    server.sin_addr.s_addr = INADDR_ANY;//set the address to any address
    server.sin_port = htons(atoi(argv[1]));//set the port number

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {//will bind the socket to the address
        perror("bind failed");//print error message to console
        return 1;
    }

    listen(server_sock, 4);//listen for connections max 4 clients

    cout << "Waiting for connections" << endl; // Print message to console to indicate server is running
    int c = sizeof(struct sockaddr_in);//get the size of the address
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t *)&c))) {//accept the connection
        cout << "Connection accepted" << endl;//indicate that the connection was accepted

        new_sock = new int;//create a new socket
        *new_sock = client_sock;//set the new socket to the client socket

        if (pthread_create(&thread_id, NULL, client_handler, (void *)new_sock) < 0) {//create a new thread to handle client and check if it was created successfully
            perror("could not create thread");//print error message to console
            return 1;
        }
    }

    if (client_sock < 0) {//check if the client socket was created successfully
        perror("accept failed");//print error message to console
        return 1;
    }

    return 0;
}
