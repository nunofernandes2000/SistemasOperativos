#ifndef RESTAURANT_H
#define RESTAURANT_H

#define NUMBER_OF_DISHES 5

#define PEDIDOS_ESPERA 20

typedef struct {
    char *name;
    int preparation_time;
}DISHES;


DISHES dishes[NUMBER_OF_DISHES] = {
    {"Pica-pau", 3},
    {"Francesinha", 5},
    {"Punheta de bacalhau", 4},
    {"Miojo", 2},
    {"Peixe cru", 1}
};

/* structure of a request from a client to central server */
typedef struct {
    long type; /* type of message */
    int pid; /* pid of the client */
    int dish; /* dish to prepare (numero do prato, corresponde ao indice do array)*/
}REQUEST;


typedef struct {
    int pid; /* pid of the client */
    int dish; /* dish to prepare (numero do prato, corresponde ao */
    int status;
} ORDER;


#endif 
