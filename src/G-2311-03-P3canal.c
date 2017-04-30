#include "../includes/G-2311-03-P3canal.h"

char prefixS[10] = "REDES2";

void join(char *string, int sock, char *userNick) {
  char *prefix, *msg, *channel, *key, *command, *topic, *list, **listArray,
      *lista;
  long parser, nUsers, num;
  int i, socket;

  parser = IRCParse_Join(string, &prefix, &channel, &key, &msg);

  if (channel == NULL && key == NULL && msg == NULL) {

    if (IRCMsg_ErrNeedMoreParams(&command, userNick, userNick, string) ==
        IRC_OK) {
      enviarDatos(sock, command);
      return;
    }
  } else if (parser == IRC_OK) {
    if (IRCTADChan_TestPassword(channel, key) == IRCERR_FAIL) {
      IRCMsg_ErrBadChannelKey(&command, prefixS, userNick, channel);
      enviarDatos(sock, command);
      return;
    }
    if (IRCTAD_Join(channel, userNick, "o", key) == IRC_OK) {

      if (IRCMsg_Join(&command, userNick, NULL, NULL, channel) == IRC_OK) {
        enviarDatos(sock, command);
      }

      IRCTAD_GetTopic(channel, &topic);

      if (topic != NULL) {
        if (IRCMsg_RplTopic(&command, prefixS, userNick, channel, topic) ==
            IRC_OK) {
          enviarDatos(sock, command);
        }
      } else {
        if (IRCMsg_RplNoTopic(&command, prefixS, userNick, channel) == IRC_OK) {
          enviarDatos(sock, command);
        }
      }

      IRCTAD_ListNicksOnChannel(channel, &list, &num);
      lista = (char *)malloc(strlen(list + num) * sizeof(char));
      strcpy(lista, "");
      IRCTAD_ListNicksOnChannelArray(channel, &listArray, &num);
      for (i = 0; i < num; i++) {

        if (IRCTAD_GetUserModeOnChannel(channel, listArray[i]) >=
            IRCUMODE_OPERATOR) {
          strcat(lista, "@");
        }
        strcat(lista, listArray[i]);
        strcat(lista, " ");
      }
      if (IRCMsg_RplNamReply(&command, prefixS, userNick, "=", channel,
                             lista) == IRC_OK) {
        if (IRCTAD_ListNicksOnChannelArray(channel, &listArray, &nUsers) ==
            IRC_OK) {
          for (i = 0; i < nUsers; i++) {
            socket = getsocket(listArray[i]);
            enviarDatos(socket, command);
          }
        }
      }
      if (IRCMsg_RplEndOfNames(&command, prefixS, userNick, channel) ==
          IRC_OK) {
        enviarDatos(sock, command);
      }
      free(command);
    }

  } else {
    syslog(LOG_ERR, "***Fallo al crear el canal");
    // Puede devolcer muchos errores
  }
  free(prefix);
  free(msg);
  free(channel);
  free(key);
}
// TODO Controlar cuando solo se manda NAMES sin ningún argumento
void names(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *list, *target;
  long num;

  if (IRCParse_Names(string, &prefix, &channel, &target) == IRC_OK) {
    if (IRCTAD_ListNicksOnChannel(channel, &list, &num) == IRC_OK) {
      if (num > 0) {
        IRCMsg_RplNamReply(&command, prefixS, userNick, "=", channel, list);
        enviarDatos(sock, command);
        syslog(LOG_INFO, "%s", command);
      }
    }
    IRCMsg_RplEndOfNames(&command, prefixS, userNick, channel);
    enviarDatos(sock, command);
  }
}

