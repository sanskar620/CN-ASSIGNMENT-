#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int clientSocket;
    int a, b, result;
    char op;

    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket < 0)
    {
        printf("Socket Creation Failed\n");
        return 0;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(clientSocket,
               (struct sockaddr *)&serverAddr,
               sizeof(serverAddr)) < 0)
    {
        printf("Connection Failed\n");
        return 0;
    }

    printf("Connected to Server\n");

    printf("Enter First Number: ");
    scanf("%d", &a);

    printf("Enter Second Number: ");
    scanf("%d", &b);

    printf("Enter Operator (+,-,*,/): ");
    scanf(" %c", &op);

    send(clientSocket, &a, sizeof(a), 0);
    send(clientSocket, &b, sizeof(b), 0);
    send(clientSocket, &op, sizeof(op), 0);

    recv(clientSocket, &result, sizeof(result), 0);

    printf("Result = %d\n", result);

    close(clientSocket);

    return 0;
}
