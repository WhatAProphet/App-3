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
#define TEXT_LENGHT	   1025
#define END_OF_STRING  '\0'

#pragma comment(lib, "Ws2_32.lib")

main(argc, argv)  int argc; char *argv[];

{	int sock;
	FILE *data_write = 0;
	FILE *data_read = 0;
	struct sockaddr_in server;
	struct hostent *hp;
	char buf[BUFFER_LENGTH];
	char filename[TEXT_LENGHT];
	int len;
	int rval;
	int n = 0;
	char text[TEXT_LENGHT]="Bonjour";

	WSADATA wsadata;
	WORD version = (1 << 8) + 1;  /* Version 1.1 */
	
	
	
	
	if (argc != 1)
	{	fprintf(stderr,"%s manque d'arguments\n", argv[0]);
		exit(1);
	}

	printf("Entrer le nom du fichier a envoyer: ");
	gets_s(filename, TEXT_LENGHT);
	

	/*  A faire: initialisation de Winsock */
	
	rval = WSAStartup(version, &wsadata);

	if (rval != 0) {
		perror("Erreur initialisation de Winsock");
		exit(1);
	}
	/*  A faire: Création d'un socket de communication  */
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);

	rval = bind(s, (struct sockaddr *)&server, TEXT_LENGHT);

	if (rval == 0) {
		perror("Erreur initialisation du socket");
	}
	
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

	connect(s, (struct sockaddr *)&server, sizeof(server));
	
	len = strlen(text);
	if (len == 0)  {
	if (s == 0)
		{	perror("lors de l'ecriture dans le socket");
			exit(1);
		}
	}
	send(s, filename, sizeof(filename),0);

	data_read = fopen(filename, "r+");
	if (data_read == NULL) {
		perror("Erreur lors de la lecture du fichier");
	}
	while (!feof(data_read)){
		fgets(text, TEXT_LENGHT, data_read);
	}
	
	printf("\nText sent = %s\n", text);
	send(s, text, sizeof(text), 0);
	fclose(data_read);

	
	


	/*  A faire: Fermeture de socket  */
	closesocket(s);
	
	/*  A faire: Terminaison de winsock  */
	WSACleanup();
	return 0;
}  
