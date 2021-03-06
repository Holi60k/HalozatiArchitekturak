#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#define MESSAGE_LIMIT 1023 + 1 
#define FEEDBACK_LIMIT 255 + 1

void initializeSocket(int *);
void setSocketOpt(int *);
void printChosenLanguageAndSendToServer(char[], int);
void connectToServer(int *, struct sockaddr *);
int parseOrder(char[]);
int main() {

	/**
	 * A célszerver címét leíró dolgokat beállítjuk.
	 */
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET; //cím család
	serverAddr.sin_port = htons(2556);//2556 os porton futunk
	//serverAddr.sin_addr.s_addr = INADDR_ANY;
	inet_aton("127.0.0.1", &serverAddr.sin_addr);// ip cím
	memset(&(serverAddr.sin_zero), 0, 8);//kitöltő
	int serverFd;

	// Inicializáljuk a socketet.
	initializeSocket(&serverFd);
	// Beállítjuk a socket beállításait
	setSocketOpt(&serverFd);
	//Kapcsolódunk a szerverhez.
	connectToServer(&serverFd, (struct sockaddr *) &serverAddr);

	char uzenet[255];
	char choice[255];
	int error = 0;
	char message[MESSAGE_LIMIT];
	char feedback[FEEDBACK_LIMIT];
	int read = 0;
	int sorrend = 0;

//	//Nagybetűs üdvözlés
//	memset(uzenet, 0, 255);
//	recv(serverFd, uzenet, 255, 0);
//	printf("%s", uzenet);
//
//	//Kisbetűs üdvözlés
//	memset(uzenet, 0, 255);
//	recv(serverFd, uzenet, 255, 0);
//	printf("%s", uzenet);

//Nyelvválasztás.
//	memset(uzenet, 0, 255);
//	recv(serverFd, uzenet, 255, 0);
//	printf("%s", uzenet);

	printf("VALASZTHATO NYELVEK/valaszthato nyelvek:MAGYAR - magyar\n");
	printf("KEREM VALASSZON NYELVET/kerem valasszon nyelvet:");
	do {
		if(error > 0)
			printf("Valasszon ujra:");
		gets(choice);
		//printf("M:%d m:%d %s",strcmp(choice,"MAGYAR"),strcmp(choice,"magyar"),choice);
		error++;
	} while (strcmp(choice,"MAGYAR") != 0 && strcmp(choice,"magyar") != 0);
	printChosenLanguageAndSendToServer(choice, serverFd);

	recv(serverFd, uzenet, 255, 0);

	//Megadja hogy hanyadikként írhatunk.
	sorrend = parseOrder(uzenet);
	fflush(stdout);

	char lenyeg[255];
	memset(lenyeg, 0, 255);
	read = recv(serverFd, lenyeg, 255, 0);
	char vote[255], anotherClientVote[255];
	//msgS = message sent, (felszólalás elküldte)
	//feedbS = feedback sent, (vélemény elküldve)
	int msgS = 0, feedbS = 0;
	for (;;) {

		//Felszolalas kuldese.
		if (sorrend % 2 == 1) {
			//-----------------------------------
			// Felszolalás küldése
			//-----------------------------------
			memset(message, 0, MESSAGE_LIMIT);
			memset(uzenet, 0, 255);
			read = recv(serverFd, uzenet, 255, 0);
			printf("%s",uzenet);
			fflush(stdout);
			fgets(message, MESSAGE_LIMIT, stdin);
			send(serverFd, message, MESSAGE_LIMIT, 0);
			//-----------------------------------
			// Partner véleményének fogadása
			//-----------------------------------
			memset(feedback, 0, FEEDBACK_LIMIT);
			read = recv(serverFd, feedback, FEEDBACK_LIMIT, 0);
			printf("%s", feedback);
			fflush(stdout);
			msgS = 1;
			//-----------------------------------

		} else if (sorrend % 2 == 0) { //vélemény küldése
			//-------------------------------------------
			// Partner felszólalásának fogadása.
			//-------------------------------------------
			memset(message, 0, MESSAGE_LIMIT);
			read = recv(serverFd, message, MESSAGE_LIMIT, 0);
			printf("%s", message);
			fflush(stdout);
			//-------------------------------------------
			// Vélemény küldése
			//-------------------------------------------
			memset(feedback, 0, FEEDBACK_LIMIT);
			memset(uzenet, 0, 255);
			read = recv(serverFd, uzenet, 255, 0);
			printf("%s",uzenet);
			fgets(feedback, FEEDBACK_LIMIT, stdin);
			send(serverFd, feedback, FEEDBACK_LIMIT, 0);
			fflush(stdout);
			feedbS = 1;
			//-------------------------------------------
		}
		// Szavazás megfigyelése, akkor ha már volt egy felszólalás illetve vélemény küldés mind a két oldalon.
		if (feedbS == 1 && msgS == 1) {
			memset(lenyeg, 0, 255);
			read = recv(serverFd, lenyeg, 255, 0);
			printf("%s", lenyeg);
			fgets(vote, 255, stdin);
			send(serverFd,vote,255,0);
			recv(serverFd,anotherClientVote,255,0);
			memset(uzenet, 0, 255);
			read = recv(serverFd, uzenet, 255, 0);
			if(strcmp(uzenet,"vege") == 0)
				break;
			feedbS = 0;
			msgS = 0;
		}

		sorrend++;

	}

	close(serverFd);

}
/**
* Socket inicializáló függvény
*/
void initializeSocket(int * sock) {

	if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Hiba a socket létrehozásakor!");
		_exit(1);
	}
}
/**
* Socket beállítások beállítása
*/
void setSocketOpt(int * sock) {

	int yes = 1;
	if (setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("Hiba a socket beállítása közben.");
		_exit(1);
	}
}
/**
* Válaszott nyelv küldése a szervernek illetve kiíratása a standard kimenetre.
*/
void printChosenLanguageAndSendToServer(char choice[], int sock) {
	char msg[255];
	memset(msg, 0, 255);
	if (strcmp(choice, "MAGYAR") == 0) {
		printf("A VALASZOTT NYELVE:MAGYAR.\n");
		strcpy(msg, "NAGY");
	} else if (strcmp(choice, "magyar") == 0) {
		printf("a valasztott nyelve:magyar.\n");
		strcpy(msg, "kicsi");
	}
	send(sock, msg, 255, 0);
}
/**
* Szerver való csatlakozás socketen keresztül.
*/
void connectToServer(int * servSock, struct sockaddr * serverAddr) {
	if (connect(*servSock, serverAddr, sizeof(struct sockaddr)) == -1) {
		perror("Nem sikerült a szervehez csatlakozni");
		_exit(1);
	}
}

/**
* Sorrend megállapítása
*/
int parseOrder(char msg[]) {
	if (strcmp(msg, "1") == 0)
		return 1;
	if (strcmp(msg, "2") == 0)
		return 2;
}
