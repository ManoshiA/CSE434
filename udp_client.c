#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//login 0X01
//timeout 0X02
//post 0X03
//client ack 0X04
//in session 0X05
//subscribe 0X06
//sub notification 0X07
//unsub 0x08
//retrieve 0X09
//logout server 0X12

int main() {

    int ret;
    int sockfd;
    int m;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    char send_buffer[1024];
    char recv_buffer[1024];
    char user_input[1024];
    char message[201];
    socklen_t len;

    char recent_message[201];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("socket() error: %s.\n", strerror(errno));
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(32000);

    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    cliaddr.sin_port = htons(32001);

    bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));

    while (1) {

        fgets(user_input, sizeof(user_input), stdin);
        m = 0;

        //login
        if(strncmp(user_input, "login#", 6) == 0)
        {
            if(strncmp(user_input, "login#user1&password1", 21) == 0)
            {
                m = strlen(user_input) - 6;
                memcpy(send_buffer + 4, user_input + 5, m);

                send_buffer[0] = 0x55;
                send_buffer[1] = 0x31;
                send_buffer[2] = 0x01;
                send_buffer[3] = m;

                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                if (ret <= 0) {
                    printf("sendto() error: %s.\n", strerror(errno));
                    return -1;
                }

                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                if (recv_buffer[0] != 0x4A || recv_buffer[1] != 0x41)
                {
                    continue;
                }
                else
                {
                    //ack
                    if (recv_buffer[2] == 0x04)
                    {
                        memset(message, 0, sizeof(message));
                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                        printf("%s\n", message);

                        while(strncmp(user_input, "logout#", 7) != 0)
                        {
                            printf("Enter command: \n");
                            fgets(user_input, sizeof(user_input), stdin);

                            //post
                            if(strncmp(user_input, "post#", 5) == 0)
                            {
                                m = strlen(user_input);

                                memcpy(send_buffer + 4, user_input + 5, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x31;
                                send_buffer[2] = 0x03;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    //timeout
                                    if (recv_buffer[2] == 0x02)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    //in session
                                    else if (recv_buffer[2] == 0x05){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("Notification: %s\n", message);
                                    }

                                }

                            }
                            //subscribe
                            else if(strncmp(user_input, "subscribe#", 10) == 0)
                            {
                                m = strlen(user_input)-10;

                                memcpy(send_buffer + 4, user_input + 10, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x31;
                                send_buffer[2] = 0x06;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                //error
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x04)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x07){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }


                                }

                            }
                            //unsub
                            else if(strncmp(user_input, "unsubscribe#", 12) == 0)
                            {

                                m = strlen(user_input)-12;

                                memcpy(send_buffer + 4, user_input + 12, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x31;
                                send_buffer[2] = 0X08;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x04)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x07){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            //retrieve
                            else if(strncmp(user_input, "retrieve#", 9) == 0)
                            {

                                m = strlen(user_input)-9;

                                memcpy(send_buffer + 4, user_input + 9, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x31;
                                send_buffer[2] = 0x09;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x02)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x11){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);

                                        printf("Your retrieval feed is: \n%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);

                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }


                                }

                            }
                            else
                            {
                                printf("Session reset by client.\n");
                                strcpy(user_input, "logout#");
                            }
                        }
                        printf("You are logged out. \n");
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            //user 2
            else if(strncmp(user_input, "login#user2&password2", 21) == 0)
            {
                m = strlen(user_input) - 6;

                memcpy(send_buffer + 4, user_input + 5, m);

                send_buffer[0] = 0x55;
                send_buffer[1] = 0x32;
                send_buffer[2] = 0x01;
                send_buffer[3] = m;

                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                if (ret <= 0) {
                    printf("sendto() error: %s.\n", strerror(errno));
                    return -1;
                }

                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                {
                    continue;
                }
                else
                {
                    if (recv_buffer[2] == 0x04)
                    {
                        memset(message, 0, sizeof(message));
                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                        printf("%s\n", message);
                        
                        while(strncmp(user_input, "logout#", 7) != 0)
                        {
                            printf("Enter command: \n");
                            fgets(user_input, sizeof(user_input), stdin);

                            if(strncmp(user_input, "post#", 5) == 0)
                            {
                                m = strlen(user_input);

                                memcpy(send_buffer + 4, user_input + 5, m);

                                send_buffer[0] = 0x55; 
                                send_buffer[1] = 0x32;
                                send_buffer[2] = 0x03;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x02)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x05){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("Notification: %s\n", message);
                                    }

                                }

                            }
                            //sub
                            else if(strncmp(user_input, "subscribe#", 10) == 0)
                            {
                                m = strlen(user_input)-10;

                                memcpy(send_buffer + 4, user_input + 10, m);

                                send_buffer[0] = 0x55; 
                                send_buffer[1] = 0x32;
                                send_buffer[2] = 0x06;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x04)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x07){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            else if(strncmp(user_input, "unsubscribe#", 12) == 0)
                            {
                                m = strlen(user_input)-12;

                                memcpy(send_buffer + 4, user_input + 12, m);

                                send_buffer[0] = 0x55; 
                                send_buffer[1] = 0x32;
                                send_buffer[2] = 0X08;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x04)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x07){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            else if(strncmp(user_input, "retrieve#", 9) == 0)
                            {
                                m = strlen(user_input)-9;

                                memcpy(send_buffer + 4, user_input + 9, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x32;
                                send_buffer[2] = 0x09;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x02)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x11){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("Your retrieval feed is: \n%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            else
                            {
                                printf("Session reset by client.\n\n");
                                strcpy(user_input, "logout#");
                            }
                        }
                        printf("You are logged out. \n");
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            //user3
            else if(strncmp(user_input, "login#user3&password3", 21) == 0)
            {
                m = strlen(user_input) - 6;

                memcpy(send_buffer + 4, user_input + 5, m);

                send_buffer[0] = 0x55;
                send_buffer[1] = 0x33;
                send_buffer[2] = 0x01;
                send_buffer[3] = m;

                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                if (ret <= 0) {
                    printf("sendto() error: %s.\n", strerror(errno));
                    return -1;
                }

                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                {
                    continue;
                }
                else
                {
                    if (recv_buffer[2] == 0x04)
                    {
                        memset(message, 0, sizeof(message));
                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                        printf("%s\n", message);

                        while(strncmp(user_input, "logout#", 7) != 0)
                        {
                            printf("Enter command: \n");
                            fgets(user_input, sizeof(user_input), stdin);

                            //post
                            if(strncmp(user_input, "post#", 5) == 0)
                            {
                                m = strlen(user_input);

                                memcpy(send_buffer + 4, user_input + 5, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x33;
                                send_buffer[2] = 0x03;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x02)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x05){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("Notification: %s\n", message);
                                    }
                                }
                            }
                            //subscribe
                            else if(strncmp(user_input, "subscribe#", 10) == 0)
                            {
                                m = strlen(user_input)-10;

                                memcpy(send_buffer + 4, user_input + 10, m);

                                send_buffer[0] = 0x55;
                                send_buffer[1] = 0x33;
                                send_buffer[2] = 0X06;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x04)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x07){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            //unsubscribe
                            else if(strncmp(user_input, "unsubscribe#", 12) == 0)
                            {
                                m = strlen(user_input)-12;

                                memcpy(send_buffer + 4, user_input + 12, m);

                                send_buffer[0] = 0x55; 
                                send_buffer[1] = 0x33;
                                send_buffer[2] = 0X08;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }

                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    //timeout
                                    if (recv_buffer[2] == 0x04)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x07){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            //retrieve
                            else if(strncmp(user_input, "retrieve#", 9) == 0)
                            {

                                m = strlen(user_input)-9;

                                memcpy(send_buffer + 4, user_input + 9, m);

                                send_buffer[0] = 0x55; 
                                send_buffer[1] = 0x33;
                                send_buffer[2] = 0x09;
                                send_buffer[3] = m;

                                ret = sendto(sockfd, send_buffer, m + 4, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
                                if (ret <= 0) {
                                    printf("sendto() error: %s.\n", strerror(errno));
                                    return -1;
                                }
                                recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);
                                if (recv_buffer[0] != 0x4D || recv_buffer[1] != 0x41)
                                {
                                    continue;
                                }
                                else
                                {
                                    if (recv_buffer[2] == 0x02)
                                    {
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        strcpy(user_input, message);
                                        printf("Your session has timed out.\n");

                                    }
                                    else if (recv_buffer[2] == 0x11){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("Your retrieval feed is: \n%s\n", message);
                                    }
                                    else if (recv_buffer[2] == 0x12){
                                        memset(message, 0, sizeof(message));
                                        memcpy(message, recv_buffer + 4, recv_buffer[3]);
                                        printf("%s\n", message);
                                        strcpy(user_input, "logout#");
                                    }
                                }
                            }
                            else
                            {
                                printf("Session reset by client.\n\n");
                                strcpy(user_input, "logout#");
                            }

                        }
                        printf("You are logged out. \n");
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            else
            {
                printf("login_ack#unsuccessful\n");
                continue;
            }

        }
        else
        {
            printf("ERROR: must login first\n");
            continue;
        }
    }

    return 0;
}
