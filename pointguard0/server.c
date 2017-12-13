# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>


# define MAX_SIZE 1024

int main(int argc, char * argv[]) {

    char buffer[MAX_SIZE], bye[10];
    int listen_fd, comm_fd, n, pid;

    struct sockaddr_in servaddr, cliaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_fd < 0) {
        perror("cannot open socket.");
        exit(1); }

    int PORT_NUM = atoi(argv[1]);
    printf("port= %d\n", PORT_NUM);

    bzero((char *) & servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(PORT_NUM);

    memset(servaddr.sin_zero, '\0', sizeof(servaddr.sin_zero));

    if (bind(listen_fd, (struct sockaddr *) & servaddr, sizeof(servaddr)) < 0) {
        perror("error on binding.");
        exit(1); }

    listen(listen_fd, 10);
    puts("Connecting...");

    socklen_t client_len = sizeof(cliaddr);

    while (1) {
        comm_fd = accept(listen_fd, (struct sockaddr *) & cliaddr, & client_len);

        if (comm_fd < 0) {
            perror("error on accept.");
            exit(1); }
        pid = fork();

        if (pid < 0) {
            perror("error on fork.");
            exit(1); }

        if (pid == 0) {
            // client process
            close(listen_fd);

            bzero(buffer, MAX_SIZE);
            n = read(comm_fd, buffer, MAX_SIZE - 1);

            if (n < 0) {
                perror("cannot read from socket.");
                exit(1); }

            printf("Here is your message: %s\n", buffer);

            n = write(comm_fd, "Capki, capki, xshsh... I got your message", 41);

            if (n < 0) {
                perror("cannot write to socket.");
                exit(1); }
            strncpy(bye, buffer, 3);
            bye[4] = '\0';

            if (strcmp(bye, "bye") == 0) {
                printf("OK, bye. Child exiting...\n");
                close(comm_fd); }
            exit(0); } else {
            close(comm_fd); }
    }
    return 0; }
