#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void initializeSocket(int *);
void setSocketOpt(int *);
void bindSocket(int *, struct sockaddr * );
void listenOnSocket(int *, int);
void toLowerCase(char[], int);
void toUpperCase(char[], int); 
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
  
  initializeSocket(&serverFd);
  
  setSocketOpt(&serverFd);
  
  bindSocket(&serverFd,(struct sockaddr *)&serverAddr);
  
  listenOnSocket(&serverFd,10);
  
  printf("TCP Socket készen áll a kliensek fogadására.\n");
  fflush(stdout);
  
  //Két kliens fáljlleírója, sockaddr_in, illetve a válaszott nyelv.
  int firstClient;
  struct sockaddr_in firstClientAddr;
  int firstLanguage;
  char firstMessage[1023];
  char firstFeedback[255];
  
  int secondClient;
  struct sockaddr_in secondClientAddr;
  int secondLanguage;
  char secondMessage[1023];
  char secondFeedback[255];
  
  unsigned sin_size = sizeof(struct sockaddr_in);
  firstClient = accept(serverFd, (struct sockaddr *)&firstClientAddr, &sin_size);
  printf("Első résztvevő csatlakozott.");
  fflush(stdout);
  char serverMessage[255];
  strcpy(serverMessage, "Udvozlom a konferencian, kerem valasszon nyelvet.\n");
  
  toUpperCase(serverMessage,strlen(serverMessage));
  send(firstClient,serverMessage,255,0);
  
  toLowerCase(serverMessage,strlen(serverMessage));
  send(firstClient,serverMessage,255,0);
  
  char * choices ="1. MAGYAR\n2. magyar\n";
  send(firstClient,choices,255,0);
  char * messageFromClient;
  while(1) {
    recv(firstClient,messageFromClient,255,0);
    printf("%s",messageFromClient);
    fflush(stdout);
  }  
  
  close(secondClient);
  close(firstClient);
  close(serverFd);
  
}


void initializeSocket(int * sock) {
  
  if((*sock = socket(AF_INET,SOCK_STREAM,0)) == -1) {
    perror("Hiba a socket létrehozásakor");
    _exit(1);
  }
}

void setSocketOpt(int * sock) {
  
  int yes = 1;
  if(setsockopt(*sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
      perror("Hiba a socket beállítása közben");
      _exit(1);
  }
}

void bindSocket(int * sock, struct sockaddr * serverAddr) {
  
  if( bind(*sock,serverAddr,sizeof(struct sockaddr)) == -1) {
    perror("Hiba a socket bind-elése közben");
    _exit(1);
  }
}

void listenOnSocket(int * sock, int maxClients) {
  if(listen(*sock,maxClients) == -1) {
    perror("Hiba a listen alkalmazása közben");
    _exit(1);
  }
}

void toLowerCase(char * message, int n) {
  for(int i = 0; message[i]; i++) {
    message[i] = tolower(message[i]);
    //printf("%c",toupper(message[i]));
  }
}

void toUpperCase(char message[], int n) {
  for(int i = 0; message[i]; i++) {
    message[i] = toupper(message[i]);
  }
}