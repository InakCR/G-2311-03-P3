#include "../includes/G-2311-03-P3user.h"

char prefixU[10] = "REDES2";
char motdServerUser[50] = "**BIENVENIDO AL SERVIDOR**";

void nick(char *string, int sock, char **userNick) {

  char *prefix, *nick, *msg, *command;

  if (IRCParse_Nick(string, &prefix, &nick, &msg) != IRC_OK) {

    syslog(LOG_ERR, "***Fallo en el Parseo. Nick");

    IRC_MFree(3, &prefix, &nick, &msg);

  } else {

    // Primera vez que llega NICK
    if (*userNick == NULL) {

      *userNick = (char *)malloc(sizeof(strlen(nick) + 1));
      strcpy(*userNick, nick);

    } else {

      if (UTestNick(nick)) {

        IRCMsg_ErrAlreadyRegistred(&command, prefix, nick);
        enviar_datos(sock, command);

      } else {

        // Para el cambio de Nick

        if (IRCTADUser_Test(0, NULL, *userNick) == IRC_OK) {

          if (IRCMsg_Nick(&command, *userNick, msg, nick) == IRC_OK) {
            send_all_user(command);
          }

          set_nick(nick, userNick);
        }
      }

      IRC_MFree(4, &prefix, &nick, &msg, &command);
    }
  }
}

void user(char *string, int sock, char *userNick) {
  char *host, *ip, *prefix, *user, *modehost, *serverother, *realname, *command;
  time_t rawtime;
  long tadret;

  host_Ip(sock, &host, &ip);

  if (IRCParse_User(string, &prefix, &user, &modehost, &serverother,
                    &realname) != IRC_OK) {

    syslog(LOG_ERR, "***Fallo en el Parseo. User");

    IRC_MFree(5, &prefix, &user, &modehost, &serverother, &realname);
  }

  tadret = IRCTADUser_New(user, userNick, realname, NULL, host, ip, sock);

  if (tadret == IRC_OK || tadret == IRCERR_NICKUSED) {
    if (UTestNick(userNick)) {
      IRCTADUser_SetSocket(0, NULL, userNick, NULL, sock);
    }
    IRCMsg_RplWelcome(&command, prefixU, userNick, user, realname, modehost);
    enviar_datos(sock, command);

    IRCMsg_RplYourHost(&command, prefixU, userNick, serverother, "1.0");
    enviar_datos(sock, command);

    time(&rawtime);
    IRCMsg_RplCreated(&command, prefixU, userNick, rawtime);
    enviar_datos(sock, command);

    IRCMsg_RplLuserClient(&command, prefixU, userNick,
                          get_numero_nlientes_actuales(), 0, 1);
    enviar_datos(sock, command);

    IRCMsg_RplLuserChannels(&command, prefixU, userNick, get_numero_canales());
    enviar_datos(sock, command);

    IRCMsg_RplMotdStart(&command, prefixU, userNick, prefixU);
    enviar_datos(sock, command);

    IRCMsg_RplMotd(&command, prefixU, userNick, motdServerUser);
    enviar_datos(sock, command);

    IRCMsg_RplEndOfMotd(&command, prefixU, userNick);
    enviar_datos(sock, command);

  } else if (tadret == IRCERR_NOENOUGHMEMORY) {

    // TODO

  } else if (tadret == IRCERR_NICKUSED) {

    IRCMsg_ErrAlreadyRegistred(&command, prefix, userNick);
    enviar_datos(sock, command);

    IRC_MFree(6, &prefix, &user, &modehost, &serverother, &realname, &command);

  } else if (tadret == IRCERR_INVALIDUSER) {

    // TODO

  } else if (tadret == IRCERR_INVALIDNICK) {

    // TODO

  } else if (tadret == IRCERR_INVALIDREALNAME) {

    // TODO

  } else if (tadret == IRCERR_INVALIDHOST) {

    // TODO

  } else if (tadret == IRCERR_INVALIDIP) {

    // TODO

  } else if (tadret == IRCERR_INVALIDID) {

    // TODO

  } else if (tadret == IRCERR_INVALIDSOCKET) {

    // TODO

  } else if (tadret == IRCERR_NOMUTEX) {

    // TODO
  }
}

