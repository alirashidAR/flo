#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void func(int connfd) {
    char buff[MAX]; 
    int n; 

    for (;;) {
        memset(buff, 0, sizeof(buff));  // Clear the buffer

        // Read from client
        int bytesRead = read(connfd, buff, sizeof(buff) - 1);
        if (bytesRead < 0) {
            perror("Read failed");
            break;
        }
        
        buff[bytesRead] = '\0';  // Null-terminate the received data

        printf("From client: %s\t To client: ", buff);
        memset(buff, 0, sizeof(buff));  // Clear the buffer for the response

        // Get user input
        n = 0;
        while ((buff[n++] = getchar()) != '\n' && n < MAX - 1);
        buff[n-1] = '\0'; // Null-terminate the string

        // Write to client
        if (write(connfd, buff, strlen(buff)) < 0) {
            perror("Write failed");
            break;
        }

        // Check for exit command
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd; 
    socklen_t len;  // Change int to socklen_t
    struct sockaddr_in servaddr, cli; 

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Socket creation failed...\n"); 
        exit(0); 
    } else {
        printf("Socket successfully created..\n"); 
    }
    
    memset(&servaddr, 0, sizeof(servaddr)); 

    // Assign PORT and IP
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 

    // Binding the created socket to the IP
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("Socket bind failed...\n"); 
        exit(0); 
    } else {
        printf("Socket successfully binded..\n"); 
    }

    // Server listens for requests
    if (listen(sockfd, 5) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } else {
        printf("Server listening..\n"); 
    }
    
    len = sizeof(cli);  // Initialize len here

    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("Server accept failed...\n"); 
        exit(0); 
    } else {
        printf("Server accepted the client...\n"); 
    }

    // Function for chatting 
    func(connfd); 

    // After chatting  
    close(connfd); // Close the connection to the client
    close(sockfd); // Close the server socket
    return 0; 
}
