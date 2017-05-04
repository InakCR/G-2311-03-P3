/**
 * @brief Libreria de funciones para la gestion del servidor IRC
 *
 * @file server.h
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
@brief deal_cliente.

Descripcion: Trata con el cliente recibiendo sus mensajes
  y contestandole
@param sock: Socket de la conexion cliente.
@return void
*/
void* deal_cliente(void* sock);
/**
@brief accept_conex.

Descripcion: Recibe un comman do por parte del cliente y lo realiza ne el servidor
@param sock: socket del servidor.
@return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
*/
int recibir(int sock, char** userNick);
/**
@brief seguridadSSL.

Descripcion: Prepara la seguridad SSL usando los certificados.
*/
void seguridadSSL();
/**
@brief aceptarConexionSSL.

Descripcion: Crea un canal seguro SSL con el socket del cliente.
@param client_sock: socket del cliente.
*/
void aceptarConexionSSL(int client_sock);
/**
@brief recibirDatos.

Descripcion: recibe los datos enviados por el socket dado.
@param sock: conexion de socket.
@param command: datos recibidos.
@return int: numero de bytes recibidos.
*/
int recibirDatos(int sock, char *command);
/**
@brief getNumeroClientes.

Descripcion: Devuelve el número de clintes conectados actualmente al servidor.
@return int: nClientes.
*/
long getNumeroClientes();