void whois(char *string, int sock, char *userNick) {

  char *prefix, *target, *maskarray, *command;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  char *listChan;
  long id = 0, actionTS = 0, creationTS = 0, num = 0;
  int socket = 0;

  if (IRCParse_Whois(string, &prefix, &target, &maskarray) == IRC_OK) {

    if (UTestNick(maskarray)) {

      IRCTADUser_GetData(&id, &user, &maskarray, &real, &host, &IP, &socket,
                         &creationTS, &actionTS, &away);
      // 311
      IRCMsg_RplWhoIsUser(&command, prefixU, userNick, maskarray, user, host,
                          real);
      enviar_datos(sock, command);
      syslog(LOG_INFO, "%s", command);
      // 312
      IRCMsg_RplWhoIsServer(&command, prefixU, userNick, maskarray, prefixU,
                            "No OnE");
      enviar_datos(sock, command);
      syslog(LOG_INFO, "%s", command);
      // // 313
      if (IRCTAD_ListChannelsOfUser(user, maskarray, &listChan, &num) ==
          IRC_OK) {
        if (num > 0) {
          // 319
          IRCMsg_RplWhoIsChannels(&command, prefixU, userNick, maskarray,
                                  listChan);

          enviar_datos(sock, command);
        }
      }
      // 317
      // 301
      if (away != NULL) {
        IRCMsg_RplAway(&command, prefixU, userNick, maskarray, away);
        enviar_datos(sock, command);
      }
      // 318
      IRCMsg_RplEndOfWhoIs(&command, prefixU, userNick, maskarray);
      enviar_datos(sock, command);

      // IRC_MFree(5, &prefix, &target, &maskarray, &id, &user, &maskarray,
      // &real,
      //&host, &IP, &socket, &creationTS, &actionTS, &away, &command);

    } else {

      // TODO error del nick. No lo encuentro

      syslog(LOG_INFO, " No existe el nick: %s", maskarray);

      // IRC_MFree(4, &prefix, &target, &maskarray, &command);
    }
  } else {

    if (IRCMsg_ErrNoNickNameGiven(&command, userNick, userNick) == IRC_OK) {
      enviar_datos(sock, command);
    }

    syslog(LOG_ERR, "***Fallo en el Parseo. WhoIs");

    // IRC_MFree(3, &prefix, &target, &maskarray);
  }
}

void away(char *string, int sock, char *userNick) {
  char *reason = NULL, *command, *prefix;

  if (IRCParse_Away(string, &prefix, &reason) != IRC_OK) {

    syslog(LOG_ERR, "***Fallo en el Parseo. Away");

    IRC_MFree(4, &prefix, &reason);
    return;
  }

  if (reason != NULL) {
    if (set_away(userNick, reason) != IRC_OK) {
      // TODO error nick, no lo encuentro
      syslog(LOG_ERR, "Error setaway");
      return;
    }

    if (IRCMsg_RplNowAway(&command, userNick, userNick) == IRC_OK) {
      enviar_datos(sock, command);
    }

    if (IRCMsg_RplAway(&command, userNick, userNick, userNick, reason) ==
        IRC_OK) {
      enviar_datos(sock, command);
    }
  } else {
    if (IRCMsg_RplUnaway(&command, userNick, userNick) == IRC_OK) {
      enviar_datos(sock, command);
    }
  }
}

void msg_user(char *nick, char *userNick, char *msg) {
  char *command, *reason;
  int socket;

  reason = is_away(nick);

  if (reason != NULL) {
    IRCMsg_RplAway(&command, userNick, userNick, nick, reason);
    socket = getsocket(userNick);
    enviar_datos(socket, command);
  } else {
    if (IRCMsg_Privmsg(&command, userNick, nick, msg) == IRC_OK) {
      socket = getsocket(nick);
      enviar_datos(socket, command);
    }
  }
}
void setMotdUser(char *motd) {
  // motdServer = (char *)malloc(strlen(motd + 1) * sizeof(char));
  // strcpy(motdServer, motd);
}