void part(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, **list, *msg;
  long parser, nUsers;
  int i, socket;

  if (IRCParse_Part(string, &prefix, &channel, &msg) == IRC_OK) {
    parser = IRCTAD_Part(channel, userNick);
    if (parser == IRC_OK) {

      IRCMsg_Part(&command, userNick, channel, msg);
      enviarDatos(sock, command);

      if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
        for (i = 0; i < nUsers; i++) {
          socket = getsocket(list[i]);
          enviarDatos(socket, command);
        }
      }
    }
    // No existe el usuario en el canal
    else if (parser == IRCERR_NOVALIDUSER) {
      IRCMsg_ErrNotOnChannel(&command, prefixS, userNick, userNick, channel);
      enviarDatos(sock, command);
    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {
      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      enviarDatos(sock, command);
    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {
      IRCMsg_ErrNoChanModes(&command, prefixS, userNick, channel);
      enviarDatos(sock, command);
    }
  }
}

void kick(char *string, int sock, char *userNick) {

  char *prefix, *channel, *msg, *user, *command, **list;
  long parser, nUsers, mode;
  int i, socket;

  if (IRCParse_Kick(string, &prefix, &channel, &user, &msg) == IRC_OK) {

    mode = IRCTAD_GetUserModeOnChannel(channel, userNick);

    if (mode < IRCUMODE_OPERATOR) { //== (IRCUMODE_CREATOR | IRCUMODE_OPERATOR
      IRCMsg_ErrChanOPrivsNeeded(&command, userNick, userNick, channel);
      enviarDatos(sock, command);
      syslog(LOG_INFO, "%s", command);
      return;
    }

    parser = IRCTAD_KickUserFromChannel(channel, user);
    if (parser == IRC_OK) {

      parser = IRCMsg_Kick(&command, userNick, channel, user, msg);
      socket = getsocket(user);
      enviarDatos(socket, command);

      IRCMsg_Part(&command, user, channel, msg);
      IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers);
      for (i = 0; i < nUsers; i++) {
        socket = getsocket(list[i]); /// &
        enviarDatos(socket, command);
      }

      // No existe el usuario en el canal
    } else if (parser == IRCERR_NOVALIDUSER) {
      IRCMsg_ErrNotOnChannel(&command, prefixS, userNick, user, channel);
      enviarDatos(sock, command);
    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {
      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      enviarDatos(sock, command);
    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {
      IRCMsg_ErrNoChanModes(&command, prefixS, userNick, channel);
      enviarDatos(sock, command);
    }
  }
}
void topic(char *string, int sock, char *userNick) {
  char *channel, *prefix, *topic, *topicActual, *command, **list;
  long nUsers;
  int i, socket;

  if (IRCParse_Topic(string, &prefix, &channel, &topic) != IRC_OK) {
    syslog(LOG_ERR, "Error Topic");
    return;
  }
  if (topic != NULL) {
    if (IRCTAD_SetTopic(channel, userNick, topic) !=
        IRC_OK) { // Comprueba internamente si puede le usuario
      IRCMsg_ErrChanOPrivsNeeded(&command, prefixS, userNick, channel);
      enviarDatos(sock, command);
      syslog(LOG_ERR, "Error SetTopic");
      return;
    }
    if (IRCMsg_Topic(&command, userNick, channel, topic) == IRC_OK) {
      if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
        for (i = 0; i < nUsers; i++) {
          socket = getsocket(list[i]);
          enviarDatos(socket, command);
        }
      }
    }
  } else {
    if (IRCTAD_GetTopic(channel, &topicActual) != IRC_OK) {
      syslog(LOG_ERR, "Error Topic");
      return;
    }
    if (topicActual != NULL) {

      if (IRCMsg_RplTopic(&command, prefixS, userNick, channel, topicActual) ==
          IRC_OK) {
        enviarDatos(sock, command);
      }
    } else {
      if (IRCMsg_RplNoTopic(&command, prefixS, userNick, channel) == IRC_OK) {
        enviarDatos(sock, command);
      }
    }
  }
}
void mode(char *string, int sock, char *userNick) {
  char *prefix, *channeluser, *mode, *user, *command, *modeChannel;
  if (IRCParse_Mode(string, &prefix, &channeluser, &mode, &user) == IRC_OK) {
    if (mode == NULL && user == NULL) {
      if (IRCTADUser_Test(0, NULL, channeluser) == IRC_OK) {

      } else {
        modeChannel = IRCTADChan_GetModeChar(channeluser);
        if (modeChannel != NULL) {

          IRCMsg_RplChannelModeIs(&command, prefixS, userNick, channeluser,
                                  modeChannel);
          enviarDatos(sock, command);
          syslog(LOG_INFO, "%s", command);
        }
      }
    } else if (user == NULL) {
      IRCTAD_Mode(channeluser, userNick, mode);

      IRCMsg_Mode(&command, prefixS, channeluser, mode, NULL);
      enviarDatos(sock, command);
      syslog(LOG_INFO, "%s", command);

    } else {
      IRCTAD_Mode(channeluser, userNick, mode);

      if (strcmp(mode, "\\+k") == 0) {
        IRCTADChan_SetPassword(channeluser, user);

        IRCMsg_Mode(&command, userNick, channeluser, mode, user);
        enviarDatos(sock, command);
      }
    }
  }
}
void msgCanal(char *channel, char *userNick, char *msg) {
  char *command, **list;
  int i, socket;
  long nUsers;
  if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
    if (IRCMsg_Privmsg(&command, userNick, channel, msg) == IRC_OK) {
      for (i = 0; i < nUsers; i++) {
        if (strcmp(list[i], userNick) != 0) {
          socket = getsocket(list[i]);
          enviarDatos(socket, command);
        }
      }
    }
  }
}
