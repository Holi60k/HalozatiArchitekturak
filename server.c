#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <boost/concept_check.hpp>

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
  
  //Szerver fájlleírója
  int serverFd;
  
  serverFd = socket(AF_INET,SOCK_STREAM,0);
  int yes = 1;
  setsockopt(serverFd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
  bind(serverFd,(struct sockaddr *)&serverAddr,sizeof(struct sockaddr));
  
  listen(serverFd,10);
  
  //Két kliens fáljlleírója, sockaddr_in, illetve a válaszott nyelv.
  int firstClient;
  struct  sockaddr_in firstClientAddr;
  int firstLanguage;
  
  
  int secondClient;
  struct  sockaddr_in secondClientAddr;
  int secondLanguage;
  
  unsigned sin_size = sizeof(struct sockaddr_in);
  firstClient = accept(serverFd, (struct sockaddr *)&firstClientAddr, &sin_size);
  
  
}