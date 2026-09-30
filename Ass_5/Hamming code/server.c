#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in server;

    char data[5];
    char hamming[8];
    int h[7];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to receiver
    connect(sock, (struct sockaddr*)&server, sizeof(server));

    printf("Enter 4 data bits: ");
    scanf("%s", data);

    // Place data bits
    h[2] = data[0] - '0';
    h[4] = data[1] - '0';
    h[5] = data[2] - '0';
    h[6] = data[3] - '0';

    // Calculate parity bits
    h[0] = h[2] ^ h[4] ^ h[6];     // P1
    h[1] = h[2] ^ h[5] ^ h[6];     // P2
    h[3] = h[4] ^ h[5] ^ h[6];     // P4

    // Create Hamming code
    for(int i = 0; i < 7; i++)
    {
        hamming[i] = h[i] + '0';
    }

    hamming[7] = '\0';

    printf("\nGenerated Hamming Code: %s\n", hamming);

    // Send generated Hamming code
    send(sock, hamming, strlen(hamming) + 1, 0);

    printf("Hamming code sent to receiver.\n");

    close(sock);

    return 0;
}