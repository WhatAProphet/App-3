////////////////////////////////////////////////////////////////////////////////
//
//		Fichier:  socket client.c
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>

#define BUFFER_LENGTH  1025
#define TEXT_LENGH	   1025
#define END_OF_STRING  '\0'

#pragma comment(lib, "Ws2_32.lib")

main(argc, argv)  int argc; char *argv[];

{	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buf[BUFFER_LENGTH];
	int len;
	int rval;
	char text[TEXT_LENGH]="Bonjour";

	WSADATA wsadata;
	WORD version = (1 << 8) + 1;  /* Version 1.1 */
	LPWSADATA lpwsadata = 0;


	if (argc != 1)
	{	fprintf(stderr,"%s manque d'arguments\n", argv[0]);
		exit(1);
	}

	/*  A faire: initialisation de Winsock */

	WSAStartup(version, lpwsadata);

	/*  A faire: Création d'un socket de communication  */
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(s, &server, TEXT_LENGH);
	
	server.sin_family = AF_INET;  
	printf("Entrer le nom de la machine distante: "); 
	gets_s(buf, BUFFER_LENGTH);
	hp = gethostbyname(buf);      
	if (hp == 0)
	{	fprintf(stderr, "%s: machine inconnue\n", buf);
		exit(1);
	}
	memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);  
	printf("Entrer le numero de port dans la machine distante: ");   
	gets_s(buf, BUFFER_LENGTH);
	server.sin_port = htons((short) atoi(buf)); 
	
    /*  A faire: connection au serveur  */

	connect(s, &server, TEXT_LENGH);
	
	len = strlen(text);
	if (len == 0)  {
	if (0)
		{	perror("lors de l'ecriture dans le socket");
			exit(1);
		}
	}

	/*  A faire: Fermeture de socket  */
	closesocket(s);
	/*  A faire: Terminaison de winsock  */
	WSACleanup();
	return 0;
}  
