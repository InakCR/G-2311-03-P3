#include "../includes/G-2311-03-P3user.h"

char prefixU[10] = "REDES2";
char motdServerUser[50] = "**BIENVENIDO AL SERVIDOR**";

void nick(char *string, int sock, char **userNick) {
  char *prefix, *nick, *msg, *command;

  if (IRCParse_Nick(string, &prefix, &nick, &msg) != IRC_OK) {
    syslog(LOG_ERR, "***Error No existe una cadena para usar como Nick.");
    return;
  }

  if (*userNick == NULL) {
    *userNick = (char *)malloc(sizeof(strlen(nick) + 1));
    strcpy(*userNick, nick);
  }

  if (UTestNick(nick)) {
    syslog(LOG_INFO, "!!!!!!!!!! Ya existe el nick: %s", nick);
    // TODO MAndar mensaje error
  } else {

    if (IRCTADUser_Test(0, NULL, *userNick) == IRC_OK) {
      if (IRCMsg_Nick(&command, *userNick, msg, nick) == IRC_OK) {
        sendAllUser(command);
      }
      setNick(nick, userNick);
    }
  }

  // // TODO PROBANDO
  // // Lista de Usuarios en el servidor
  //
  // IRCTADUser_GetUserList(&nicklist, &nelements);
  //
  // for (i = 0; i < nelements; i++) {
  //   syslog(LOG_INFO, "Usuario nº%d -> %s", i, nicklist[i]);
  // }
  //
  // // Lista de Nicks en el servidor
  //
  // IRCTADUser_GetNickList(&nicklist, &nelements);
  //
  // for (i = 0; i < nelements; i++) {
  //   syslog(LOG_INFO, "Nick nº%d -> %s", i, nicklist[i]);
  // }
  //
  // syslog(LOG_INFO, "Nick pasado correctamente como %s.", *userNick);
  free(prefix);
  free(nick);
  free(msg);
}
void user(char *string, int sock, char *userNick) {
  char *host, *ip, *prefix, *user, *modehost, *serverother, *realname, *command;
  time_t rawtime;
  char **nicklist;
  long nelements;
  int i = 0;

  hostIp(sock, &host, &ip);

  if (IRCParse_User(string, &prefix, &user, &modehost, &serverother,
                    &realname) != IRC_OK) {
    syslog(LOG_ERR, "***Error No existe una cadena para usar como User.");
    return;
  }

  if (IRCTADUser_New(user, userNick, realname, NULL, host, ip, sock) !=
      IRC_OK) {
    syslog(LOG_INFO, "************Usuario no creado");
    return;
  }

  // Lista de Usuarios en el servidor
  //
  //
  // IRCTADUser_GetUserList(&nicklist, &nelements);
  //
  // for (i = 0; i < nelements; i++) {
  //   syslog(LOG_INFO, "Usuario nº%d -> %s", i, nicklist[i]);
  // }

  // Lista de Nicks en el servidor

  // IRCTADUser_GetNickList(&nicklist, &nelements);
  //
  // for (i = 0; i < nelements; i++) {
  //   syslog(LOG_INFO, "Nick nº%d -> %s", i, nicklist[i]);
  // }
  // syslog(LOG_INFO, "%s", prefix);
  // syslog(LOG_INFO, "%s", serverother);

  IRCMsg_RplWelcome(&command, prefixU, userNick, user, realname, modehost);
  enviarDatos(sock, command);

  IRCMsg_RplYourHost(&command, prefixU, userNick, serverother, "1.0");
  enviarDatos(sock, command);

  time(&rawtime);
  IRCMsg_RplCreated(&command, prefixU, userNick, rawtime);
  enviarDatos(sock, command);

  IRCMsg_RplLuserClient(&command, prefixU, userNick,
                        getNumeroClientesActuales(), 0, 1);
  enviarDatos(sock, command);

  IRCMsg_RplLuserChannels(&command, prefixU, userNick, getNumeroCanales());
  enviarDatos(sock, command);

  IRCMsg_RplMotdStart(&command, prefixU, userNick, prefixU);
  enviarDatos(sock, command);

  IRCMsg_RplMotd(&command, prefixU, userNick, motdServerUser);
  enviarDatos(sock, command);

  IRCMsg_RplEndOfMotd(&command, prefixU, userNick);
  enviarDatos(sock, command);

  free(prefix);
  free(user);
  free(modehost);
  free(serverother);
  free(realname);
}
void whois(char *string, int sock, char *userNick) {
  char *prefix, *target, *maskarray, *command;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  char *listChan;
  long id = 0, actionTS = 0, creationTS = 0, num = 0;
  int socket = 0;

  if (IRCParse_Whois(string, &prefix, &target, &maskarray) == IRC_OK) {

    if (UTestNick(maskarray)) {

      if (IRCTADUser_GetData(&id, &user, &maskarray, &real, &host, &IP, &socket,
                             &creationTS, &actionTS, &away) == IRC_OK) {
      }
      // 311
      IRCMsg_RplWhoIsUser(&command, prefixU, userNick, maskarray, user, host,
                          real);
      enviarDatos(sock, command);
      syslog(LOG_INFO, "%s", command);
      // 312
      IRCMsg_RplWhoIsServer(&command, prefixU, userNick, maskarray, prefixU,
                            "No OnE");
      enviarDatos(sock, command);
      syslog(LOG_INFO, "%s", command);
      // // 313
      if (IRCTAD_ListChannelsOfUser(user, maskarray, &listChan, &num) ==
          IRC_OK) {
        if (num > 0) {
          // 319
          IRCMsg_RplWhoIsChannels(&command, prefixU, userNick, maskarray,
                                  listChan);

          enviarDatos(sock, command);
        }
      }
      // 317
      // 301
      if (away != NULL) {
        IRCMsg_RplAway(&command, prefixU, userNick, maskarray, away);
        enviarDatos(sock, command);
      }
      // 318
      IRCMsg_RplEndOfWhoIs(&command, prefixU, userNick, maskarray);
      enviarDatos(sock, command);
    } else {
      syslog(LOG_INFO, " No existe el nick: %s", maskarray);
      // MENSAJE ERROR
    }
  } else {
    if (IRCMsg_ErrNoNickNameGiven(&command, userNick, userNick) == IRC_OK) {
      enviarDatos(sock, command);
    }
    syslog(LOG_ERR, "Error Parseo Whois");
  }
}
void away(char *string, int sock, char *userNick) {
  char *reason = NULL, *command, *prefix;

  if (IRCParse_Away(string, &prefix, &reason) != IRC_OK) {
    syslog(LOG_ERR, "Error Away");
    return;
  }
  if (reason != NULL) {
    if (setAway(userNick, reason) != IRC_OK) {
      syslog(LOG_ERR, "Error setaway");
      return;
    }

    if (IRCMsg_RplNowAway(&command, userNick, userNick) == IRC_OK) {
      enviarDatos(sock, command);
    }

    if (IRCMsg_RplAway(&command, userNick, userNick, userNick, reason) ==
        IRC_OK) {
      enviarDatos(sock, command);
    }
  } else {
    if (IRCMsg_RplUnaway(&command, userNick, userNick) == IRC_OK) {
      enviarDatos(sock, command);
    }
  }
}

void msgUser(char *nick, char *userNick, char *msg) {
  char *command, *reason;
  int socket;

  // reason = isAway(userNick);
  //
  // if (reason != NULL) {
  //   if (IRCTADUser_SetAway(0, NULL, userNick, NULL, NULL) != IRC_OK) {
  //     syslog(LOG_ERR, "Error unSetAway");
  //     return;
  //   }
  //
  //   IRCMsg_RplUnaway(&command, userNick, userNick);
  //   socket = getsocket(userNick);
  //   enviarDatos(socket, command);
  // }

  reason = isAway(nick);

  if (reason != NULL) {
    IRCMsg_RplAway(&command, userNick, userNick, nick, reason);
    socket = getsocket(userNick);
    enviarDatos(socket, command);
  } else {
    if (IRCMsg_Privmsg(&command, userNick, nick, msg) == IRC_OK) {
      socket = getsocket(nick);
      enviarDatos(socket, command);
    }
  }
}
void setMotdUser(char *motd) {
  // motdServer = (char *)malloc(strlen(motd + 1) * sizeof(char));
  // strcpy(motdServer, motd);
}
