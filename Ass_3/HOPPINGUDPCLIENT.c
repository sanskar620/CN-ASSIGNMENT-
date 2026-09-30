#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

struct packet
{
    int frequency;
    char data;
};

int main()
{
    int clientSocket;

    struct sockaddr_in serverAddr;

    struct packet p;

    int freq[] = {900,905,910,915,920};

    char msg[] = "HELLO";

    clientSocket = socket(AF_INET,
                          SOCK_DGRAM,
                          0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr =
        inet_addr("127.0.0.1");

    for(int i=0;i<strlen(msg);i++)
    {
        p.frequency = freq[i%5];
        p.data = msg[i];

        sendto(clientSocket,
               &p,
               sizeof(p),
               0,
               (struct sockaddr*)&serverAddr,
               sizeof(serverAddr));

        printf("Sent '%c' on %d MHz\n",
               p.data,
               p.frequency);
    }

    p.data='#';

    sendto(clientSocket,
           &p,
           sizeof(p),
           0,
           (struct sockaddr*)&serverAddr,
           sizeof(serverAddr));

    close(clientSocket);
}
