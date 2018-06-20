#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>
 
#define BUFFER_LENGTH  1024

#pragma comment(lib, "Ws2_32.lib")

main(argc, argv)  int argc; char *argv[];
{
     WSADATA              wsaData;
     int				  SendingSocket;
	 int                  countByteSent=0;
	 struct sockaddr_in   ReceiverAddr, SrcInfo;
	 struct hostent		  *hp;
     char                 SendBuf[BUFFER_LENGTH]="Bonjour";
	 char				  buf[BUFFER_LENGTH];
	 char                 fileName[BUFFER_LENGTH];
     int len;
     int TotalByteSent;
 
   
	 if (argc != 1)
	 {	fprintf(stderr, "%s has no command-line arguments\n", argv[0]);
	 exit(1);
	 }
	 WORD version = (1 << 8) + 1;  /* Version 1.1 de WINSOCK*/
	 //  Vérifie les données des sockets liés à l'ordinateur et initie le programme pour utiliser Windows Sockets DLL
	 if (WSAStartup(version, &wsaData) != 0)
	 {
		 perror("lors de l'initialisation du socket");
		 exit(1);
	 }
     
 
     // Création d'un socket utilisant le protocole UDP
     SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	 if (SendingSocket < 0)
	 {
		 perror("lors de l'ouverture du socket");
		 exit(1);
	 }

	 // Définition de la structure du socket récepteur
     ReceiverAddr.sin_family = AF_INET;
	 printf("Entrer le nom de la machine distante: ");
	 gets(buf);
	 hp = gethostbyname(buf);      //Établie le nom de l'appareil récepteur
	 if (hp == 0)
	 {
		 fprintf(stderr, "%s: machine inconnue\n", buf);
		 exit(1);
	 }
	 memcpy((char *)&ReceiverAddr.sin_addr, (char *)hp->h_addr, hp->h_length);  //Copie l'adresse du récepteur 
	 printf("Entrer le numero de port dans la machine distante: ");
	 gets(buf);
	 ReceiverAddr.sin_port = htons((short)atoi(buf)); //Établie le numéro du port avec le socket récepteur de l'aute appareil

	 //Début de l'envoie du fichier
	 FILE *file;
	 while (1)
	 {
		 printf("\nVeuillez entrer le nom du fichier: ");
		 scanf("%s", fileName);
		 file = fopen(fileName, "rb"); //Ouverture du fichier en mode lecture bytes
		 if (file != NULL) //Vérifie que le fichier s'est bien ouvert'
		 {
			 TotalByteSent = sendto(SendingSocket, fileName, sizeof(fileName), 0,
				 (struct sockaddr *)&ReceiverAddr, sizeof(ReceiverAddr)); //Envoie du nom du fichier au récepteur
			 printf("\nLe contenu du fichier sera imprime");
			 while (!feof(file)) //Lecture du fichier
			 {
				 fread(SendBuf, BUFFER_LENGTH, 1, file);
				 TotalByteSent = sendto(SendingSocket, SendBuf, sizeof(SendBuf), 0,
					 (struct sockaddr *)&ReceiverAddr, sizeof(ReceiverAddr)); //Envoie d'une partie des données au récepteur
				 printf("Nombre de bytes envoyes: %d\n", TotalByteSent);
				 Sleep(3);
			 } 
			 fclose(file);
			 break;
		 }
		 else//S'il y a eu un problème à l'ouverture du fichier
		 {
			 printf("\nLe nom de fichier est incorrect.");
		 }
		 
	 }

	 
     memset(&SrcInfo, 0, sizeof(SrcInfo));
     len = sizeof(SrcInfo);
 
	 getsockname(SendingSocket, (struct sockaddr *)&SrcInfo, &len);
     printf("Client: adresse  IP d'envoi: %s\n", inet_ntoa(SrcInfo.sin_addr));
     printf("Client: port utilise: %d\n", htons(SrcInfo.sin_port));
 
	 // Imprime et reçoie les information du socket récepteur
	 getpeername(SendingSocket, (struct sockaddr *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));
     printf("Client: adresse IP du serveur: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
     printf("Client: port du serveur: %d\n", htons(ReceiverAddr.sin_port));
 
	 // Fermeture du socket
	 closesocket(SendingSocket);

	 
	// Terminaison de Winsock
	WSACleanup();
	// Back to the system
	return 0;
}