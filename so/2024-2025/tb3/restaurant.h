#ifndef RESTAURANT_H
#define RESTAURANT_H

#define NUMBER_OF_DISHES 5

#define PEDIDOS_ESPERA 100

typedef struct {
    char *name;
    int preparation_time;
}DISHES;


DISHES dishes[NUMBER_OF_DISHES] = {
    {"Tripas de porco", 3},
    {"Francesinha", 5},
    {"Bife da vazia", 4},
    {"Miojo", 2},
    {"Sushi", 1}
};

/* structure of a request from a client to central server */
typedef struct {
    long type; /* type of message */
    int pid; /* pid of the client */
    int dish; /* dish to prepare (numero do prato, corresponde ao indice do array)*/
}REQUEST;


typedef struct {
    int client_id; // ID do cliente
    int dish;      // Prato solicitado
} SharedDish;



#endif 
