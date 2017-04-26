#include "../includes/lib.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
char cert[200] = "./certs/cliente.pem";
char certRoot[200] = "./certs/ca.pem";

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in server;
  char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];
  SSL_CTX *contex;
  SSL *ssl;
  // SSL
  inicializar_nivel_SSL();
  if (fijar_contexto_SSL(&contex, cert, certRoot) == -1) {
    puts("Error SSL");
    return -1;
  }
  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Could not create socket");
  }

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(3333);

  // Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
  if (conectar_canal_seguro_SSL(contex, &ssl, sock) < 1) {
    perror("Error conectar SSL");
    return 1;
  }
  if (evaluar_post_connectar_SSL(ssl) == 0) {
    perror("Evaluacion SSL Incorrecta");
    return 1;
  }
  while (1) {
    bzero(message, BUFFER_SIZE);
    bzero(server_reply, BUFFER_SIZE);

    scanf("%s", message);
    if (strcmp(message, "exit") == 0)
      break;
    // Send some data
    if (enviar_datos_SSL(ssl, message, strlen(message)) < 0) {
      puts("Send SSL failed");
      return 1;
    }
    if (recibir_datos_SSL(ssl, server_reply, strlen(message)) < 0) {
      puts("Recibir  SSL failed");
      return 1;
    }
    puts(server_reply);
  }

  cerrar_canal_SSL(contex, ssl, sock);
  return 0;
}
