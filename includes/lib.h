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
@page on_error() \b
@brief on_error - Notificacion error
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b void on_error(int log,char* err);
@section description Descripcion
Imprime el mensaje dado en el syslog y finaliza el proceso.
@param log: Nivel de prioridad del log.
@param err: mensaje a imprimir.
@section return RETORNO
@return void
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
void on_error(int log,char* err);
/**
@page ini_server() \b
@brief ini_server - Inicializacion Servidor
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int ini_server(int port);
@section description Descripcion
Inicializa un socket y lo deja escuchando
@param port: Puerto donde escucha el servidor.
@section return RETORNO
@return int: EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE en caso de error.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int ini_server(int port);
/**
@page daemonizar() \b
@brief daemonizar - Creacion de un daemon
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int daemonizar(char* service);
@section description Descripcion
 Convierte el proceso a depender de ini (daemon)
@param service: Nombre de servicion a daemonizar.
@section return RETORNO
@return int: EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE en caso de error.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int daemonizar(char* service);
/**
@page accept_conex() \b
@brief accept_conex - Conexion con cliente
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int accept_conex(int sock);
@section description Descripcion
Acepta la conexion por parte de un cliente
@param sock: socket del servidor.
@section return RETORNO
@return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int accept_conex(int sock);
/**
@page host_Ip() \b
@brief host_Ip- Conexion con cliente
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b void host_Ip(int sock, char ** host,char** ip);
@section description Descripcion
Averigua la ip del socket cliente y su nombre de host.
@param sock: Puerto en el que se encuentra el usuario.
@section return RETORNO
@return HostNameIp: encaso de acierto la estructura rellena sino, NULL.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
void host_Ip(int sock, char ** host,char** ip);
/*****************************************************************************
 FUNCIONES SSL
 *****************************************************************************/
 /**
 @page inicializar_nivel_SSL() \b
 @brief inicializar_nivel_SSL - Inicializacion SSL
 @section synopsis SINOPSIS
 \b #include \b lib.h
 \n\b void inicializar_nivel_SSL ();
 @section description Descripcion
 Esta función se encargará de realizar todas las llamadas
 necesarias para que la aplicación pueda usar la capa segura SSL.
 @section return RETORNO
 @section seealso VER TAMBIEN
 @section authors AUTORES
 Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */
void inicializar_nivel_SSL ();
/**
@page fijar_contexto_SSL() \b
@brief fijar_contexto_SSL - Inicializacion Contexto certificados
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int fijar_contexto_SSL(SSL_CTX **contex, char *cert, char *certRoot);
@section description Descripcion
Esta función se encargará de inicializar correctamente
el contexto que será utilizado para la creación de canales seguros
mediante SSL. Deberá recibir información sobre las rutas a los certificados y
claves con los que vaya a trabajar la aplicación.
@param contex : contexto SSL que se inicializa
@param cert: certificado del cliente/servidor
@param certRoot: certificado externo
@section return RETORNO
@return -1 en caso de error
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int fijar_contexto_SSL(SSL_CTX **contex, char *cert, char *certRoot);
/**
@page canal_seguro.() \b
@brief canal_seguro - Creacion Canal Seguro
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int canal_seguro(SSL_CTX *contex, SSL **ssl, int socket);
@section description Descripcion
Dado un contexto SSL y un descriptor de socket esta función
se encargará de bloquear la aplicación.
@param contex : contexto SSL
@param ssl : canal seguro ssl
@param socket : descriptor de socket
@section return RETORNO
@return -1 en caso de error
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int canal_seguro(SSL_CTX *contex, SSL **ssl, int socket);
/**
@page conectar_canal_seguro_SSL.() \b
@brief conectar_canal_seguro_SSL - Conectar a un canal seguro
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int conectar_canal_seguro_SSL(SSL_CTX *contex, SSL **ssl, int socket);
@section description Descripcion
Dado un contexto SSL y un descriptor de socket esta función
se encargará de obtener un canal seguro SSL iniciando el proceso de
handshake con el otro extremo.
@param contex : contexto SSL
@param ssl : canal seguro ssl
@param socket : descriptor de socket
@section return RETORNO
@return -1 en caso de error
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int conectar_canal_seguro_SSL(SSL_CTX *contex, SSL **ssl, int socket);
/**
@page aceptar_canal_seguro_SSL() \b
@brief aceptar_canal_seguro_SSL - Acepta una conexion de canal seguro
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int aceptar_canal_seguro_SSL(SSL_CTX *contex, SSL **ssl, int socket);
@section description Descripcion
Dado un contexto SSL y un descriptor de socket esta función
se encargará de bloquear la aplicación, que se quedará esperando hasta
recibir un handshake por parte del cliente.
@param contex : contexto SSL
@param ssl : canal seguro ssl
@param socket : descriptor de socket
@section return RETORNO
@return -1 en caso de error
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int aceptar_canal_seguro_SSL(SSL_CTX *contex, SSL **ssl, int socket);
/**
@page evaluar_post_connectar_SSL() \b
@brief evaluar_post_connectar_SSL - Comprueba la correcta conexion SSL
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int evaluar_post_connectar_SSL(SSL *ssl);
@section description Descripcion
Esta función comprobará una vez realizado el handshake que el
canal de comunicación se puede considerar seguro.
@param ssl : canal seguro ssl
@section return RETORNO
@return -1 en caso de error
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int evaluar_post_connectar_SSL(SSL *ssl);
/**
@page enviar_datos_SSL() \b
@brief enviar_datos_SSL - Envio de datos por un canal SSL
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int enviar_datos_SSL(SSL *ssl, const void *buf, int num);
@section description Descripcion
Esta función será el equivalente a la función de envío de
mensajes que se realizó en la práctica 1, pero será utilizada para enviar
datos a través del canal seguro. Es importante que sea genérica y
pueda ser utilizada independientemente de los datos que se vayan a enviar.
@param ssl : canal seguro ssl
@param buf : mensaje a enviar
@param num : tamaño en bytes del mensaje
@section return RETORNO
 @return numero de bytes leidos, negativo en caso de fallo
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int enviar_datos_SSL(SSL *ssl, const void *buf, int num);
/**
@page recibir_datos_SSL() \b
@brief recibir_datos_SSL - Recepcion de datos por un canal SSL
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int recibir_datos_SSL(SSL *ssl, void *buf, int num);
@section description Descripcion
Esta función será el equivalente a la función de lectura de
mensajes que se realizó en la práctica 1, pero será utilizada para enviar
datos a través del canal seguro. Es importante que sea genérica y
pueda ser utilizada independientemente de los datos que se vayan a recibir.
@param ssl : canal seguro ssl
@param buf : mensaje a recibir
@param num : tamaño en bytes del mensaje
@section return RETORNO
 @return numero de bytes leidos, negativo en caso de fallo
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int recibir_datos_SSL(SSL *ssl, void *buf, int num);
/**
@page cerrar_canal_SSL() \b
@brief cerrar_canal_SSL - Clausuara de una conexion SSL
@section synopsis SINOPSIS
\b #include \b lib.h
\n\b int recibir_datos_SSL(SSL *ssl, void *buf, int num);
@section description Descripcion
Esta función liberará todos los recursos y cerrará el canal
de comunicación seguro creado previamente.
@param contex : contexto SSL
@param ssl : canal seguro ssl
@param socke : descriptor de socket
@section return RETORNO
 @return numero de bytes leidos, negativo en caso de fallo
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
void cerrar_canal_SSL(SSL_CTX *contex, SSL *ssl, int socke);
