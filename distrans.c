#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define VERSION 0.1
#define BUFFER 8026
#define WRITE_BUFFER 124

void implode(int socket) {
  close(socket);
  exit(1);
}

void forbidden(int socket) {
  (void)write(socket, "HTTP/1.1 403 Forbidden\nContent-Length: 14\nConnection: close\nContent-Type: text/html\n\n403 Forbidden\n",99);
  implode(socket);
}

void error(char *msg) {
  (void)fprintf(stderr, "ERROR: %s\n", msg);
  exit(1);
}

void respond(int socket, int verbose) {
  long i, req;
  static char buffer[BUFFER+1];
  static char write_buffer[WRITE_BUFFER+1];
  static const char *GIF = "GIF89a\x01\x00\x01\x00\x00\xFF\x00,\x00\x00\x00\x00\x01\x00\x01\x00\x00\x02\x00;";

  req = read(socket, buffer, BUFFER);

  /* read failure stop now */
	if(req == 0 || req == -1)
		(void)forbidden(socket);

  if(req > 0 && req < BUFFER) {
    /* request is valid, terminate buffer */
    buffer[req] = 0;
  } else {
    buffer[0] = 0;
  }

  /* remove CF and LF characters */
	for(i=0;i<req;i++)
		if(buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i]='*';

  /* null terminate after the second space to ignore extra stuff */
	for(i=4;i<BUFFER;i++) {
	  /* string is "GET URL " +lots of other stuff */
		if(buffer[i] == ' ') {
			buffer[i] = 0;
			break;
		}
	}

  if (verbose)
    puts(buffer);

	/* verify we are asking for gif */
	if( strncmp(buffer,"GET /_.px",9) != 0 && strncmp(buffer,"get /_.px",9) != 0)
	  (void)forbidden(socket);

	if (verbose)
	  (void)printf("writing content for: %s\n", buffer);

  /* Header + a blank line */
  (void)sprintf(write_buffer,"HTTP/1.1 200 OK\nServer: distrans/%f\nContent-Length: 26\nConnection: close\nContent-Type: image/gif\n\n", VERSION);
  (void)sprintf(write_buffer,"%s", GIF);
  (void)write(socket, write_buffer, strlen(write_buffer));

  sleep(0.001);
  implode(socket);
}

int main(int argc, char *argv[]) {
  int i, pid, listenfd, socketfd;
  socklen_t length;
  static struct sockaddr_in serv_addr;
	static struct sockaddr_in cli_addr;

  int verbose = 0;
  int port = 80;
  char *address = "0.0.0.0";

  char sendBuff[1025];

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) {
      port = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-l") == 0) {
      address = argv[i+1];
    } else if (strcmp(argv[i], "-v") == 0) {
      verbose = 1;
    }
  }

  if (port < 0 || port > 60000)
    (void)error("Invalid port");

  if (verbose)
    (void)printf("listening on: %s:%d\n", address, port);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);

  while(1) {
    length = sizeof(cli_addr);
    socketfd = accept(listenfd, (struct sockaddr*)&cli_addr, &length);

    pid = fork();
    if(pid == 0) {
      (void)close(listenfd);
      respond(socketfd, verbose);
    } else if (pid > 0) {
      (void)close(socketfd);
    }
  }

  return 0;
}
