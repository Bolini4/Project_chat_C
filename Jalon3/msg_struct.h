//
// Created by matte on 10/06/2022.
//

#ifndef PROJETC_MSG_STRUCT_H

#define NICK_LEN 128
#define INFOS_LEN 128

/*On crée une variable de type "enum msg_type"
  Ex :
  Nickname_new = 1
  nickname_list =2
  etc ...*/

enum msg_type {
    NICKNAME_NEW,
    NICKNAME_LIST,
    NICKNAME_INFOS,
    ECHO_SEND,
    UNICAST_SEND,
    BROADCAST_SEND,
    EXIT,
};

struct message {
    int pld_len;
    char nick_sender[NICK_LEN];
    enum msg_type type;
    char infos[INFOS_LEN];
};

#define PROJETC_MSG_STRUCT_H

#endif //PROJETC_MSG_STRUCT_H
