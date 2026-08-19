#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
    int serverSocket, clientSocket;
    int a, b, result;
    char op;

    struct sockaddr_in serverAddr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket < 0)
    {
        printf("Socket Creation Failed\n");
        return 0;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,
         (struct sockaddr *)&serverAddr,
         sizeof(serverAddr));

    listen(serverSocket, 5);

    printf("Waiting for Client...\n");

    clientSocket = accept(serverSocket, NULL, NULL);

    printf("Client Connected\n");

    recv(clientSocket, &a, sizeof(a), 0);
    recv(clientSocket, &b, sizeof(b), 0);
    recv(clientSocket, &op, sizeof(op), 0);

    switch(op)
    {
        case '+':
            result = a + b;
            break;

        case '-':
            result = a - b;
            break;

        case '*':
            result = a * b;
            break;

        case '/':
            result = a / b;
            break;

        default:
            result = 0;
    }

    send(clientSocket, &result, sizeof(result), 0);

    close(clientSocket);
    close(serverSocket);

    return 0;
}
