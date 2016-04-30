#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void initializeSocket(int *);
void setSocketOpt(int *);
void printChosenLanguageAndSendToServer(int,int);
void connectToServer(int *,struct sockaddr *);

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
  
  // Inicializáljuk a socketet.
  initializeSocket(&serverFd);
  // Beállítjuk a socket beállításait
  setSocketOpt(&serverFd);
  //Kapcsolódunk a szerverhez.
  connectToServer(&serverFd,(struct sockaddr *)&serverAddr);
  
  char uzenet[255];
  int choice = 0;
  
  while(1) {
    
    //Nagybetűs üdvözlés
    memset(uzenet,0,255);
    recv(serverFd,uzenet,255,0);
    printf("%s",uzenet);
    fflush(stdout);
    
    //Kisbetűs üdvözlés
    memset(uzenet,0,255);
    recv(serverFd,uzenet,255,0);
    printf("%s",uzenet);
    fflush(stdout);
    
    //Nyelvválasztás.
    memset(uzenet,0,255);
    recv(serverFd,uzenet,255,0);
    printf("%s",uzenet);
    fflush(stdout);
    do {
    scanf("%d",&choice);
    } while(choice !=1 && choice !=2);
    printChosenLanguageAndSendToServer(choice,serverFd);
  }
  
  close(serverFd);
  
}

void initializeSocket(int * sock) {
  
  if((*sock = socket(AF_INET,SOCK_STREAM,0)) == -1) {
    perror("Hiba a socket létrehozásakor!");
    _exit(1);
  }
}

void setSocketOpt(int * sock) {
  
  int yes = 1;
  if(setsockopt(*sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
      perror("Hiba a socket beállítása közben.");
      _exit(1);
  }
}

void printChosenLanguageAndSendToServer(int choice, int sock) {
  char msg[255];
  if(choice == 1) {
    printf("A VALASZOTT NYELVE:MAGYAR.\n");
    strcpy(msg,"NAGY");
  } else {
    printf("a valasztott nyelve:magyar.\n");
    strcpy(msg,"kicsi");
  }
  send(sock,msg,255,0);
}

void connectToServer(int * servSock,struct sockaddr * serverAddr){
  if(connect(*servSock,serverAddr,sizeof(struct sockaddr)) == -1) {
    perror("Nem sikerült a szervehez csatlakozni");
    _exit(1);
  }
}