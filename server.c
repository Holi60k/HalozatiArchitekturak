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
void toLowerCase(char[]);
void toUpperCase(char[]); 
void setChosenLanguage(char*, int *);
void sendMessageToSocket(char *, int);
void translateMessageForClient(int, char[]);

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
  
  char  messageFromClient[255];
  char serverMessage[255];
  strcpy(serverMessage, "Udvozlom a konferencian, kerem valasszon nyelvet.\n");
  char * choices ="1. MAGYAR\n2. magyar\n";
  
  unsigned sin_size = sizeof(struct sockaddr_in);
  
  //Várakozunk az első kliens kapcsolódására.
  firstClient = accept(serverFd, (struct sockaddr *)&firstClientAddr, &sin_size);
  printf("Első résztvevő csatlakozott.\n");
  
  fflush(stdout);
  
  //Üdvözlő szövegek elküldése.
  toUpperCase(serverMessage);
  send(firstClient,serverMessage,255,0);
  
  toLowerCase(serverMessage);
  send(firstClient,serverMessage,255,0);
  
  send(firstClient,choices,strlen(choices),0);
  //Választott nyelv beállítása.
  recv(firstClient,messageFromClient,255,0);
  setChosenLanguage(messageFromClient,&firstLanguage);
  
    
  
  //Várakozunk a második kliens csatlakozására.
  memset(serverMessage,0,255);
  secondClient = accept(serverFd, (struct sockaddr *)&secondClientAddr, &sin_size);
  printf("Második résztvevő csatlakozott.\n");
  strcpy(serverMessage, "Udvozlom a konferencian, kerem valasszon nyelvet.\n");
  
  //Üdvözlő szövegek elküldése.
  toUpperCase(serverMessage);
  send(secondClient,serverMessage,255,0);
  
  toLowerCase(serverMessage);
  send(secondClient,serverMessage,255,0);
  
  send(secondClient,choices,strlen(choices),0);
  
  recv(secondClient,messageFromClient,255,0);
  setChosenLanguage(messageFromClient,&secondLanguage);
  
  //memset(serverMessage,0,255);
  strcpy(serverMessage,"A második resztvevo is csatlakozott.\nKezodhet a konferencia.\nOn az elso felszolalo.\nKerem mondja el a javaslatat:\n");
  translateMessageForClient(firstLanguage,serverMessage);
  sendMessageToSocket(serverMessage,firstClient);
  
  memset(firstMessage,0,1023);
  memset(secondMessage,0,1023);
  memset(firstFeedback,0,255);
  memset(secondFeedback,0,255);
  int received = 0;
  //while(1) {
    
    
    
    //Első felszólal - második reagál.
    received = recv(firstClient,firstMessage,1023,0);
    firstMessage[received] = '\0';
    printf("Elso kliens felszolalasa:%s\n",firstMessage);
    fflush(stdout);
    //translateMessageForClient(secondLanguage,firstMessage);
    send(secondClient,firstMessage,1023,0);
    
    recv(secondClient,secondFeedback,255,0);
    printf("Masodik kliens velemenye:%s\n",secondFeedback);
    //translateMessageForClient(firstLanguage,secondFeedback);
    send(firstClient,secondFeedback,255,0);
    
    
    //Második felszólal - első reagál.
    recv(secondClient,secondMessage,1023,0);
    printf("Masodik kliens felszolalasa:%s\n",secondMessage);
    //translateMessageForClient(firstLanguage,secondMessage);
    send(firstClient,secondMessage,1023,0);
    recv(firstClient,firstFeedback,255,0);
    printf("Elso kliens velemenye:%s\n",firstFeedback);
   // translateMessageForClient(secondLanguage,firstFeedback);
    send(secondClient,firstFeedback,255,0);
       
  //}
  
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

void toLowerCase(char * message) {
  int i;
  for(i = 0; i < strlen(message); i++) {
    message[i] = tolower(message[i]);
    //printf("%c,",message[i]);
  }
}

void toUpperCase(char message[]) {
  int i;
  for(i = 0; i < strlen(message); i++) {
    message[i] = toupper(message[i]);
  }
}

void setChosenLanguage(char* choice, int * language) {
  if(strcmp(choice,"kicsi") == 0) {
    *language = 2;
  } else if(strcmp(choice,"NAGY") == 0) {
    *language = 1;
  }
}

void sendMessageToSocket(char msg[], int sock) {
   send(sock,msg,255,0);
}

void translateMessageForClient(int language, char msg[]) {
  if(language == 1) {
    toUpperCase(msg);
  } else if (language == 2) {
    toLowerCase(msg);
  }
}
