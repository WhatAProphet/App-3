#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>


#define BUFFER_LENGTH  1024

#pragma comment(lib, "Ws2_32.lib")


int main(argc, argv)
int argc;
char *argv[];
{
     WSADATA		wsaData;
	 char           *answer = "Server got data";
     int			ReceivingSocket;
	 int			length;
	 struct sockaddr_in	ReceiverAddr;
	 char				ReceiveBuf[BUFFER_LENGTH];
	 struct sockaddr_in SenderAddr;
     int                SenderAddrSize = sizeof(SenderAddr);
	 int				ReceiverAddrSize = sizeof(ReceiverAddr);
     int                ByteReceived = 5;
	 int                byteReceivedCount=0;
 
  
	 if (argc > 2)
	 {
		 fprintf(stderr, "Usage: %s [portnumber]\n", argv[0]);
		 exit(1);
	 }
	 WORD version = (1 << 8) + 1;  /* Version 1.1 de WINSOCK*/

	 // Initialisation de Winsock
	 if (WSAStartup(version, &wsaData) != 0)
	 {
		 perror("lors de l'initialisation winsock");
		 exit(1);
	 }
 
	 // On v�rifie si l'ouverture du socket est un succ�s
	 if ((ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	 {
		 perror("lors de l'ouverture du socket");
		 exit(1);
	 }
	 printf("Socket cree\n");
	 // D�finition du type d'adresse : IPv4
     ReceiverAddr.sin_family = AF_INET;
	 // Assigne l'adresse IP de la machine utilis�e pour le socket du serveur
	 ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 if (argc == 2)  
		 ReceiverAddr.sin_port = htons((short)atoi(argv[1]));  //Assigne le port disponible
	 else
		 ReceiverAddr.sin_port= 0;  //On n'assigne pas de port
    
 
	 // Liaison du socket � l'adresse IPv4
	 if (bind(ReceivingSocket, (struct sockaddr  *)&ReceiverAddr, ReceiverAddrSize) <0)
		{ 
			perror("lors de l'association au numero de port");
			exit(1);
		}
	
	 // On assure la validit� du socket dans le serveur
	 if (getsockname(ReceivingSocket, (struct sockaddr *)&ReceiverAddr, &ReceiverAddrSize)< 0) //On assure d'avoir une r�ponse du socket
		{
			perror("lors de l'interrogation sur le socket");
			exit(1);
		}

	 // Impression � l'�cran des informations obtenues pr�c�demment

   printf("Serveur: adresse IP utilisee: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
   printf("Serveur: port utilise %d\n", htons(ReceiverAddr.sin_port));
   printf("Server: pret a recevoir...\n");
 
   // Pr�paration � la r�ception d'un fichier

   FILE *fptr;
   ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, sizeof(ReceiveBuf), 0,
	   (struct sockaddr *)&SenderAddr, &SenderAddrSize); //Les premiers bytes re�ues seront le nom du fichier qui sera transferr�
   fptr = fopen(ReceiveBuf, "wb"); //Ouverture du fichier en mode: �criture bytes
   printf("Fichier cree au nom de: %s\n", ReceiveBuf);

   //R�ception des donn�es qui seront envoy�es
   while (ByteReceived > 0)
   {
	   ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, sizeof(ReceiveBuf), 0,
		   (struct sockaddr *)&SenderAddr, &SenderAddrSize); //R�ception des bytes
	   byteReceivedCount += ByteReceived;
	   if (ByteReceived > 0) //Lorsque des bytes auront �t� re�us
	   {
		   printf("Serveur: Nombre de bytes recus: %d\n", ByteReceived);
		   fwrite(ReceiveBuf, BUFFER_LENGTH, 1, fptr);
		   Sleep(3);
		   //sendto(ReceivingSocket, (const char *)answer, strlen(answer), 0, (struct sockaddr *)&SenderAddr, SenderAddrSize);
	   }
	   else if (ByteReceived == 0) //Lorsqu'il n'y a plus de bytes � recevoir
	   {
		   printf("Connection done");
	   }
	   else //En cas d'erreur lors de la r�ception
		   printf("Serveur: recvfrom() non reussi avec code: %d\n", WSAGetLastError());
   }
   
   fclose(fptr);
   
 
 // Re�oie et imprime les informations du client
 getpeername(ReceivingSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
 printf("Serveur: adresse IP du client: %s\n", inet_ntoa(SenderAddr.sin_addr));
 printf("Server: numero de port du client: %d\n", htons(SenderAddr.sin_port));
 printf("Server: Bytes received: %d\n", byteReceivedCount);

 // Fermeture du socket
 printf("Serveur: Fini de recevoir...\n");
 closesocket(ReceivingSocket);

 // Met fin � l'utilisation de Winsock

WSACleanup(); 
return 0;
}