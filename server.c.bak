// Codul serverului

#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>

#include <string.h>

#include <time.h>



#define MY_PORT_ID 6000 /* numar > 5000 */



void handle_client(int client_socket) {

    char buffer[255];

    clock_t start, end;

    double duration;



    while (1) {



        bzero(buffer, sizeof(buffer));

	printf("Astept mesaje de la client.\n");

        int bytes_read = read(client_socket, buffer, sizeof(buffer));

        



	if (bytes_read < 0) {

            perror("Eroare citire din socket");

            break; 

        } else if (bytes_read == 0) {

            printf("Clientul a inchis conexiunea. Iesire din bucla.\n");

            break;

        }



        if (strncmp(buffer, ".d", 2) == 0) {

            // Verificăm comanda de întârziere

            float delay = atof(buffer + 3);

            if (delay >= 0) {

                printf("Setare delay de %f secunde\n", delay);

                usleep(delay*1000000);

	 	printf("Am primit de la client: %s", buffer);

		write(client_socket, buffer, strlen(buffer));



		printf("Am raspuns catre client\n");



            } else {

                printf("EROARE: delay >= 0\n");

                strcpy(buffer, "EROARE: delay >= 0");

                write(client_socket, buffer, strlen(buffer));

            }

        } 

    }

	close(client_socket);

}





int main() {

    int sockid, client_socket;

    struct sockaddr_in ssock_addr, csock_addr;

    unsigned int len;



    printf("->creare socket\n");

    if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        perror("Eroare creare socket");

        return -1;

    }



    bzero((char *) &ssock_addr, sizeof(ssock_addr));

    ssock_addr.sin_family = AF_INET;

    ssock_addr.sin_addr.s_addr = INADDR_ANY;

    ssock_addr.sin_port = htons(MY_PORT_ID);



    printf("->asociere adresa\n");

    if (bind(sockid, (struct sockaddr *) &ssock_addr, sizeof(ssock_addr)) < 0) {

        perror("Eroare asociere adresa");

        return -1;

    }



    printf("->incepere ascultare\n");

    listen(sockid, 5);



    while (1) {

	printf("====Asteptare la client====\n");

        len = sizeof(csock_addr);

        client_socket = accept(sockid, (struct sockaddr *) &csock_addr, &len);

        if (client_socket < 0) {

            perror("Eroare acceptare conexiune");

            return -1;

        }

        

        printf("Conexiune acceptata.\n");

        handle_client(client_socket);

    }



    return 0;

}

