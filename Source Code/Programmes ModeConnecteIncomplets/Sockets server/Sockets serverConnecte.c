////////////////////////////////////////////////////////////////////////////////
//
//		Fichier:  Sockets server.c
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>

#define BUFFER_LENGTH  1024
#define END_OF_STRING  '\0'

#pragma comment(lib, "Ws2_32.lib")

main(argc, argv)
int argc;
char *argv[];

{	int sock, msgsock;
	int length;
	int rval;
	char buf[BUFFER_LENGTH];
	char content[BUFFER_LENGTH];
	struct sockaddr_in server;
	struct sockaddr_in client;
	struct hostent *cname;
	int port;

	WSADATA wsadata;
	WORD version = (1 << 8) + 1;  /* Version 1.1 de WINSOCK*/

	FILE* data = 0;
	

	if (argc > 2)
	{	fprintf(stderr,"Usage: %s [portnumber]\n", argv[0]);
		exit(1);
	}


	/*  ???? Indiquer quelle op�ration on effectue ici????? */
	//initialisation de Winsock

	if (WSAStartup(version, &wsadata) != 0)    
	{	perror("lors de l'initialisation winsock");
		exit(1);
	}
	
	/*  ???? Indiquer quelle op�ration on effectue ici????? */
	//On v�rifie que l'ouverture du socket a �t� un succ�s
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{	perror("lors de l'ouverture du socket");
		exit(1);
	}


	server.sin_family = AF_INET;    /*?????????????????????*/ //On definit le type de connection/adresse, soit IPv4
	server.sin_addr.s_addr = INADDR_ANY; /*?????????????????????*/ //Assigne l'addresse IP de la machine utilis� pour le socket du serveur
	if (argc == 2)  /*?????????????????????*/
		port = htons((short) atoi(argv[1]));  /*?????????????????????*/ //Assigne le num�ro de port disponible
	else
		port = 0;  /*?????????????????????*/
	server.sin_port = port; /*?????????????????????*/
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) /*?????????????????????*/ //Lie le socket � l'addresse IP
	{	perror("lors de l'association au numero de port");
		exit(1);
	}

	/*  ???? Indiquer quelle op�ration on effectue ici????? */
	//On s'assure de la validit� du socket dans le serveur
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length) < 0) /*?????????????????????*/ // On s'assure d'avoir un r�ponse du socket
	{	perror("lors de l'interrogation sur le socket");
		exit(1);
	}
	printf("Le port %d a ete associe au socket\n", ntohs(server.sin_port));

	/*  ???? Indiquer quelle op�ration on effectue ici????? */
	listen(sock, 5); // On met le socket en mode �coute pour la r�ception de donn�es

	/*  ???? Indiquer quelles op�rations on effectue ici????? */
	// On part une boucle pour la r�ception de donn�es
	while (1)  /*?????????????????????*/ // Boucle infinie
	{	length = sizeof(client);
		msgsock = accept(sock, (struct sockaddr *) &client, &length); /*?????????????????????*/ //Accepte la reception de donn�es
		cname = gethostbyaddr((char *) &client.sin_addr, 4, AF_INET); /*?????????????????????*/ //Retourne le nom de la machine connect�e au port
		if (cname == NULL)
			printf("Connection d'une machine inconnue (%s), ",
				inet_ntoa(client.sin_addr));
		else
			printf("Connection de la machine %s (%s), ", cname->h_name, /*?????????????????????*/ // Envoie au client la confirmation de la connection
				inet_ntoa(client.sin_addr));
		printf("au port %d\n", client.sin_port); /*?????????????????????*/ //indique le port au client sur lequel il est connect�

		if (msgsock < 0)
		{	perror("lors de l'acceptation"); /*?????????????????????*/ //Renvoie un message d'erreur si mauvaise connection
		}

		/*  ???? Indiquer quelle op�ration on effectue ici????? */
		else do  /*?????????????????????*/ //recoit des donn�es jusqu'� ce qu'il en ait plus � transmettre
		{	
			printf("Reception de donn�es...\n");
			
			if ((rval = recv(msgsock, buf, sizeof(buf), 0)) < 0)
			{	perror("lors de la lecture du message");
			}
			if (rval == 0)
				printf("Fin de connection\n\n");
			else
			{	
				//buf[rval-1] = END_OF_STRING;
				printf("Nom du fichier recu: '%s'\n", buf);
				data = fopen(buf, "wb");//Ouvre un fichier avec le nom recu

				while (rval != 0){
					rval = recv(msgsock, content, sizeof(content), 0);
					printf("Nobre de bytes envoyes: %d\n", rval);
					fwrite(content, BUFFER_LENGTH, 1, data);
					Sleep(3);
			}
				
				
				printf("Contenu du fichier recu: '%s'\n", content);//Imprime le message recu � l'�cran
				
				//fprintf(data, "%s", content);//imprime le message dans le fichier
				
				printf("Entrer les caracteres+retour de chariot pour envoyer, ou juste retour de chariot pour terminer:\n ");
				gets_s(buf, BUFFER_LENGTH);
				
				length = strlen(buf);
				
				if (length == 0) {
					rval = length;//Permet de sortir du do while lorsqu'il n'y a plus de donn�es recues
				}
				if (send(msgsock, buf, length, 0) < 0) /*?????????????????????*/ //renvoie une erreur si le socket ne repond pas
					{	perror("lors de l'ecriture dans le socket");
						exit(1);
					}
				
			}
			
		} while (rval != 0);
		break;//sort de la boucle infinie afin de terminer le programme
	} 
	
	printf("Fin de connection\n");
	fclose(data);
	closesocket(sock); /*?????????????????????*/ //ferme le socket
	WSACleanup(); /*?????????????????????*/ //Fermeture de Winsock

	return 0;
}  