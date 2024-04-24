#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>

#include <string.h>

#include <sys/time.h>

#include <pthread.h>

#include <arpa/inet.h>



#define MY_PORT_ID 6000 /* numar > 5000 */



struct client_info {

    int client_socket;

    struct sockaddr_in client_addr;

};



int counterClntMsg=1;

int counterSrvrRspns=1;



void handle_client(void *arg) {

    struct client_info *info = (struct client_info *)arg;

    int client_socket = info->client_socket;

    struct sockaddr_in *client_addr = &(info->client_addr);



    char *client_ip = inet_ntoa(client_addr->sin_addr);

    int client_port = ntohs(client_addr->sin_port);



    char buffer[255];

    clock_t start, end;

    double duration;



    while (1) {

        bzero(buffer, sizeof(buffer));

        int bytes_read = read(client_socket, buffer, sizeof(buffer));



        if (bytes_read < 0) {

            perror("[E] Eroare citire din socket\n");

            break; 

        } else if (bytes_read == 0) {

            printf("[i] Clientul (%s:%d) a inchis conexiunea\n",client_ip, client_port);

            break;

        }



        if (strncmp(buffer, ".d", 2) == 0) {

            float delay = atof(buffer + 3);

            if (delay >= 0) {

                printf("[i] Setare delay de %f secunde (%s:%d)\n", delay, client_ip, client_port);

                usleep(delay*1000000);

                printf("[M] Am primit de la client: %s", buffer);

                write(client_socket, buffer, strlen(buffer));



                printf("[i] Am raspuns catre client\n");



            } else {

                printf("[E] EROARE: (delay < 0)\n");

                strcpy(buffer, "[E] EROARE: (delay < 0)");

                write(client_socket, buffer, strlen(buffer));

            }

        } else if(strncmp(buffer, ".t", 2) == 0) {

            printf("[i] clientul (%s:%d) a cerut data si ora actuala\n", client_ip, client_port);

            char response[1000];

            struct timeval tv;

            time_t current_time;

            struct tm *local_time;



            gettimeofday(&tv, NULL);

            current_time = tv.tv_sec;



            local_time = localtime(&current_time);



            strftime(response, sizeof(response), "Timpul curent: %Y-%m-%d %H:%M:%S", local_time);



            write(client_socket, response, strlen(response));

        } else if(strncmp(buffer, ".h", 2) == 0) {

            printf("[i] clientul (%s:%d) a cerut ajutor\n", client_ip, client_port);

            char response[] = "=HELP=\n.h = ajutor\n.t = Obtineti data si ora actuala\n.d x = temporizator setat la x secunde\nmesaj = 'mesaj' va fi transmis catre server\n";



            write(client_socket, response, strlen(response));

        }else{

		printf("%d.[M] Clientul (%s:%d) spune: %s",counterClntMsg++, client_ip, client_port, buffer);



		// Send a response to the client

		printf(" >%d\n",counterSrvrRspns);

		char message[1024];

		fgets(message, sizeof(message), stdin); // Read message from keyboard

		

		write(client_socket, message, strlen(message));



		printf("[i] Mesajul a fost trimis\n");

		counterSrvrRspns++;

	}

        //printf("=================================\n");

    }

    close(client_socket);

    pthread_exit(NULL);

}



int main() {

    int sockid, client_socket;

    struct sockaddr_in ssock_addr, csock_addr;

    unsigned int len;



    //printf("->creare socket\n");

    if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        perror("[E] Eroare creare socket\n");

        return -1;

    }



    bzero((char *) &ssock_addr, sizeof(ssock_addr));

    ssock_addr.sin_family = AF_INET;

    ssock_addr.sin_addr.s_addr = INADDR_ANY;

    ssock_addr.sin_port = htons(MY_PORT_ID);



    //printf("->asociere adresa\n");

    if (bind(sockid, (struct sockaddr *) &ssock_addr, sizeof(ssock_addr)) < 0) {

        perror("[E] Eroare asociere adresa\n");

        return -1;

    }



    //printf("->incepere ascultare\n");

    listen(sockid, 5);



    printf("_________     _____   _______\n");

    printf("|        |   /  ___|  |   _  \\ \n");

    printf("|__    __|  /  /      |  |_| /\n");

    printf("   |  |    |   |      |  ___/\n");

    printf("   |  |     \\   \\_    |  |\n");

    printf("   |__|      \\____|   |__|\n");   

    printf("Server\n");

    printf("running on //10.0.2.15:6000\n");

    printf("Terminate connection with CTRL+C\n\n"); 

    printf("=====================================\n");

    pthread_t tid;



    while (1) {

        len = sizeof(csock_addr);

        client_socket = accept(sockid, (struct sockaddr *) &csock_addr, &len);

        if (client_socket < 0) {

            perror("[E] Eroare acceptare conexiune\n");

            return -1;

        }



        printf("[i] Un nou client s-a conectat.\n");



	struct client_info info;

	info.client_socket = client_socket;

	info.client_addr = csock_addr;



        pthread_create(&tid, NULL, handle_client, (void *)&info);

        pthread_detach(tid); // Detach the thread to avoid memory leaks

    }

    printf("===Conxiune terminata===\n");

    return 0;

}

