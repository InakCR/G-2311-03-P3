/**
 * @brief Libreria de funciones para la gestion del servidor IRC
 *
 * @file G-2311-03-P3server.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/G-2311-03-P3commands.h"
#include <getopt.h>
#include <inttypes.h>
/**
   @brief tiempo maximo de alarma en segundos
 */
#define STIEMPO 30
/**
   @brief tiempo maximo de alarma en milisegundos
 */
#define MSTIEMPO 30000
/**
   @brief puerto de escucha del servidor
 */
#define PORT 6667
#define STIEMPO 30
#define MSTIEMPO 30000
/**
   @page deal_cliente() \b
   @brief deal_cliente. - Gestion de un nuevo cliente
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1server.h
   \n\b void* deal_cliente(void* sock);
   @section description Descripcion
   Trata con el cliente recibiendo sus mensajes
   y contestandole
   @param sock: Socket de la conexion cliente.
   @section return RETORNO
   @return void*
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */
void* deal_cliente(void* sock);
/**
   @page recibir() \b
   @brief recibir. - Acepta Conexion Cliente
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1server.h
   \n\b int recibir(int sock, char** userNick);
   @section description Descripcion
   Recibe un comman do por parte del cliente y lo realiza ne el servidor
   @param sock: socket del servidor.
   @section return RETORNO
   @return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */

int recibir(int sock, char** userNick);
/**
   @page get_numero_clientes() \b
   @brief get_numero_clientes. - Numero de clientes conectados en el Servidor
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1server.h
   \n\b long get_numero_clientes();
   @section description Descripcion
   Devuelve el número de clintes conectados actualmente al servidor.
   @section return RETORNO
   @return long: nClientes.
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */

long get_numero_clientes();
/**
   @page seguridad_ssl() \b
   @brief seguridad_ssl - Inicializa la seguridad SSL
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1server.h
   \n\b void seguridad_ssl();
   @section description Descripcion
   Prepara la seguridad SSL usando los certificados.
   @section return RETORNO
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */
void seguridad_ssl();
/**
   @page aceptar_conexion_ssl() \b
   @brief aceptar_conexion_ssl - Creacion canal SSL
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1server.h
   \n\b void aceptar_conexion_ssl(int client_sock);
   @section description Descripcion
   Crea un canal seguro SSL con el socket del cliente.
   @param client_sock: socket del cliente.
   @section return RETORNO
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */
void aceptar_conexion_ssl(int client_sock);
/**
   @page recibir_datos() \b
   @brief recibir_datos - Creacion canal SSL
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1server.h
   \n\b int recibir_datos(int sock, char *command);
   @section description Descripcion
   recibe los datos enviados por el socket dado.
   @param sock: conexion de socket.
   @param command: datos recibidos.
   @section return RETORNO
   @return int: numero de bytes recibidos.
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 */
int recibir_datos(int sock, char *command);
