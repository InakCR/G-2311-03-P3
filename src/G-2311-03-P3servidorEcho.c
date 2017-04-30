#include "../includes/lib.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

char cert[200] = "./certs/servidor.pem";
char certRoot[200] = "./certs/ca.pem";

int main(int argc, char *argv[]) {
  int socket_desc, client_sock, c, read_size;
  struct sockaddr_in server, client;
  char client_message[BUFFER_SIZE];
  SSL_CTX *contex;
  SSL *ssl;

  if (argc > 2) {
    strcpy(cert, argv[1]);
    strcpy(certRoot, argv[2]);
  }

  // SSL
  inicializar_nivel_SSL();
  if (fijar_contexto_SSL(&contex, cert, certRoot) == -1) {
    puts("Error SSL");
    return -1;
  }
  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    printf("Could not create socket");
  }
  puts("Socket created");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(3333);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    // print the error message
    perror("bind failed. Error");
    return 1;
  }
  puts("bind done");

  // Listen
  listen(socket_desc, 3);

  // Accept and incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  // accept connection from an incoming client
  client_sock =
      accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }
  if (aceptar_canal_seguro_SSL(contex, &ssl, client_sock) == 0) {
    puts("Error Aceptar SSL");
    return -1;
  }

  if (evaluar_post_connectar_SSL(ssl) == 0) {
    perror("Evaluacion SSL Incorrecta");
    return 1;
  }
  puts("Connection accepted");

  while ((read_size = recibir_datos_SSL(ssl, client_message, BUFFER_SIZE)) >
         0) {
    enviar_datos_SSL(ssl, client_message, strlen(client_message));
    bzero(client_message, strlen(client_message));
  }

  if (read_size == 0) {
    puts("Client disconnected");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }

  cerrar_canal_SSL(contex, ssl, client_sock);
  return 0;
}
