/**
 * @brief Libreria de funciones para la creacion
 * y uso de un servidor basado en IRC. Implementacion de funciones OpenSSL
 *
 * @file lib.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 3.0
 * @date 23-03-2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <redes2/irc.h>
#include <redes2/irctad.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdarg.h>
#include <memory.h>
#include <stropts.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include "openssl/ssl.h"
#include "openssl/err.h"

/**
   @brief tamaño maximo que puede tener un buffer.
 */
#define BUFFER_SIZE 8192
/**
   @brief numero de clientes maximo
   que soporta el servidor.
 */
#define MAX_CLIENTS 100
/**
   @brief numero maximo de milisegundos.
 */
#define MAX_MNS 2000
/**
   @brief tamaño maximo de bytes.
 */
#define NUM_BYTES 8192
/**
   @brief on_error.

   Descripcion: Imprime el mensaje dado en el syslog y finaliza el proceso.
   @param log: Nivel de prioridad del log.
   @param err: mensaje a imprimir.
   @return void
 */
void on_error(int log,char* err);
/**
   @brief ini_server.

   Descripcion: Inicializa un socket y lo deja escuchando
   @param port: Puerto donde escucha el servidor.
   @return int: EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE en caso de error.
 */
int ini_server(int port);
/**
   @brief daemonizar.

   Descripcion: Convierte el proceso a depender de ini (daemon)
   @param service: Nombre de servicion a daemonizar.
   @return int: EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE en caso de error.
 */
int daemonizar(char* service);
/**
   @brief accept_conex.

   Descripcion: Acepta la conexion por parte de un cliente
   @param sock: socket del servidor.
   @return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
 */
int accept_conex(int sock);
/**
   @brief hostIp.

   Descripcion: Averigua la ip del socket cliente y su nombre de host.
   @param sock: Puerto en el que se encuentra el usuario.
   @return HostNameIp: encaso de acierto la estructura rellena sino, NULL.
 */
void hostIp(int sock, char ** host,char** ip);
/*****************************************************************************
 FUNCIONES SSL
 *****************************************************************************/

/**
   @brief inicializar_nivel_SSL.

   Descripcion:  Esta función se encargará de realizar todas las llamadas
    necesarias para que la aplicación pueda usar la capa segura SSL.
 */
void inicializar_nivel_SSL ();
/**
   @brief fijar_contexto_SSL.

   Descripcion: Esta función se encargará de inicializar correctamente
   el contexto que será utilizado para la creación de canales seguros
   mediante SSL. Deberá recibir información sobre las rutas a los certificados y
   claves con los que vaya a trabajar la aplicación.
   @param contex : contexto SSL que se inicializa
   @param cert: certificado del cliente/servidor
   @param certRoot: certificado externo
   @return -1 en caso de error
 */
int fijar_contexto_SSL(SSL_CTX **contex, char *cert, char *certRoot);
/**
   @brief canal_seguro.

   Descripcion: Dado un contexto SSL y un descriptor de socket esta función
   se encargará de bloquear la aplicación.
   @param contex : contexto SSL
   @param ssl : canal seguro ssl
   @param socket : descriptor de socket
   @return -1 en caso de error
 */
int canal_seguro(SSL_CTX *contex, SSL **ssl, int socket);
/**
   @brief conectar_canal_seguro_SSL.

   Descripcion: Dado un contexto SSL y un descriptor de socket esta función
   se encargará de obtener un canal seguro SSL iniciando el proceso de
   handshake con el otro extremo.
   @param contex : contexto SSL
   @param ssl : canal seguro ssl
   @param socket : descriptor de socket
   @return -1 en caso de error
 */
int conectar_canal_seguro_SSL(SSL_CTX *contex, SSL **ssl, int socket);

/**
   @brief aceptar_canal_seguro_SSL.

   Descripcion: Dado un contexto SSL y un descriptor de socket esta función
   se encargará de bloquear la aplicación, que se quedará esperando hasta
   recibir un handshake por parte del cliente.
   @param contex : contexto SSL
   @param ssl : canal seguro ssl
   @param socket : descriptor de socket
   @return -1 en caso de error
 */
int aceptar_canal_seguro_SSL(SSL_CTX *contex, SSL **ssl, int socket);
/**
   @brief evaluar_post_connectar_SSL.

   Descripcion: Esta función comprobará una vez realizado el handshake que el
   canal de comunicación se puede considerar seguro.
   @param ssl : canal seguro ssl
   @return -1 en caso de error
 */
int evaluar_post_connectar_SSL(SSL *ssl);
/**
   @brief enviar_datos_SSL.

   Descripcion:  Esta función será el equivalente a la función de envío de
   mensajes que se realizó en la práctica 1, pero será utilizada para enviar
   datos a través del canal seguro. Es importante que sea genérica y
   pueda ser utilizada independientemente de los datos que se vayan a enviar.
   @param ssl : canal seguro ssl
   @param buf : mensaje a enviar
   @param num : tamaño en bytes del mensaje
   @return numero de bytes leidos, negativo en caso de fallo
 */
int enviar_datos_SSL(SSL *ssl, const void *buf, int num);
/**
   @brief recibir_datos_SSL.

   Descripcion: Esta función será el equivalente a la función de lectura de
   mensajes que se realizó en la práctica 1, pero será utilizada para enviar
   datos a través del canal seguro. Es importante que sea genérica y
   pueda ser utilizada independientemente de los datos que se vayan a recibir.
   @param ssl : canal seguro ssl
   @param buf : mensaje a recibir
   @param num : tamaño en bytes del mensaje
   @return numero de bytes leidos, negativo en caso de fallo
 */
int recibir_datos_SSL(SSL *ssl, void *buf, int num);
/**
   @brief cerrar_canal_SSL.

   Descripcion: Esta función liberará todos los recursos y cerrará el canal
   de comunicación seguro creado previamente.
 */
void cerrar_canal_SSL(SSL_CTX *contex, SSL *ssl, int socke);
