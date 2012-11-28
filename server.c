#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define READ_BUFFER 8026

void respond(int socket) {
  long req;
  static char read_buffer[READ_BUFFER+1];

  req = read(socket, read_buffer, READ_BUFFER);

  if(req > 0 && req < READ_BUFFER) {
    /* request is valid, terminate buffer */
    read_buffer[req] = 0;
  } else {
    read_buffer[0] = 0;
  }

  sprintf("request: %s", buffer);

  close(socketfd);
  exit(1);
}

int main(int argc, char *argv[]) {
  int i, pid, verbose, listenfd, socketfd;
  socklen_t length;
  static struct sockaddr_in serv_addr;
	static struct sockaddr_in cli_addr;

  int port = 80;
  char *address = "0.0.0.0";


  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) {
      port = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-l") == 0) {
      address = argv[i+1];
    } else if (strcmp(argv[i], "-v") == 0) {
      verbose = 1;
    }
  }

  if (verbose)
    (void) sprintf("listening on: %s:%d\n", address, port);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);

  while(1) {
    length = sizeof(cli_addr);
    socketfd = accept(listenfd, (struct sockaddr*)&cli_addr, &length);

    pid = fork();
    if(pid == 0) {
      (void)close(listenfd);
      respond(socketfd);
    } else if (pid > 0) {
      (void)close(socketfd);
    }
  }

  return 0;
}
