/* 
Programmer: Mena Mikhail
   the code establishes a connection with the server, 
   allows users to send and receive messages, 
   and handles user join and leave notifications in a chat environment. 
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

// Including the header file that defines constants, enums, and functions for the chat program
#include "chat.h"

#define PORT 3334

// Function prototypes
void send_handler(int *);
void recv_handler(int *);

// Global variable for storing the username
char username[MAX_SENDER_NAME_LENGTH] = "";

int main() {
    int sd;
    struct sockaddr_in server_add;
    pthread_t send_thread, recv_thread;

    // Creating a socket for the client
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        printErrorStatus("socket creation");
        return -1;
    }

    // Configuring server address
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_add.sin_port = htons(PORT);

    // Connecting to the server
    if (connect(sd, (struct sockaddr *) &server_add, sizeof(server_add)) < 0) {
        printErrorStatus("socket connection");
        return -1;
    }

    // Acquiring user's username
    while (strcmp(username, "") == 0) {
        printf("Enter your username: ");
        fgets(username, MAX_SENDER_NAME_LENGTH, stdin);

        // Removing the newline character if present
        if ((strlen(username) > 0) && (username[strlen(username) - 1] == '\n'))
            username[strlen(username) - 1] = '\0';
    }

    // Sending a JOIN message to the server to notify other clients about the new user
    messageProtocol messageToSend;
    messageToSend.typeMessage = MESSAGE_TYPE_JOIN;
    strcpy(messageToSend.sender, username);
    send(sd, &messageToSend, SIZE_OF_MESSAGE_PROTOCOL, 0);

    // Printing a message to indicate that the user has joined the chat
    printUserJoin(username);

    // Creating a thread for sending messages
    if (pthread_create(&send_thread, NULL, (void *) send_handler, &sd) != 0) {
        printErrorStatus("sender thread creation");
        return -1;
    }

    // Creating a thread for receiving messages
    if (pthread_create(&recv_thread, NULL, (void *) recv_handler, &sd) != 0) {
        printErrorStatus("receiver thread creation");
        return -1;
    }

    // Waiting for the receive thread to return, which happens when the connection with the server is lost
    pthread_join(recv_thread, NULL);

    // Closing the socket and detaching threads
    close(sd);
    pthread_detach(send_thread);
    pthread_detach(recv_thread);

    return 0;
}

// Function executed by the send thread
void send_handler(int *sd_pointer) {
    int sd = *sd_pointer;
    messageProtocol messageToSend;

    // Configuring the message to be sent
    messageToSend.typeMessage = MESSAGE_TYPE_SEND;
    strcpy(messageToSend.sender, username);

    while (1) {
        // Printing the message sending interface and waiting for user input
        printSendingInterface(username);
        fgets(messageToSend.message, MAX_MESSAGE_LENGTH, stdin);

        // Removing the newline character if present
        if ((strlen(messageToSend.message) > 0) && (messageToSend.message[strlen(messageToSend.message) - 1] == '\n'))
            messageToSend.message[strlen(messageToSend.message) - 1] = '\0';

        // Sending the message to the server if it's not empty
        if (strcmp(messageToSend.message, "") != 0)
            send(sd, &messageToSend, SIZE_OF_MESSAGE_PROTOCOL, 0);
    }
}

// Function executed by the receive thread
void recv_handler(int *pointer_sd) {
    int sd = *pointer_sd;
    messageProtocol receivedMessage;

    while (1) {
        // Receiving a message from the server
        ssize_t bytercv = recv(sd, &receivedMessage, SIZE_OF_MESSAGE_PROTOCOL, 0);

        // Clearing the input interface
        clearInput();

        // Handling disconnection or errors
        if (bytercv <= 0) {
            printErrorStatus("lost connection with the server");
            return;
        }
        // Handling received messages
        else if (receivedMessage.typeMessage == MESSAGE_TYPE_SEND)
            printMessage(receivedMessage.sender, receivedMessage.message);
        else if (receivedMessage.typeMessage == MESSAGE_TYPE_JOIN)
            printUserJoin(receivedMessage.sender);
        else if (receivedMessage.typeMessage == MESSAGE_TYPE_LEAVE)
            printUserLeft(receivedMessage.sender);

        // Restoring the sending interface
        printSendingInterface(username);
        fflush(stdout);
    }
}
