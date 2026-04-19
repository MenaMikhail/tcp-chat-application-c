// Header "chat.h"

// Maximum lengths for message and sender name
#define MAX_MESSAGE_LENGTH 100
#define MAX_SENDER_NAME_LENGTH 32

// Size of the messageProtocol structure
#define SIZE_OF_MESSAGE_PROTOCOL sizeof(messageProtocol)

// color codes for text formatting
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_RESET   "\x1b[0m"

// Enumeration for different types of chat messages
typedef enum {
    MESSAGE_TYPE_JOIN,   // Indicates a user joining the chat
    MESSAGE_TYPE_LEAVE,  // Indicates a user leaving the chat
    MESSAGE_TYPE_SEND,   // Indicates a message being sent
} messageType;

// Structure representing a chat message
typedef struct {
    messageType typeMessage;                // Type of the message (join, leave, send)
    char sender[MAX_SENDER_NAME_LENGTH];    // Sender's name
    char message[MAX_MESSAGE_LENGTH];       // Message content
} messageProtocol;


// Function prototypes for printing formatted messages and status

// Print in red color
void printRed(char *str) {
    printf(COLOR_RED "%s" COLOR_RESET, str);
}

// Print in green color
void printGreen(char *str) {
    printf(COLOR_GREEN "%s" COLOR_RESET, str);
}

// Print in cyan color
void printCyan(char *str) {
    printf(COLOR_CYAN "%s" COLOR_RESET, str);
}

// Print in yellow color
void printYellow(char *str) {
    printf(COLOR_YELLOW "%s" COLOR_RESET, str);
}

// Clear input buffer
void clearInput() {
    printf("\33[2K\r"); // Clear the current line
}

// Print a chat message
void printMessage(char* name, char* msg){
    printCyan(name);    // Print sender's name in cyan color
    printf(": %s", msg); // Print message content
    printf("\n");
}

// Print sending interface
void printSendingInterface(char* name){
    printYellow(name);  // Print sender's name in yellow color
    printf(": ");
}

// Print notification for user joining the chat
void printUserJoin(char* name){
    char formatString[100];
    sprintf(formatString, "=== %s has joined the chat ===", name); // Create formatted string

    printGreen(formatString);   // Print formatted string in green color
    printf("\n");
}

// Print notification for user leaving the chat
void printUserLeft(char* name){
    char formatString[100];
    sprintf(formatString, "=== %s has left the chat ===", name); // Create formatted string

    printRed(formatString);     // Print formatted string in red color
    printf("\n");
}

// Print error status message
void printErrorStatus(char* message){
    printRed("ERROR: ");    // Print "ERROR: " in red color
    printf("%s", message);  // Print error message
    printf("\n");
}

// Print OK status message
void printOKStatus(char* message){
    printGreen("OK: ");     // Print "OK: " in green color
    printf("%s", message);  // Print OK message
    printf("\n");
}
