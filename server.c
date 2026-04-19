/*
Programmer: Mena Mikhail
    code implements a basic client-server chat program using sockets and pthreads.
    Code sets up a basic chat server that allows multiple clients to connect,
    send messages, and receive messages from other connected clients.
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

#include "chat.h"

#define PORT 3334
#define LIMIT 10

// Structure defining the data of a single client
typedef struct {
    int sockfd;                 // Socket descriptor for the client
    struct sockaddr_in indirizzo;   // Address information of the client
} client_t;

// Function prototypes
void handleClient(void *);
void forward(messageProtocol, int);

// Global variables for managing the client queue
client_t *clients[LIMIT];          // Array of client structures
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for thread synchronization

// Function to add a client to the queue
void addQueue(client_t *newClient) {
    pthread_mutex_lock(&clients_mutex);   // Locking to prevent concurrent access
    for (int i = 0; i < LIMIT; i++) {
        if (clients[i] == NULL) {
            clients[i] = newClient;     // Adding the new client to the queue
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex); // Unlocking
}

// Function to remove a client from the queue
void removeQueue(int sd) {
    pthread_mutex_lock(&clients_mutex);   // Locking
    for (int i = 0; i < LIMIT; i++) {
        if (clients[i] != NULL && clients[i]->sockfd == sd) {
            clients[i] = NULL;      // Removing the client from the queue
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex); // Unlocking
}

int main() {
    int sd;
    struct sockaddr_in server_add, client_add;
    socklen_t sizeCli = sizeof(client_add);
    pthread_t tid;

    // Creating a socket for the server
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        printErrorStatus("socket creation");
        return -1;
    }
    printOKStatus("socket creation");

    // Configuring server address
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_add.sin_port = htons(PORT);

    // Binding the socket to the server address
    if (bind(sd, (struct sockaddr *) &server_add, sizeof(server_add)) < 0) {
        printErrorStatus("binding");
        return -1;
    }
    printOKStatus("binding");

    // Listening for incoming connections
    if (listen(sd, LIMIT) < 0) {
        printErrorStatus("listening");
        return -1;
    }
    printOKStatus("listening");

    while (1) {
        // Accepting connection and retrieving client socket descriptor
        int clientdescriptor = accept(sd, (struct sockaddr *) &client_add, &sizeCli);

        // Allocating memory for client data and setting client information
        client_t *cli = (client_t *) malloc(sizeof(client_t));
        cli->indirizzo = client_add;
        cli->sockfd = clientdescriptor;

        // Adding the client to the queue and creating a thread to handle it
        addQueue(cli);
        pthread_create(&tid, NULL, (void *) handleClient, (void *) cli);
    }

    return 0;
}

// Function executed by each thread that handles a client
void handleClient(void *arg) {
    client_t *thisClient = (client_t *) arg;
    messageProtocol receivedMessage;

    while (1) {
        // Receiving a message from the client
        ssize_t bytercv = recv(thisClient->sockfd, &receivedMessage, SIZE_OF_MESSAGE_PROTOCOL, 0);

        // Handling disconnection or errors
        if (bytercv <= 0) {
            receivedMessage.typeMessage = MESSAGE_TYPE_LEAVE;
            printUserLeft(receivedMessage.sender);
            forward(receivedMessage, thisClient->sockfd);
            break;
        }

        // Handling join and regular messages
        else if (receivedMessage.typeMessage == MESSAGE_TYPE_JOIN)
            printUserJoin(receivedMessage.sender);
        else if (receivedMessage.typeMessage == MESSAGE_TYPE_SEND)
            printf("%s (%d): %s\n", receivedMessage.sender, thisClient->sockfd, receivedMessage.message);

        // Forwarding the message to other clients
        forward(receivedMessage, thisClient->sockfd);
    }

    // Closing the client socket, removing it from the queue, freeing memory, and detaching the thread
    close(thisClient->sockfd);
    removeQueue(thisClient->sockfd);
    free(thisClient);
    pthread_detach(pthread_self());
}

// Function for forwarding messages to all clients except the sender
void forward(messageProtocol message, int senderSocketDescriptor) {
    pthread_mutex_lock(&clients_mutex);    // Acquiring lock to prevent modification of the client list while forwarding messages

    // Forwarding the message
    for (int i = 0; i < LIMIT; i++)
        if (clients[i] != NULL && clients[i]->sockfd != senderSocketDescriptor)
            send(clients[i]->sockfd, &message, SIZE_OF_MESSAGE_PROTOCOL, 0);

    pthread_mutex_unlock(&clients_mutex);  // Releasing the lock
}
