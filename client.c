#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
  
  /**
   * A szerver címét leíró dolgokat beállítjuk.
   */
  struct  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(2556);
  //serverAddr.sin_addr.s_addr = INADDR_ANY;
  inet_aton("127.0.0.1",&serverAddr.sin_addr);
  memset(&(serverAddr.sin_zero),0,8);
  
  int serverFd;
  serverFd = socket(AF_INET,SOCK_STREAM,0);
  int yes = 1;
  setsockopt(serverFd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
  
  connect(serverFd,(struct sockaddr *)&serverAddr,sizeof(struct sockaddr));
  
  char uzenet[100];
  while(1) {
    
    
    memset(uzenet,0,100);
    printf("Mit mondjak:");
    scanf("%s",&uzenet);
    send(serverFd,uzenet,100,0);
    if(strcmp(uzenet,"vege\n") == 0) break;
    
    memset(uzenet,0,100);
    recv(serverFd,uzenet,100,0);
    printf("%s",uzenet);
    
  }
  
  close(serverFd);
  
}