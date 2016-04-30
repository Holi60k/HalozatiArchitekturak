#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

void initializeSocket(int *);
void setSocketOpt(int *);
void printChosenLanguageAndSendToServer(int,int);
void connectToServer(int *,struct sockaddr *);
void toLowerCase(char[]);
void toUpperCase(char[]); 

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
  int error = 0;
  char message[1023];
  char feedback[255];
  int read = 0;
  
    
    //Nagybetűs üdvözlés
    memset(uzenet,0,255);
    recv(serverFd,uzenet,255,0);
    printf("%s",uzenet);
    
    
    //Kisbetűs üdvözlés
    memset(uzenet,0,255);
    recv(serverFd,uzenet,255,0);
    printf("%s",uzenet);
    
    
    //Nyelvválasztás.
    memset(uzenet,0,255);
    recv(serverFd,uzenet,255,0);
    printf("%s",uzenet);
    
    do {
    //if(error >0)
      //printf("Valasszon ujra:");
    scanf("%d",&choice);
    //error++;
    } while(choice !=1 && choice !=2);
    printChosenLanguageAndSendToServer(choice,serverFd);
    fflush(stdout);
    
  while(1) {  
    memset(uzenet,0,255);
    read = recv(serverFd,uzenet,255,0);
    uzenet[read] = '\0';
    //translateMessageFromServer(choice,uzenet);
    printf("%s",uzenet);
    
    
    memset(message,0,1023);
    strcpy(message,"Hello from the other side!");
    //gets(message);
    printf("Hossza:%d\nFelszolalas üzenete:%s\n",strlen(message),message);
    send(serverFd,message,strlen(message),0);
    
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
  memset(msg,0,255);
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
    printf("%c,",message[i]);
  }
}

