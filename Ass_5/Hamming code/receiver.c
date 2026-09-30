#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in server, client;
    socklen_t clientSize = sizeof(client);

    char generated[8];
    char received[8];

    int h[7];
    int c1, c2, c4;
    int errorPosition;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(serverSocket,
         (struct sockaddr*)&server,
         sizeof(server));

    // Listen
    listen(serverSocket, 5);

    printf("Receiver waiting...\n");

    // Accept connection
    clientSocket = accept(serverSocket,
                           (struct sockaddr*)&client,
                           &clientSize);

    // Receive generated Hamming code
    recv(clientSocket, generated, sizeof(generated), 0);

    printf("\nGenerated Hamming Code: %s\n", generated);

    // User enters modified code
    printf("Enter received Hamming code: ");
    scanf("%s", received);

    // Convert received code to array
    for(int i = 0; i < 7; i++)
    {
        h[i] = received[i] - '0';
    }

    // Calculate C1
    c1 = h[0] ^ h[2] ^ h[4] ^ h[6];

    // Calculate C2
    c2 = h[1] ^ h[2] ^ h[5] ^ h[6];

    // Calculate C4
    c4 = h[3] ^ h[4] ^ h[5] ^ h[6];

    printf("\nC1 = %d", c1);
    printf("\nC2 = %d", c2);
    printf("\nC4 = %d", c4);

    // Calculate error position
    errorPosition = c1 * 1 + c2 * 2 + c4 * 4;

    if(errorPosition == 0)
    {
        printf("\n\nNo error detected.\n");
    }
    else
    {
        printf("\n\nError detected at position: %d\n",
               errorPosition);

        // Correct the error
        h[errorPosition - 1] =
            !h[errorPosition - 1];

        printf("Corrected Hamming Code: ");

        for(int i = 0; i < 7; i++)
        {
            printf("%d", h[i]);
        }

        printf("\n");

        // Compare with generated Hamming code
        int same = 1;

        for(int i = 0; i < 7; i++)
        {
            if(h[i] != generated[i] - '0')
            {
                same = 0;
                break;
            }
        }

        if(same)
            printf("Corrected code matches generated code.\n");
        else
            printf("Corrected code does not match generated code.\n");
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}