#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <malloc.h>

void error_handling(char *msg);

int main(int argc, char *argv[]){
    int sock, min_port = 0, max_port = 65535, i, open_port_count = 0;
    int *port_list;
    struct sockaddr_in serv_addr;

    if(argc < 2){
        printf("Usage : %s <IP> <MIN_PORT (default = 0)> <MAX_PORT (default = 65535)>\n", argv[0]);
        return 0;
    }

    if(argc >= 3) min_port = atoi(argv[2]);
    if(argc >= 4) max_port = atoi(argv[3]);


    if(min_port > max_port) error_handling("<MIN_PORT> must be less than or equal to <MAX_PORT>");

    port_list = (int *)malloc(sizeof(int) * (max_port - min_port + 1));

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) error_handling("socket() error");

    for(i = min_port; i <= max_port; i++){
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(i);
        printf("Port %d is ", i);
        if(!connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))){
            port_list[open_port_count++] = i;
            close(sock);
            puts("\x1B[32mOpened\x1B[37m");
        } else puts("\x1B[31mClosed\x1B[37m");
    }

    puts("\n\n===== Result =====\n\n");
    for(i = 0; i < open_port_count; i++){
        printf("%d Port is Opened. ", port_list[i]);

        switch(port_list[i]){
            case 0 ... 1023 :
            puts("(Well-Known Port)\n");
            break;

            case 1024 ... 49151 :
            puts("(Registered Port)\n");
            break;

            case 49152 ... 65535 :
            puts("(Dynamic Port)\n");
            break;
        }
    }

    free(port_list);
    return 0;
}

void error_handling(char *msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
