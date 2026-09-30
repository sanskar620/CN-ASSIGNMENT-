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
    int sock;
    struct sockaddr_in server;

    char dividend[100];
    char divisor[100];
    char temp[200];
    char codeword[200];

    int dataLen, divisorLen;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    printf("Dividend: ");
    scanf("%s", dividend);

    printf("Divisor: ");
    scanf("%s", divisor);

    dataLen = strlen(dividend);
    divisorLen = strlen(divisor);

    // Copy dividend
    strcpy(temp, dividend);

    // Append divisorLen - 1 zeros
    for(int i = 0; i < divisorLen - 1; i++)
        strcat(temp, "0");

    // CRC division
    for(int i = 0; i <= strlen(temp) - divisorLen; i++)
    {
        if(temp[i] == '1')
            xorOperation(temp, divisor, i);
    }

    // Create codeword = original dividend + remainder
    strcpy(codeword, dividend);

    for(int i = dataLen;
        i < dataLen + divisorLen - 1;
        i++)
    {
        char bit[2];

        bit[0] = temp[i];
        bit[1] = '\0';

        strcat(codeword, bit);
    }

    printf("\nCRC Remainder: ");

    for(int i = dataLen; i < dataLen + divisorLen - 1; i++)
        printf("%c", temp[i]);

    printf("\nTransmitted Codeword: %s\n", codeword);

    // Send dividend, divisor and codeword
    send(sock, dividend, sizeof(dividend), 0);
    send(sock, divisor, sizeof(divisor), 0);
    send(sock, codeword, sizeof(codeword), 0);

    close(sock);

    return 0;
}