#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <ctype.h>

//login 0X01
//timeout 0X02
//post 0X03
//client ack 0X04
//in session 0X05
//subscribe 0X06
//sub notification 0X07
//unsub 0X08
//retrieve 0X09
//retreival feed 0X11
//logout server 0X12

int main() {

    int ret;
    int sockfd;
    int recv_len;
    int m;

    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    struct sockaddr_in cliaddr2;
    struct sockaddr_in cliaddr3;

    socklen_t len;

    char recv_buffer[1024];
    char send_buffer[6000];
    char output[6000];

    time_t end_timer = 30;
    time_t start;
    time_t end;

    char sub_1[50], sub_2[50], sub_3[50] = "none";

    char array_1[32][256], array_2[32][256], array_3[32][256];
    char array_21[32][256], array_31[32][256], array_32[32][256];

    int ctr_1, ctr_2, ctr_3 = 0;
    int ctr_21, ctr_31, ctr_32 = 0;
    
    char recent_msg[201];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("socket() error: %s.\n", strerror(errno));
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);

    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliaddr.sin_port = htons(32001);


    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    while (1) {
        FILE *logfile = fopen ("logFile.txt", "a+");
        if (logFile == NULL)
        {
            fprintf(stderr, "\nError\n");
            exit (1);
        }

        len = sizeof(cliaddr);
        recv_len = recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &cliaddr, &len);

        if (recv_len <= 0) {
            printf("recvfrom() error: %s.\n", strerror(errno));
            return -1;
        }

        //user1
        if (recv_buffer[0] == 0x55 && recv_buffer[1] == 0x31)
        {
            if (recv_buffer[2] == 0x01)
            {
                memset(recent_msg, 0, sizeof(recent_msg));
                memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                char user_input[1024] = "login_ack#successful\nUser is logged in\n\nUser feed: \n";

                if(strncmp(sub_1, "none",4) == 0)
                {
                    strcat(user_input, "EMPTY\n\n");

                } else if(strncmp(sub_1, "user2",5) == 0)
                {
                    for(int i = ctr_2 +1; i > 0; i--)
                    {
                        strcat(array_2[i-1], "\n");
                        strcat(user_input, array_2[i-1]);
                    }
                } else if(strncmp(sub_1, "user3",5) == 0)
                {
                    for(int i = ctr_3+1; i > 0; i--)
                    {
                        strcat(array_3[i-1], "\n");
                        strcat(user_input, array_3[i-1]);
                    }
                }
                else if(strncmp(sub_1, "user23",6) == 0)
                {
                    for(int i = ctr_32+1; i > 0; i--)
                    {
                        strcat(array_32[i-1], "\n");
                        strcat(user_input, array_32[i-1]);
                    }
                }

                m = strlen(user_input);
                memcpy(send_buffer + 4, user_input, m);

                send_buffer[0] = 0x4D;
                send_buffer[1] = 0x41;
                send_buffer[2] = 0x04;
                send_buffer[3] = m;

                start = time(NULL);
                end = start + end_timer;
            }
            //posts
            else if (recv_buffer[2] == 0x03)
            {
                start = time(NULL);
                //timed out
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //in session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    strcpy(array_1[ctr_1],recent_msg);
                    strcpy(array_21[ctr_21],recent_msg);
                    strcpy(array_31[ctr_31],recent_msg);

                    ctr_1++;
                    ctr_21++;
                    ctr_31++;

                    m = strlen(recent_msg);
                    memcpy(send_buffer + 4, recent_msg, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x05; 
                    send_buffer[3] = m;

                }
            }
            //subscribe
            else if (recv_buffer[2] == 0X06)
            {
                start = time(NULL);
                //timed out
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //in session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    //sub to user 2
                    if(strncmp(recent_msg, "user2",5) == 0)
                    {
                        //sub to user 2
                        if(strncmp(sub_1, "none", 4) == 0 || strncmp(sub_1, "user2", 5) == 0)
                        {
                            strcpy(sub_1, "user2");
                            char user_input[1024] = "Notification: Subscribed user 2\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //sub to both user 2 and 3
                        else if(strncmp(sub_1, "user3", 5)== 0)
                        {
                            strcpy(sub_1, "user23");
                            char user_input[1024] = "Notification: Subscribed Users 2 and 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    //sub to user 3
                    else if(strncmp(recent_msg, "user3",5) == 0)
                    {
                        //sub to user3
                        if(strncmp(sub_1, "none", 4) == 0 || strncmp(sub_1, "user3", 5) == 0)
                        {
                            strcpy(sub_1, "user3");
                            char user_input[1024] = "Notification: Subscribed User 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //sub to user 2 and 3
                        else if(strncmp(sub_1, "user2", 5)== 0)
                        {
                            strcpy(sub_1, "user23");
                            char user_input[1024] = "Notification: Subscribed Users 2 and 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    else
                    {
                        memset(recent_msg, 0, sizeof(recent_msg));
                        memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                        char user_input[1024] = "session reset by server\n";

                        m = strlen(user_input);
                        memcpy(send_buffer + 4, user_input, m);

                        send_buffer[0] = 0x4D;   
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X12; 
                        send_buffer[3] = m;
                    }
                }
            }
            //unsubscribe
            else if (recv_buffer[2] == 0X08)
            {
                start = time(NULL);
                //timed out
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";
                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02;
                    send_buffer[3] = m;
                }
                //in session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    //unsub to user2
                    if(strncmp(recent_msg, "user2",5) == 0)
                    {
                        //unsubs user2
                        if(strncmp(sub_2, "user2", 5) == 0)
                        {
                            strcpy(sub_2, "none");
                            char user_input[1024] = "Notification: unsubscribed user 2\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }

                        //sub to user 2 and 3 and unsubs only user 2
                        else if(strncmp(sub_2, "user23", 6)== 0)
                        {
                            strcpy(sub_2, "user3");
                            char user_input[1024] = "Notification: unsubscribed user 2, Subscribed 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //unsub to user3
                        else if(strncmp(sub_2, "user3", 5) == 0)
                        {
                            strcpy(sub_2, "user2");
                            char user_input[1024] = "Notification: unsubscribed user 3, still subscribed 2.\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                    }
                    //unsub to user3
                    else if(strncmp(recent_msg, "user3",5) == 0)
                    {
                        //unsub to user3
                        if(strncmp(sub_2, "user3", 5) == 0)
                        {
                            strcpy(sub_2, "none");
                            char user_input[1024] = "Notification: unsubscribed user 3. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //unsub user 3 when sub to both user 2 and 3
                        else if(strncmp(sub_2, "user13", 6)== 0)
                        {
                            strcpy(sub_2, "user2");
                            char user_input[1024] = "Notification: unsubscribed user 3, still subscribed 2. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //unsub to user 2
                        else if(strncmp(sub_2, "user2", 5) == 0)
                        {
                            strcpy(sub_2, "user2");
                            char user_input[1024] = "Notification: unsubscribed user 2 and 3.\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                    }
                    //if not sub to anyone
                    else if (strncmp(recent_msg, "none",4) == 0)
                    {
                        strcpy(sub_1, "none");
                        char user_input[1024] = "Notification: not subscribed anyone. \n";
                        m = strlen(user_input);
                        memcpy(send_buffer + 5, user_input, m);

                        send_buffer[0] = 0x4D;
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X07;
                        send_buffer[3] = m;
                    }
                    //otherwise
                    else
                    {
                        memset(recent_msg, 0, sizeof(recent_msg));
                        memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                        char user_input[1024] = "session reset by server\n";
                        m = strlen(user_input);
                        memcpy(send_buffer + 4, user_input, m);

                        send_buffer[0] = 0x4D;
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X12;
                        send_buffer[3] = m;

                    }
                }
            }
                //retrieve
            else if (recv_buffer[2] == 0X09)
            {
                start = time(NULL);
                //if session ended
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";
                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02;
                    send_buffer[3] = m;
                }
                    //with in session
                else {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char _temp = recent_msg[0];
                    printf("%c", _temp);

                    if (isdigit(_temp)) {
                        if (isdigit(_temp)) {
                            int temp = (int) recent_msg;
                            memset(output, 0, sizeof(output));

                            m = strlen(output);
                            memcpy(send_buffer + 4, output, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X11;
                            send_buffer[3] = m;
                        } else {
                            memset(recent_msg, 0, sizeof(recent_msg));
                            memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                            char user_input[1024] = "session reset by server\n";

                            m = strlen(user_input);
                            memcpy(send_buffer + 4, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X12;
                            send_buffer[3] = m;
                        }
                    }
                }
            }
            else{
                continue;
            }
        }
        //user 2
        else if (recv_buffer[0] == 0x55 && recv_buffer[1] == 0x32)
        {
            //user2 login
            if (recv_buffer[2] == 0x01)
            {
                memset(recent_msg, 0, sizeof(recent_msg));
                memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                char user_input[1024] = "login_ack#successful\nUser is logged in\n\nUser feed: \n";

                if(strncmp(sub_2, "none",4) == 0)
                {
                    strcat(user_input, "EMPTY\n\n");

                }
                else if(strncmp(sub_2, "user1",5) == 0)
                {
                    for(int i = ctr_1+1; i > 0; i--)
                    {
                        strcat(array_1[i-1], "\n");
                        strcat(user_input, array_1[i-1]);
                    }
                }
                else if(strncmp(sub_2, "user3",5) == 0)
                {
                    for(int i = ctr_3+1; i > 0; i--)
                    {
                        strcat(array_3[i-1], "\n");
                        strcat(user_input, array_3[i-1]);
                    }
                }
                else if(strncmp(sub_1, "user13",6) == 0)
                {
                    for(int i = ctr_31+1; i > 0; i--)
                    {
                        strcat(array_31[i-1], "\n");
                        strcat(user_input, array_31[i-1]);
                    }
                }
                m = strlen(user_input);
                memcpy(send_buffer + 4, user_input, m);

                send_buffer[0] = 0x4D;   
                send_buffer[1] = 0x41;
                send_buffer[2] = 0x02; 
                send_buffer[3] = m;

                start = time(NULL);
                end = start + end_timer;
            }
            //posts
            else if (recv_buffer[2] == 0x03)
            {
                start = time(NULL);
                //timeout
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //in session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    strcpy(array_2[ctr_2],recent_msg);
                    strcpy(array_21[ctr_21],recent_msg);
                    strcpy(array_32[ctr_32],recent_msg);

                    ctr_2++;
                    ctr_21++;
                    ctr_32++;

                    m = strlen(recent_msg);
                    memcpy(send_buffer + 4, recent_msg, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x05; 
                    send_buffer[3] = m;
                }
            }
            //subscribe
            else if (recv_buffer[2] == 0X06)
            {
                start = time(NULL);
                //timed out
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //in session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    //sub to user 1
                    if(strncmp(recent_msg, "user1",5) == 0)
                    {
                        //sub to user1 only
                        if(strncmp(sub_2, "none", 4) == 0 && strncmp(sub_2, "user1", 5) == 0)
                        {
                            strcpy(sub_2, "user1");
                            char user_input[1024] = "Notification: subscribed user 1\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //sub to user 1 and 3
                        else if(strncmp(sub_2, "user3", 5)== 0)
                        {
                            strcpy(sub_2, "user13");
                            char user_input[1024] = "Notification: subscribed users 1 and 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }

                    //sub to user3
                    else if(strncmp(recent_msg, "user3",5) == 0)
                    {
                        //sub to user 3 only
                        if(strncmp(sub_2, "none", 4) == 0 && strncmp(sub_2, "user3", 5) == 0)
                        {
                            strcpy(sub_2, "user3");
                            char user_input[1024] = "Notification: subscribed user 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //sub to user 1 and 3
                        else if(strncmp(sub_2, "user1", 5)== 0)
                        {
                            strcpy(sub_2, "user13");
                            char user_input[1024] = "Notification: subscribed users 1 and 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    //otherwise
                    else
                    {
                        memset(recent_msg, 0, sizeof(recent_msg));
                        memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                        char user_input[1024] = "session reset by server\n";
                        m = strlen(user_input);
                        memcpy(send_buffer + 4, user_input, m);

                        send_buffer[0] = 0x4D;   
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X12; 
                        send_buffer[3] = m;
                    }
                }
            }
            //unsubscribe
            else if (recv_buffer[2] == 0X08)
            {
                start = time(NULL);
                //timed out
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";
                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //in session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    //unsub to user1
                    if(strncmp(recent_msg, "user1",5) == 0)
                    {
                        //unsubs user1
                        if(strncmp(sub_2, "user1", 5) == 0)
                        {
                            strcpy(sub_2, "none");
                            char user_input[1024] = "Notification: unsubscribed user 1\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }

                        //sub to user1 and 3 and unsubs only user 1
                        else if(strncmp(sub_2, "user13", 6)== 0)
                        {
                            strcpy(sub_2, "user3");
                            char user_input[1024] = "Notification: unsubscribed user 1, Subscribed 3\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                        //unsub to user3
                        else if(strncmp(sub_2, "user3", 5) == 0)
                        {
                            strcpy(sub_2, "user1");
                            char user_input[1024] = "Notification: not subscribed user 3 only 1.\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                    }
                    //unsub to user3
                    else if(strncmp(recent_msg, "user3",5) == 0)
                    {
                        //unsub to user3
                        if(strncmp(sub_2, "user3", 5) == 0)
                        {
                            strcpy(sub_2, "none");
                            char user_input[1024] = "Notification: not subscribed user 3. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //unsub user 3 when sub to both user 1 and 3
                        else if(strncmp(sub_2, "user13", 6)== 0)
                        {
                            strcpy(sub_2, "user1");
                            char user_input[1024] = "Notification: unsubscribed user 3, still subscribed 1. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                        //unsub to user 1
                        else if(strncmp(sub_2, "user1", 5) == 0)
                        {
                            strcpy(sub_2, "user1");
                            char user_input[1024] = "Notification: not subscribed user 3 only 1.\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    //if not sub to anyone
                    else if (strncmp(recent_msg, "none",4) == 0)
                    {
                        strcpy(sub_1, "none");
                        char user_input[1024] = "Notification: not subscribed anyone. \n";
                        m = strlen(user_input);
                        memcpy(send_buffer + 5, user_input, m);

                        send_buffer[0] = 0x4D;
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X07;
                        send_buffer[3] = m;
                    }
                    //otherwise
                    else
                    {
                        memset(recent_msg, 0, sizeof(recent_msg));
                        memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                        char user_input[1024] = "session reset by server\n";
                        m = strlen(user_input);
                        memcpy(send_buffer + 4, user_input, m);

                        send_buffer[0] = 0x4D;   
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X12; 
                        send_buffer[3] = m;

                    }
                }
            }
            //retrieve
            else if (recv_buffer[2] == 0X09)
            {
                start = time(NULL);
                //if session ended
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";
                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02;
                    send_buffer[3] = m;
                }
                    //with in session
                else {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char _temp = recent_msg[0];
                    printf("%c", _temp);

                    if (isdigit(_temp)) {
                        if (isdigit(_temp)) {
                            int temp = (int) recent_msg;
                            memset(output, 0, sizeof(output));

                            m = strlen(output);
                            memcpy(send_buffer + 4, output, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X11;
                            send_buffer[3] = m;
                        } else {
                            memset(recent_msg, 0, sizeof(recent_msg));
                            memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                            char user_input[1024] = "session reset by server\n";

                            m = strlen(user_input);
                            memcpy(send_buffer + 4, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X12;
                            send_buffer[3] = m;
                        }
                    }
                }
            }
            else{
                continue;
            }
        }

        //user3
        else if (recv_buffer[0] == 0x55 && recv_buffer[1] == 0x33)
        {
            //user3 login
            if (recv_buffer[2] == 0x01)
            {
                memset(recent_msg, 0, sizeof(recent_msg));
                memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                char user_input[1024] = "login_ack#successful\nUser is logged in\n\nUser feed: \n";

                if(strncmp(sub_3, "none",4) == 0)
                {
                    strcat(user_input, "EMPTY\n\n");

                } else if(strncmp(sub_3, "user1",5) == 0)
                {
                    for(int i = ctr_1+1; i > 0; i--)
                    {
                        strcat(array_1[i-1], "\n");
                        strcat(user_input, array_1[i-1]);
                    }
                } else if(strncmp(sub_3, "user2",5) == 0)
                {
                    for(int i = ctr_2+1; i > 0; i--)
                    {
                        strcat(array_2[i-1], "\n");
                        strcat(user_input, array_2[i-1]);
                    }
                } else if(strncmp(sub_3, "user12",6) == 0)
                {
                    for(int i = ctr_21+1; i > 0; i--)
                    {
                        strcat(array_21[i-1], "\n");
                        strcat(user_input, array_21[i-1]);
                    }
                }
                m = strlen(user_input);
                memcpy(send_buffer + 4, user_input, m);

                send_buffer[0] = 0x4D;   
                send_buffer[1] = 0x41;
                send_buffer[2] = 0x02; 
                send_buffer[3] = m;

                start = time(NULL);
                end = start + end_timer;
            }
            //user3 post
            else if (recv_buffer[2] == 0x03)
            {
                start = time(NULL);
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                else
                {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    strcpy(array_3[ctr_3],recent_msg);
                    strcpy(array_31[ctr_31],recent_msg);
                    strcpy(array_32[ctr_32],recent_msg);

                    ctr_3++;
                    ctr_31++;
                    ctr_32++;

                    m = strlen(recent_msg);
                    memcpy(send_buffer + 4, recent_msg, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x05; 
                    send_buffer[3] = m;

                }
            }

            //user3 subscribe
            else if (recv_buffer[2] == 0X06)
            {
                start = time(NULL);

                //user 3 timeout
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //within session
                else
                {
                    start = time(NULL);

                    end = start + end_timer;
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    //subscribe user 2
                    if(strncmp(recent_msg, "user2",5) == 0)
                    {
                        //only sub user2
                        if(strncmp(sub_3, "none", 4 &) == 0 && strncmp(sub_3, "user2", 5) == 0)
                        {
                            strcpy(sub_3, "user2");
                            char user_input[1024] = "Notification: Subscribed User 2\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        //subscribe user 1 and 2
                        else if(strncmp(sub_3, "user1", 5)== 0)
                        {
                            strcpy(sub_3, "user12");
                            char user_input[1024] = "Notification: Subscribed User 1 and 2\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }

                    //sub to user 1
                    else if(strncmp(recent_msg, "user1",5) == 0)
                    {
                        //only subs to user 1
                        if(strncmp(sub_3, "none", 4) == 0 && strncmp(sub_3, "user1", 5) == 0)
                        {
                            strcpy(sub_3, "user1");
                            char user_input[1024] = "Notification: Subscribed User 1\n";

                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07;
                            send_buffer[3] = m;
                        }
                        else if(strncmp(sub_3, "user2", 5)== 0)
                        {
                            strcpy(sub_3, "user12");
                            char user_input[1024] = "Notification: Subscribed User 1 and 2\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    //any useer other than user 1 and 2
                    else
                    {
                        memset(recent_msg, 0, sizeof(recent_msg));
                        memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                        char user_input[1024] = "Session Reset by Server\n";

                        m = strlen(user_input);
                        memcpy(send_buffer + 4, user_input, m);

                        send_buffer[0] = 0x4D;   
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X12; 
                        send_buffer[3] = m;

                    }
                }
            }
            //unsubscribe
            else if (recv_buffer[2] == 0X08)
            {
                start = time(NULL);
                //if session ended
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";

                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }

                //within session
                else
                {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);

                    //unsub to user2
                    if(strncmp(recent_msg, "user2",5) == 0)
                    {
                        //if sub to both the users and unsubs user 2
                        if(strncmp(sub_3, "user12", 6)== 0)
                        {
                            strcpy(sub_3, "user1");
                            char user_input[1024] = "Notification: Unsubscribed User 2, Still Subscribed User 1\n";

                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                        //unsubs user 2 if only sub to user 2
                        else if(strncmp(sub_3, "user2", 5) == 0)
                        {
                            strcpy(sub_3, "none");
                            char user_input[1024] = "Notification: Unsubscribed User 2\n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                        //if not sub to anyone
                        else if(strncmp(sub_3, "none", 4) == 0)
                        {
                            strcpy(sub_3, "none");
                            char user_input[1024] = "Notification: not subscribed anyone. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    //unsub to user1
                    else if(strncmp(recent_msg, "user1",5) == 0)
                    {
                        //unsub to user1 when sub to both user1 and user 2
                        if(strncmp(sub_3, "user12", 6)== 0)
                        {
                            strcpy(sub_3, "user2");
                            char user_input[1024] = "Notification: unsubscribed user 1, still subscribed 2. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                        //unsub to user1 when only sub to user 1
                        else if(strncmp(sub_3, "user1", 5) == 0)
                        {
                            strcpy(sub_3, "none");
                            char user_input[1024] = "Notification: not subscribed user 1. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                        //no sub
                        else if(strncmp(sub_3, "none", 4) == 0)
                        {
                            strcpy(sub_3, "none");
                            char user_input[1024] = "Notification: not subscribed anyone. \n";
                            m = strlen(user_input);
                            memcpy(send_buffer + 5, user_input, m);

                            send_buffer[0] = 0x4D; 
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X07; 
                            send_buffer[3] = m;
                        }
                    }
                    //session reset otherwise
                    else
                    {
                        memset(recent_msg, 0, sizeof(recent_msg));
                        memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                        char user_input[1024] = "session reset by server\n";
                        m = strlen(user_input);
                        memcpy(send_buffer + 4, user_input, m);

                        send_buffer[0] = 0x4D;   
                        send_buffer[1] = 0x41;
                        send_buffer[2] = 0X12; 
                        send_buffer[3] = m;
                    }
                }
            }
            //retrieve
            else if (recv_buffer[2] == 0X09)
            {
                start = time(NULL);
                //if session ended
                if(start >= end)
                {
                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char user_input[1024] = "logout#";
                    m = strlen(user_input);
                    memcpy(send_buffer + 4, user_input, m);

                    send_buffer[0] = 0x4D;   
                    send_buffer[1] = 0x41;
                    send_buffer[2] = 0x02; 
                    send_buffer[3] = m;
                }
                //with in session
                else {
                    start = time(NULL);
                    end = start + end_timer;

                    memset(recent_msg, 0, sizeof(recent_msg));
                    memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                    char _temp = recent_msg[0];
                    printf("%c", _temp);

                    if (isdigit(_temp)) {
                        if (isdigit(_temp)) {
                            int temp = (int) recent_msg;
                            memset(output, 0, sizeof(output));

                            m = strlen(output);
                            memcpy(send_buffer + 4, output, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X11;
                            send_buffer[3] = m;
                        } else {
                            memset(recent_msg, 0, sizeof(recent_msg));
                            memcpy(recent_msg, recv_buffer + 4, recv_buffer[3]);
                            char user_input[1024] = "session reset by server\n";

                            m = strlen(user_input);
                            memcpy(send_buffer + 4, user_input, m);

                            send_buffer[0] = 0x4D;
                            send_buffer[1] = 0x41;
                            send_buffer[2] = 0X12;
                            send_buffer[3] = m;
                        }
                    }
                }
            }
            else{
                    continue;
            }
        }
        else{
                continue;
        }
        ret = sendto(sockfd, send_buffer, strlen(send_buffer), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
        if (ret <= 0) {
            printf("sendto() error: %s.\n", strerror(errno));
            return -1;
        }
        fclose (logFile);
    }

    close(sockfd);
    return 0;
}
