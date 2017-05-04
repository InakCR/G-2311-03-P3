#include "../includes/lib.h"
#include <arpa/inet.h>
#include <getopt.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <redes2/ircxchat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_BYTES 8192
#define LOG_NUM 2000
#define T_AL 20
#define MAX_DS 100
#define PORT 6669

char cert[200] = "./certs/cliente.pem";
char certRoot[200] = "./certs/ca.pem";

char id[LOG_NUM] = "";
char *nickC, *userC, *realnameC, *passwordC, *serverC;
int sock, portC = 0;
boolean sslC;
int numero = 14606209;
/**
 * @defgroup IRCInterface Interface
 *
 */

/**
 * @defgroup IRCInterfaceCallbacks Callbaks
 * @ingroup IRCInterface
 *
 */

/**
 * @addtogroup IRCInterfaceCallbacks
 * Funciones que van a ser llamadas desde el interface y que deben ser
 * implementadas por el usuario.
 * Todas estas funciones pertenecen al hilo del interfaz.
 *
 * El programador puede, por supuesto, separar todas estas funciones en
 * múltiples ficheros a
 * efectos de desarrollo y modularización.
 *
 * <hr>
 */
/**
 * @brief Manejador de la alarma para el Ping.
 *
 * @description Crea un mensaje de Ping para lanzarlo al servidor
*/

