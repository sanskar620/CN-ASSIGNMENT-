#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void xorOperation(char *data, char *divisor, int pos)
{
    int len = strlen(divisor);

    for(int i = 0; i < len; i++)
    {
        if(data[pos + i] == divisor[i])
            data[pos + i] = '0';
        else
            data[pos + i] = '1';
    }
}

int main()
{
    int serverSocket, clientSocket;

    struct sockaddr_in server, client;
    socklen_t clientSize = sizeof(client);

    char dividend[100];
    char divisor[100];
    char codeword[200];
    char temp[200];

    int divisorLen;
    int codeLen;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,
         (struct sockaddr*)&server,
         sizeof(server));

    listen(serverSocket, 5);

    printf("Receiver waiting...\n");

    clientSocket = accept(serverSocket,
                           (struct sockaddr*)&client,
                           &clientSize);

    // Receive data
    recv(clientSocket, dividend, sizeof(dividend), 0);
    recv(clientSocket, divisor, sizeof(divisor), 0);
    recv(clientSocket, codeword, sizeof(codeword), 0);

    printf("\nReceived Codeword: %s\n", codeword);

    divisorLen = strlen(divisor);
    codeLen = strlen(codeword);

    // Copy received codeword
    strcpy(temp, codeword);

    // CRC division
    for(int i = 0; i <= codeLen - divisorLen; i++)
    {
        if(temp[i] == '1')
            xorOperation(temp, divisor, i);
    }

    // Check remainder
    int error = 0;

    for(int i = codeLen - (divisorLen - 1);
        i < codeLen;
        i++)
    {
        if(temp[i] != '0')
        {
            error = 1;
            break;
        }
    }

    if(error == 0)
    {
        printf("CRC Remainder: ");

        for(int i = codeLen - (divisorLen - 1);
            i < codeLen;
            i++)
        {
            printf("%c", temp[i]);
        }

        printf("\nNo error detected\n");
    }
    else
    {
        printf("CRC Remainder: ");

        for(int i = codeLen - (divisorLen - 1);
            i < codeLen;
            i++)
        {
            printf("%c", temp[i]);
        }

        printf("\nError detected\n");
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}