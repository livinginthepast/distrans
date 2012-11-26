#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  int i, listenfd, connfd;
  struct sockaddr_in serv_addr;

  char *address = "0.0.0.0";
  int port = 80;

  char sendBuff[1025];
  time_t ticks;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) {
      port = atoi(argv[i+1]);
    } else if (strcmp(argv[i], "-l") == 0) {
      address = argv[i+1];
    }
  }

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);

  printf("listen: %s:%d\n", address, port);
}
