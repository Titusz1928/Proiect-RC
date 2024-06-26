// Codul clientului

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <sys/time.h>



#define MY_PORT_ID 6000 /* numar > 5000 */

#define SERV_HOST_ADDR "10.0.2.15"



#define SYSTEM_SOUND "/usr/share/sounds/freedesktop/stereo/complete.oga"



#define ANSI_COLOR_RED     "\x1b[31m"

#define ANSI_COLOR_GREEN   "\x1b[32m"

#define ANSI_COLOR_YELLOW  "\x1b[33m"

#define ANSI_COLOR_BLUE    "\x1b[34m"

#define ANSI_COLOR_MAGENTA "\x1b[35m"

#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"



void play_system_sound() {

    pid_t pid = fork();

    if (pid == 0) {

        // Child process

        execlp("paplay", "paplay", SYSTEM_SOUND, NULL);

        // If execlp returns, it means an error occurred

        perror("Error executing paplay");

        exit(EXIT_FAILURE);

    } else if (pid < 0) {

        // Fork failed

        perror("Error forking");

    } else {

        // Parent process

        // Wait for the child process to finish

        wait(NULL);

    }

}







int main() {

    int sockid;

    struct sockaddr_in ssock_addr;

    char msg[255];

    clock_t start, end;

    double duration;



    //printf("->creare socket\n");

    if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        perror("[E] Eroare creare socket");

        return -1;

    }



    bzero((char *) &ssock_addr, sizeof(ssock_addr));

    ssock_addr.sin_family = AF_INET;

    ssock_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);

    ssock_addr.sin_port = htons(MY_PORT_ID);

    

    //printf("->conectare catre server\n");

    if (connect(sockid, (struct sockaddr *) &ssock_addr, sizeof(ssock_addr)) < 0) {

        perror(ANSI_COLOR_RED"[E] Eroare conectare la server"ANSI_COLOR_RESET);

        return -1;

    }



    printf(ANSI_COLOR_CYAN"_________     _____   _______\n");

    printf("|        |   /  ___|  |   _  \\ \n");

    printf("|__    __|  /  /      |  |_| /\n");

    printf("   |  |    |   |      |  ___/\n");

    printf("   |  |     \\   \\_    |  |\n");

    printf("   |__|      \\____|   |__|\n"ANSI_COLOR_RESET); 

    printf("Client\n");

    printf("connected to 10.0.2.15:6000\n");

    printf("to get help use command .h\n");

    printf("Terminate connection with CTRL+C\n\n");

    printf("=====================================\n"); 

    while (1) {

	    printf("[M] Introduceti un mesaj pentru server: ");

	    fgets(msg, sizeof(msg), stdin);



	    struct timeval start, end;

	    long seconds, microseconds;

	    double elapsed;



	    gettimeofday(&start, NULL);

	    if (write(sockid, msg, strlen(msg)) < 0) {

		perror(ANSI_COLOR_RED"[E] Eroare scriere in socket"ANSI_COLOR_RESET);

		break;

	    }



	    bzero((char *) &msg, sizeof(msg));



	    if (read(sockid, msg, sizeof(msg)) < 0) {

		perror(ANSI_COLOR_RED"[E] Eroare citire din socket"ANSI_COLOR_RESET);

		break;

	    }

	    

	    gettimeofday(&end, NULL); // Move this line here



	    seconds = end.tv_sec - start.tv_sec;

	    microseconds = end.tv_usec - start.tv_usec;

	    elapsed = seconds + microseconds * 1e-6;

	    play_system_sound();

	    printf(ANSI_COLOR_BLUE"[i] Timp masurat: %.6f secunde\n"ANSI_COLOR_RESET, elapsed);

	    printf(ANSI_COLOR_GREEN"[R] Serverul spune: %s\n"ANSI_COLOR_RESET, msg);

	    printf("=====================================\n");

	}



    close(sockid);

    printf("===Conxiune terminata\n===");

    return 0;

}

