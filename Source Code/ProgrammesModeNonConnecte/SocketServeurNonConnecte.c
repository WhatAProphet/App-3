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
     int			ReceivingSocket;
	 int			length;
	 struct sockaddr_in	ReceiverAddr;
	 char				ReceiveBuf[BUFFER_LENGTH];
	 struct sockaddr_in SenderAddr;
     int                SenderAddrSize = sizeof(SenderAddr);
	 int				ReceiverAddrSize = sizeof(ReceiverAddr);
     int                ByteReceived = 5;
 
  
	 if (argc > 2)
	 {
		 fprintf(stderr, "Usage: %s [portnumber]\n", argv[0]);
		 exit(1);
	 }
	 WORD version = (1 << 8) + 1;  /* Version 1.1 de WINSOCK*/

	 // ????????????????
	 if (WSAStartup(version, &wsaData) != 0)
	 {
		 perror("lors de l'initialisation winsock");
		 exit(1);
	 }
 
	 // ????????????????
	 if ((ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	 {
		 perror("lors de l'ouverture du socket");
		 exit(1);
	 }
	 printf("Socket cree\n");
	 // ????????????????
     ReceiverAddr.sin_family = AF_INET;
	 // ????????????????
	 ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 if (argc == 2)  /*?????????????????????*/
		 ReceiverAddr.sin_port = htons((short)atoi(argv[1]));  /*?????????????????????*/
	 else
		 ReceiverAddr.sin_port= 0;  /*?????????????????????*/
    
 
	 // ????????????????
	 if (bind(ReceivingSocket, (struct sockaddr  *)&ReceiverAddr, ReceiverAddrSize) <0)
		{ 
			perror("lors de l'association au numero de port");
			exit(1);
		}
	
	 // ????????????????
	 if (getsockname(ReceivingSocket, (struct sockaddr *)&ReceiverAddr, &ReceiverAddrSize)< 0) /*?????????????????????*/
		{
			perror("lors de l'interrogation sur le socket");
			exit(1);
		}

	 // ????????????????

   printf("Serveur: adresse IP utilisee: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
   printf("Serveur: port utilise %d\n", htons(ReceiverAddr.sin_port));
   printf("Server: pret a recevoir...\n");
 
   // ????????????????
   ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, BUFFER_LENGTH, 0,
				(struct sockaddr *)&SenderAddr, &SenderAddrSize);
 if ( ByteReceived > 0 )
	{
     printf("Serveur: Nombre de bytes recus: %d\n", ByteReceived);
     printf("Serveur: Les donnees sont \"%s\"\n", ReceiveBuf);
	}
 else 
    printf("Serveur: recvfrom() non reussi avec code: %d\n", WSAGetLastError());
 
 // ????????????????
 getpeername(ReceivingSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
 printf("Serveur: adresse IP du client: %s\n", inet_ntoa(SenderAddr.sin_addr));
 printf("Server: numero de port du client: %d\n", htons(SenderAddr.sin_port));

 // ????????????????
 printf("Serveur: Fini de recevoir...\n");
 closesocket(ReceivingSocket);

 // ????????????????

WSACleanup(); 
return 0;
}