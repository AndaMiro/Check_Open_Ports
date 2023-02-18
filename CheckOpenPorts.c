#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <malloc.h>

void error_handling(char *msg);

int main(int argc, char *argv[]){
    int sock, min_port, max_port, i, open_port_count = 0;
    int *port_list;
    struct sockaddr_in serv_addr;

    if(argc < 4){
        printf("Usage : %s <IP> <MIN_PORT> <MAX_PORT>", argv[0]);
        return 0;
    }

    min_port = atoi(argv[2]);
    max_port = atoi(argv[3]);


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
            printf("\x1B[32mOpened\x1B[37m\n");
        } else printf("\x1B[31mClosed\x1B[37m\n");
    }

    printf("\n\n===== Result =====\n\n");
    for(i = 0; i < open_port_count; i++) printf("%d Port is Opened.\n", port_list[i]);
    
    free(port_list);
    return 0;
}

void error_handling(char *msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
