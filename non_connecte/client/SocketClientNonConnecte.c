#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>
#include <time.h>
 
#define BUFFER_LENGTH  1024

#pragma comment(lib, "Ws2_32.lib")

void FileSend(SOCKET FileSendSocket, char *FilePath);
void delay(int milliseconds);

main(argc, argv)  int argc; char *argv[];
{
     WSADATA              wsaData;
     int				  SendingSocket;
	 int                  countByteSent=0;
	 struct sockaddr_in   ReceiverAddr, SrcInfo;
	 struct hostent		  *hp;
     char                 *SendBuf = "Bonjour!Terre appelle Mars!";
	 char				  buf[BUFFER_LENGTH];
	 char                 *filePath = "test.txt";
     int len;
     int TotalByteSent;
 
   
	 if (argc != 1)
	 {	fprintf(stderr, "%s has no command-line arguments\n", argv[0]);
	 exit(1);
	 }
	 WORD version = (1 << 8) + 1;  /* Version 1.1 de WINSOCK*/
	 /*  ???? Indiquer quelle opération on effectue ici????? */
	 if (WSAStartup(version, &wsaData) != 0)
	 {
		 perror("lors de l'initialisation du socket");
		 exit(1);
	 }
     
 
     // ?????????????????????????????????????????????
     SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	 if (SendingSocket < 0)
	 {
		 perror("lors de l'ouverture du socket");
		 exit(1);
	 }

	 // ?????????????????????????????????????????????
     ReceiverAddr.sin_family = AF_INET;
	 printf("Entrer le nom de la machine distante: ");
	 gets(buf);
	 hp = gethostbyname(buf);      /*?????????????????????*/
	 if (hp == 0)
	 {
		 fprintf(stderr, "%s: machine inconnue\n", buf);
		 exit(1);
	 }
	 memcpy((char *)&ReceiverAddr.sin_addr, (char *)hp->h_addr, hp->h_length);  /*?????????????????????*/
	 printf("Entrer le numero de port dans la machine distante: ");
	 gets(buf);
	 ReceiverAddr.sin_port = htons((short)atoi(buf)); /*?????????????????????*/

	 char fileName[1024], fileContent[1024];
	 FILE *file;
	 int n;
	 while (1)
	 {
		 printf("\nVeuillez entrer le nom du fichier: ");
		 scanf("%s", fileName);
		 file = fopen(fileName, "r");
		 if (file != NULL)
		 {
			 printf("\nLe contenu du fichier sera imprime");
			 while (!feof(file)) {
				 fgets(fileContent, 1024, file);
				 //printf("%s\n", fileContent);
				 TotalByteSent = sendto(SendingSocket, fileContent, strlen(fileContent), 0,
					 (struct sockaddr *)&ReceiverAddr, sizeof(ReceiverAddr));
				 countByteSent += TotalByteSent;
				 n = recvfrom(SendingSocket, (char *)SendBuf, BUFFER_LENGTH, 1,
					 (struct sockaddr *)&ReceiverAddr, &len);
				 SendBuf[n] = '\0';
			 }
			 fclose(file);
			 break;
		 }
		 else
		 {
			 printf("\nLe nom de fichier est incorrect.");
		 }
		 
	 }
	 
 
	 // ?????????????????????????????????????????????
     /*printf("Client: les donnees a envoyer : \"%s\"\n", SendBuf);
     printf("Client: entrain d'envoyer...\n");
	 TotalByteSent = sendto(SendingSocket, SendBuf, BUFFER_LENGTH, 0,
		 (struct sockaddr *)&ReceiverAddr, sizeof(ReceiverAddr))*/;

 
	 // ?????????????????????????????????????????????
     memset(&SrcInfo, 0, sizeof(SrcInfo));
     len = sizeof(SrcInfo);
 
	 getsockname(SendingSocket, (struct sockaddr *)&SrcInfo, &len);
     printf("Client: adresse  IP d'envoi: %s\n", inet_ntoa(SrcInfo.sin_addr));
     printf("Client: port utilise: %d\n", htons(SrcInfo.sin_port));
 
	 // ?????????????????????????????????????????????
	 getpeername(SendingSocket, (struct sockaddr *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));
     printf("Client: adresse IP du serveur: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
     printf("Client: port du serveur: %d\n", htons(ReceiverAddr.sin_port));
     printf("Client: total de bytes envoyes: %d\n", countByteSent);
 
	 // ?????????????????????????????????????????????
	
	 closesocket(SendingSocket);
	// ?????????????????????????????????????????????
	WSACleanup();
	// Back to the system
	return 0;
}

void FileSend(SOCKET FileSendSocket, char *FilePath)
{
	FILE *fptr;
	int sendbuflen = 1024;
	char *sendbuf[1024];
	fptr = fopen_s(&fptr, FilePath, "r");
	ZeroMemory(&sendbuf, sendbuflen);

	if (fptr != NULL)
	{
		while (1)
		{
			fscanf_s(fptr, "%s", sendbuf);
			if (sendbuf == EOF)
			{
				printf("End of File sending from Client\n");
				fclose(fptr);
				break;
			}
			else
			{
				send(FileSendSocket, sendbuf, sendbuflen, 0);
				ZeroMemory(&sendbuf, sendbuflen);
			}
		}
	}

}

void delay(int milliseconds)
{
	long pause;
	clock_t now, then;

	pause = milliseconds * (CLOCKS_PER_SEC / 1000);
	now = then = clock();
	while ((now - then) < pause)
		now = clock();
}