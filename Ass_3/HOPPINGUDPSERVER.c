#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

struct packet
{
    int frequency;
    char data;
};

int main()
{
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;

    socklen_t addr_size;

    struct packet p;

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));

    addr_size = sizeof(clientAddr);

    printf("Waiting...\n");

    while(1)
    {
        recvfrom(serverSocket,
                 &p,
                 sizeof(p),
                 0,
                 (struct sockaddr*)&clientAddr,
                 &addr_size);

        if(p.data=='#')
            break;

        printf("Received '%c' on %d MHz\n",
               p.data,
               p.frequency);
    }

    close(serverSocket);
}