void alarma_ping() {
  char *command, idPing[LOG_NUM] = "";

  command = (char *)malloc(NUM_BYTES * sizeof(char));

  sprintf(idPing, "LAG%d", numero);
  numero++;

  IRCMsg_Ping(&command, NULL, idPing, NULL);
  // //enviarDatos(sock, command);

  // Escribimos en el registro plano
  IRCInterface_PlaneRegisterOutMessage(command);
  free(command);
  alarm(20);
}
/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateChannelKey IRCInterface_ActivateChannelKey
 *
 * @brief Llamada por el botón de activación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateChannelKey (char *channel, char * key)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la clave del canal. El segundo
 *parámetro es
 * la clave del canal que se desea poner. Si es NULL deberá impedirse la
 *activación
 * con la función implementada a tal efecto. En cualquier caso sólo se puede
 *realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se va a activar la clave.
 * @param[in] key clave para el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateChannelKey(char *channel, char *key) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateExternalMessages
 *IRCInterface_ActivateExternalMessages
 *
 * @brief Llamada por el botón de activación de la recepción de mensajes
 *externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateExternalMessages (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la recepción de mensajes
 *externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateExternalMessages(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateInvite IRCInterface_ActivateInvite
 *
 * @brief Llamada por el botón de activación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateInvite (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateInvite(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateModerated IRCInterface_ActivateModerated
 *
 * @brief Llamada por el botón de activación de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateModerated (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateModerated(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateNicksLimit IRCInterface_ActivateNicksLimit
 *
 * @brief Llamada por el botón de activación del límite de usuarios en el canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateNicksLimit (char *channel, int * limit)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación del límite de usuarios en el canal. El
 *segundo es el
 * límite de usuarios que se desea poner. Si el valor es 0 se sobrentiende que
 *se desea eliminar
 * este límite.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará el límite de usuarios.
 * @param[in] limit límite de usuarios en el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateNicksLimit(char *channel, int limit) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivatePrivate IRCInterface_ActivatePrivate
 *
 * @brief Llamada por el botón de activación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivatePrivate (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivatePrivate(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateProtectTopic IRCInterface_ActivateProtectTopic
 *
 * @brief Llamada por el botón de activación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateProtectTopic (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la protección de tópico.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar la protección de
 *tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateProtectTopic(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateSecret IRCInterface_ActivateSecret
 *
 * @brief Llamada por el botón de activación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateSecret (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar el estado de secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_ActivateSecret(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_BanNick IRCInterface_BanNick
 *
 * @brief Llamada por el botón "Banear".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_BanNick (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Banear". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se va a realizar el baneo. En principio
 *es un valor innecesario.
 * @param[in] nick nick del usuario que va a ser baneado
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_BanNick(char *channel, char *nick) {}
void client() {
  char sendBuff[100], client_message[LOG_NUM];
  char *command, *pipeCommand, *pipe;

  pipe = (char *)malloc(NUM_BYTES * sizeof(char));
  command = (char *)malloc(NUM_BYTES * sizeof(char));

  if (pipe == NULL || command == NULL)
    return -1;

  while (1) {

    bzero(command, NUM_BYTES);

    //  if (recvCommand(sock, command) == -1)
    //  return;

    pipe = IRC_UnPipelineCommands(command, &pipeCommand);

    while (pipe != NULL) {

      // commandARealizar(pipeCommand, sock);

      pipe = IRC_UnPipelineCommands(NULL, &pipeCommand);
    }

    bzero(command, NUM_BYTES);
  }

  return;
}
/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_Connect IRCInterface_Connect
 *
 * @brief Llamada por los distintos botones de conexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	long IRCInterface_Connect (char *nick, char * user, char * realname,
 *char * password, char * server, int port, boolean ssl)
 * @endcode
 *
 * @description
 * Función a implementar por el programador.
 * Llamada por los distintos botones de conexión. Si implementará la
 *comunicación completa, incluido
 * el registro del usuario en el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leída.
 *
 *
 * @param[in] nick nick con el que se va a realizar la conexíón.
 * @param[in] user usuario con el que se va a realizar la conexión.
 * @param[in] realname nombre real con el que se va a realizar la conexión.
 * @param[in] password password del usuario si es necesaria, puede valer NULL.
 * @param[in] server nombre o ip del servidor con el que se va a realizar la
 *conexión.
 * @param[in] port puerto del servidor con el que se va a realizar la conexión.
 * @param[in] ssl puede ser TRUE si la conexión tiene que ser segura y FALSE si
 *no es así.
 *
 * @retval IRC_OK si todo ha sido correcto (debe devolverlo).
 * @retval IRCERR_NOSSL si el valor de SSL es TRUE y no se puede activar la
 *conexión SSL pero sí una
 * conexión no protegida (debe devolverlo).
 * @retval IRCERR_NOCONNECT en caso de que no se pueda realizar la comunicación
 *(debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

long IRCInterface_Connect(char *nick, char *user, char *realname,
                          char *password, char *server, int port, boolean ssl) {
  struct hostent *he;
  struct sockaddr_in serverdir;
  struct sockaddr_in serveraux;
  pthread_t pthread;
  pthread_attr_t attr;
  char *ip, *command, buffer[MAX_DS], client_msg[LOG_NUM];
  int *nsocket, numbytes, err;

  printf("Nick: %s,user : %s, realname : %s, pass : %s server : %s \n", nick,
         user, realname, password, server);

  nickC = (char *)malloc(sizeof(char) * strlen(nick) + 1);
  strncpy(nickC, nick, strlen(nick) + 1);
  userC = (char *)malloc(sizeof(char) * strlen(user) + 1);
  strncpy(userC, user, strlen(user) + 1);
  realnameC = (char *)malloc(sizeof(char) * strlen(realname) + 1);
  strncpy(realnameC, realname, strlen(realname) + 1);
  passwordC = (char *)malloc(sizeof(char) * strlen(password) + 1);
  strncpy(passwordC, password, strlen(password) + 1);

  printf("Nick: %s,user : %s, realname : %s, pass : %s server : %s \n", nickC,
         userC, realnameC, passwordC, server);
  portC = port;
  sslC = ssl;
  // mutex?
  if ((he = gethostbyname(server)) == NULL)
    return IRCERR_NOCONNECT;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return IRCERR_NOCONNECT;

  serverdir.sin_family = AF_INET;
  serverdir.sin_port = htons(port);
  serverdir.sin_addr = *((struct in_addr *)he->h_addr);

  bzero(&(serverdir.sin_zero), 8);
  pthread_attr_init(&attr);
  if (pthread_create(&pthread, NULL, client, NULL) < 0)
    return IRCERR_NOCONNECT;

  if (connect(sock, (struct sockaddr *)&serverdir, sizeof(struct sockaddr)) ==
      -1)
    return IRCERR_NOCONNECT;

  ip = inet_ntoa(serverdir.sin_addr);

  printf("Enviamos CAP LS\n");
  // enviarDatos(sock, "CAP LS\r\n", strlen("CAP LS\r\n"), 0);

  printf("Enviamos Msg_Nick\n");
  if (IRCMsg_Nick(&command, NULL, nick, NULL) != IRC_OK)
    return IRCERR_NOCONNECT;
  // enviarDatos(sock, command);

  printf("Enviamos Msg_User\n");
  if (IRCMsg_User(&command, NULL, user, "w", realname) != IRC_OK)
    return IRCERR_NOCONNECT;
  // enviarDatos(sock, command);

  printf("Enviamos CAP END\n");
  // enviarDatos(sock, "CAP END\r\n", strlen("CAP END\r\n"), 0);
  return IRC_OK;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateChannelKey IRCInterface_DeactivateChannelKey
 *
 * @brief Llamada por el botón de desactivación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateChannelKey (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la clave del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la clave.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateChannelKey(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateExternalMessages
 *IRCInterface_DeactivateExternalMessages
 *
 * @brief Llamada por el botón de desactivación de la recepción de mensajes
 *externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateExternalMessages (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a deactivar la recepción de
 *mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateExternalMessages(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateInvite IRCInterface_DeactivateInvite
 *
 * @brief Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateInvite (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateInvite(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateModerated IRCInterface_DeactivateModerated
 *
 * @brief Llamada por el botón de desactivación  de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateModerated (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación  de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateModerated(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateNicksLimit IRCInterface_DeactivateNicksLimit
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateNicksLimit (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación  del límite de usuarios en el canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar el límite de
 *usuarios.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateNicksLimit(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivatePrivate IRCInterface_DeactivatePrivate
 *
 * @brief Llamada por el botón de desactivación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivatePrivate (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @param[in] channel canal sobre el que se va a desactivar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivatePrivate(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateProtectTopic IRCInterface_DeactivateProtectTopic
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateProtectTopic (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la protección de tópico.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la protección de
 *tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateProtectTopic(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateSecret IRCInterface_DeactivateSecret
 *
 * @brief Llamada por el botón de desactivación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateSecret (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la propiedad de
 *canal secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_DeactivateSecret(char *channel) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DisconnectServer IRCInterface_DisconnectServer
 *
 * @brief Llamada por los distintos botones de desconexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	boolean IRCInterface_DisconnectServer (char * server, int port)
 * @endcode
 *
 * @description
 * Llamada por los distintos botones de desconexión. Debe cerrar la conexión con
 el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.

 * @param[in] server nombre o ip del servidor del que se va a realizar la
 desconexión.
 * @param[in] port puerto sobre el que se va a realizar la desconexión.
 *
 * @retval TRUE si se ha cerrado la conexión (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_DisconnectServer(char *server, int port) {
  printf("Desconectamos del server\n");

  char *command; // = (char*)malloc(sizeof(char)*8192);

  printf("QUIT - Enviamos Msg_Quit\n");

  if (IRCMsg_Quit(&command, NULL, "Salir, Adios") != IRC_OK)
    return FALSE;

  IRCInterface_PlaneRegisterOutMessage(command);
  // enviarDatos(sock, command);

  IRCInterface_WriteSystem("*", "Desconectado ().");
  IRCInterface_RemoveAllChannels();
  close(sock);
  alarm(0);

  free(command);
  return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ExitAudioChat IRCInterface_ExitAudioChat
 *
 * @brief Llamada por el botón "Cancelar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ExitAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe
 *seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función cierrala comunicación.
 *Evidentemente tiene que
 * actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario que solicita la parada del chat de audio.
 *
 * @retval TRUE si se ha cerrado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_ExitAudioChat(char *nick) { return TRUE; }

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveOp IRCInterface_GiveOp
 *
 * @brief Llamada por el botón "Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveOp (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Op". Previamente debe seleccionarse un nick del
 * canal para darle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se va dar op al usuario.
 * @param[in] nick nick al que se le va a dar el nivel de op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_GiveOp(char *channel, char *nick) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveVoice IRCInterface_GiveVoice
 *
 * @brief Llamada por el botón "Dar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveVoice (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Dar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se va dar voz al usuario.
 * @param[in] nick nick al que se le va a dar voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_GiveVoice(char *channel, char *nick) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_KickNick IRCInterface_KickNick
 *
 * @brief Llamada por el botón "Echar".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_KickNick (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Echar". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se va a expulsar al usuario.
 * @param[in] nick nick del usuario que va a ser expulsado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_KickNick(char *channel, char *nick) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewCommandText IRCInterface_NewCommandText
 *
 * @brief Llamada la tecla ENTER en el campo de texto y comandos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewCommandText (char *command)
 * @endcode
 *
 * @description
 * Llamada de la tecla ENTER en el campo de texto y comandos. El texto deberá
 *ser
 * enviado y el comando procesado por las funciones de "parseo" de comandos de
 * usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] comando introducido por el usuario.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_NewCommandText(char *command) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewTopicEnter IRCInterface_NewTopicEnter
 *
 * @brief Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewTopicEnter (char * topicdata)
 * @endcode
 *
 * @description
 * Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 * Deberá intentarse cambiar el tópico del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * param[in] topicdata string con el tópico que se desea poner en el canal.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_NewTopicEnter(char *topicdata) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_SendFile IRCInterface_SendFile
 *
 * @brief Llamada por el botón "//enviar Archivo".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_SendFile (char * filename, char *nick, char *data,
 *long unsigned int length)
 * @endcode
 *
 * @description
 * Llamada por el botón "//enviar Archivo". Previamente debe seleccionarse un
 *nick
 *del
 * canal para darle voz a dicho usuario. Esta función como todos los demás
 *callbacks bloquea el interface
 * y por tanto es el programador el que debe determinar si crea un nuevo hilo
 *para //enviar el archivo o
 * no lo hace.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] filename nombre del fichero a //enviar.
 * @param[in] nick nick del usuario que //enviará el fichero.
 * @param[in] data datos a ser enviados.
 * @param[in] length longitud de los datos a ser enviados.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_SendFile(char *filename, char *nick, char *data,
                              long unsigned int length) {
  return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StartAudioChat IRCInterface_StartAudioChat
 *
 * @brief Llamada por el botón "Iniciar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StartAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Iniciar" del diálogo de chat de voz. Previamente debe
 *seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás
 *callbacks bloquea el interface
 * y por tanto para mantener la funcionalidad del chat de voz es imprescindible
 *crear un hilo a efectos
 * de comunicación de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario con el que se desea conectar.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_StartAudioChat(char *nick) { return TRUE; }

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StopAudioChat IRCInterface_StopAudioChat
 *
 * @brief Llamada por el botón "Parar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StopAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe
 *seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función sólo para la comunicación
 *que puede ser reiniciada.
 * Evidentemente tiene que actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario con el que se quiere parar el chat de voz.
 *
 * @retval TRUE si se ha parado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

boolean IRCInterface_StopAudioChat(char *nick) { return TRUE; }

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeOp IRCInterface_TakeOp
 *
 * @brief Llamada por el botón "Quitar Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeOp (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Quitar Op". Previamente debe seleccionarse un nick del
 * canal para quitarle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se va a quitar op al usuario.
 * @param[in] nick nick del usuario al que se le va a quitar op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_TakeOp(char *channel, char *nick) {}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeVoice IRCInterface_TakeVoice
 *
 * @brief Llamada por el botón "Quitar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeVoice (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Quitar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo
 *leídas.
 *
 * @param[in] channel canal sobre el que se le va a quitar voz al usuario.
 * @param[in] nick nick del usuario al que se va a quitar la voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
*/

void IRCInterface_TakeVoice(char *channel, char *nick) {}
int conexion(char *data) {
  struct hostent *he;
  struct sockaddr_in serverdir;
  struct sockaddr_in serveraux;
  pthread_t pthread;
  pthread_attr_t attr;
  char *ip, *command, buffer[MAX_DS], client_msg[LOG_NUM];
  int *nsocket, numbytes, err;
  if (portC == 0) {
    portC = PORT;
  }
  if ((he = gethostbyname("localhost")) == NULL)
    return IRCERR_NOCONNECT;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return IRCERR_NOCONNECT;

  serverdir.sin_family = AF_INET;
  serverdir.sin_port = htons(portC);
  serverdir.sin_addr = *((struct in_addr *)he->h_addr);

  bzero(&(serverdir.sin_zero), 8);

  if (connect(sock, (struct sockaddr *)&serverdir, sizeof(struct sockaddr)) ==
      -1)
    return IRCERR_NOCONNECT;

  printf("Enviamos Data\n");
  return send(sock, data, strlen(data), 0);
}
int conexionSegura(char *data) {
  struct sockaddr_in server;
  SSL_CTX *contex;
  SSL *ssl;
  int sock;
  char user[512];

  if (portC == 0) {
    portC = PORT;
  }
  syslog(LOG_INFO, "Puerto %d\n", portC);
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
  syslog(LOG_INFO, "Soket");
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(portC);

  syslog(LOG_INFO, "connect");
  // Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
  syslog(LOG_INFO, "Connect");
  if (conectar_canal_seguro_SSL(contex, &ssl, sock) < 1) {
    perror("Error conectar SSL");
    return 1;
  }
  if (evaluar_post_connectar_SSL(ssl) == 0) {
    perror("Evaluacion SSL Incorrecta");
    return 1;
  }
  syslog(LOG_INFO, "Enviamos Data %s\n", data);
  if (enviar_datos_SSL(ssl, data, strlen(data)) < 0) {
    syslog(LOG_ERR, "Send SSL failed");
    return 1;
  }
  // bzero(user, 512);
  // strcpy(user, "USER yoda 0 * :miservidor.com");
  if (enviar_datos_SSL(ssl, "USER yoda 0 * :miservidor.com", 512) < 0) {
    syslog(LOG_ERR, "Send SSL failed");
    return 1;
  }
  cerrar_canal_SSL(contex, ssl, sock);
  return 1;
}
/***************************************************************************************************/
/***************************************************************************************************/
/** **/
/** MMMMMMMMMM               MMMMM           AAAAAAA           IIIIIII
 * NNNNNNNNNN          NNNNNN **/
/**  MMMMMMMMMM             MMMMM            AAAAAAAA           IIIII
 * NNNNNNNNNN          NNNN  **/
/**   MMMMM MMMM           MM MM            AAAAA   AA           III     NNNNN
 * NNNN          NN   **/
/**   MMMMM  MMMM         MM  MM            AAAAA   AA           III     NNNNN
 * NNNN         NN   **/
/**   MMMMM   MMMM       MM   MM           AAAAA     AA          III     NNNNN
 * NNNN        NN   **/
/**   MMMMM    MMMM     MM    MM           AAAAA     AA          III     NNNNN
 * NNNN       NN   **/
/**   MMMMM     MMMM   MM     MM          AAAAA       AA         III     NNNNN
 * NNNN      NN   **/
/**   MMMMM      MMMM MM      MM          AAAAAAAAAAAAAA         III     NNNNN
 * NNNN     NN   **/
/**   MMMMM       MMMMM       MM         AAAAA         AA        III     NNNNN
 * NNNN    NN   **/
/**   MMMMM        MMM        MM         AAAAA         AA        III     NNNNN
 * NNNN   NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN
 * NNNN  NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN
 * NNNN NN   **/
/**  MMMMMMM                 MMMM     AAAAAA            AAAA    IIIII   NNNNNN
 * NNNNNNN  **/
/** MMMMMMMMM               MMMMMM  AAAAAAAA           AAAAAA  IIIIIII NNNNNNN
 * NNNNNNN **/
/** **/
/***************************************************************************************************/
/***************************************************************************************************/

int main(int argc, char *argv[]) {
  /* La función IRCInterface_Run debe ser llamada al final      */
  /* del main y es la que activa el interfaz gráfico quedándose */
  /* en esta función hasta que se pulsa alguna salida del       */
  /* interfaz gráfico.                                          */
  static struct option long_options[] = {
      {"ssl", no_argument, NULL, 's'},
      {"port", required_argument, NULL, 'p'},
      {"ssldata", required_argument, NULL, 'l'},
      {"data", required_argument, NULL, 'd'},
      {NULL, 0, NULL, 0}};
  char ch;
  int conex = 0;
  char data[512];
  int i;
  int index = 2;
  bzero(data, 512);

  while ((ch = getopt_long(argc, argv, "s:p:l:d", long_options, &index)) !=
         -1) {
    switch (ch) {
    case 'd':
      conex = 1;
      for (i = 0; i < argc; i++) {
        if (strcmp("--data", argv[i]) == 0) {
          i++;
          break;
        }
      }
      for (i; i < argc; i++) {
        strcat(data, argv[i]);
        strcat(data, " ");
        syslog(LOG_INFO, " %s\n", argv[i]);
      }
      syslog(LOG_INFO, "Conexion %s\n", data);
      break;
    case 'l':

      conex = 2;
      for (i = 0; i < argc; i++) {
        if (strcmp("--ssldata", argv[i]) == 0) {
          i++;
          break;
        }
      }
      for (i; i < argc; i++) {
        strcat(data, argv[i]);
        strcat(data, " ");
        syslog(LOG_INFO, " %s\n", argv[i]);
      }
      syslog(LOG_INFO, "Conexion Segura %s\n", data);
      break;
    case 's':
      sslC = TRUE;
      printf("Seguridad SSL activada \n");
      break;
    case 'p':
      portC = atoi(optarg);
      syslog(LOG_INFO, "Puerto %d\n", portC);
      printf("Escuchando puerto %d\n", portC);
      break;
    case '?':
      printf("Argumento no reconocido\n");
      break;
    }
  }
  if (conex == 1) {
    return conexion(data);
  } else if (conex == 2) {
    return conexionSegura(data);
  }

  IRCInterface_Run(argc, argv);

  return 0;
}
