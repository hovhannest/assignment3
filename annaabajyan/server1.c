#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>

void str_echo(int s){
char buf[50];
//receiving data from client
recv(s,buf,50,0);
puts("Message from Client...");
fputs(buf,stdout);
send(s,buf,50,0);
}
int main(){
int ls,cs,len;
struct sockaddr_in serv, cli;
pid_t pid;
puts("I am Server...");
//creating socket
ls = socket(AF_INET,SOCK_STREAM,0);
puts("Socket Created Successfully..");
//socket address structure
serv.sin_family = AF_INET;
serv.sin_addr.s_addr = INADDR_ANY;
serv.sin_port = htons(5000);

bind(ls,(struct sockaddr*) &serv, sizeof(serv));
puts("Binding Done...");

listen(ls,3);
puts("Listening for Client...");

for(;;){
len = sizeof(cli);
//accepting client connection
cs = accept(ls,(struct sockaddr*) &cli,&len);
puts("\nConnected to Client...");
//creating child process
if((pid = fork()) == 0){
puts("Child process created...");
close(ls);
str_echo(cs);
close(cs);
exit(0);
}
close(cs);
}
return 0;
}

