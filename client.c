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
int parseOrder(char[]);
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
  int sorrend = 0;
    
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
    
    recv(serverFd,uzenet,255,0);
    sorrend = parseOrder(uzenet);
    printf("%d vagyok a felszolalasban.\n",sorrend);
    fflush(stdout);
    
  int round = 1;
  char lenyeg[255];
  while(1) {  
    
    memset(lenyeg,0,255);
    read = recv(serverFd,lenyeg,255,0);
    lenyeg[read] = '\0';
    printf("%s",lenyeg);
    //printf("Kor:%d \nSorrend:%d\n",round,sorrend);
    printf("round maradekos sorrend:%d\n",round % sorrend);
    //Felszolalas kuldese.
    if(round % sorrend == 0) {
      memset(message,0,1023);
      printf("Szolaljon fel kerem:");
      gets(message);
      printf("Felszolalas:%s\n",message);
      send(serverFd,message,1023,0);
    } else if (round % sorrend == 1) { //vélemény küldése
      memset(feedback,0,255);
      printf("Mondjon velemenyt kerem:");
      gets(feedback);
      printf("Velemeny:%s\n",feedback);
      send(serverFd,feedback,255,0);
    }
    
    round++;
    
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

int parseOrder(char msg[]) {
  if(strcmp(msg,"1") == 0) 
    return 1;
  if(strcmp(msg,"2") == 0) 
    return 2;
}
