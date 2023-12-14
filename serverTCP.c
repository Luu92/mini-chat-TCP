// Ficheros de cabecera
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

// Funcion principal
int main(int argc, char *argv[])
{
    
    if(argc < 3){
        printf("Debes ingresar más parametros: <programa> <IPv4> <Puerto>\n\n");
        return -1;
    }
    
    // Estructuras y Variables
    struct sockaddr_in server;
    struct sockaddr_in client;
    int fd, fd2, longitud_cliente, numbytes, puerto;
    char buf[1024];     // Para recibir mensaje
    char enviar2[1024]; // Para enviar mensaje
    char enviar[1024];  // Para enviar mensaje
    char direccion_ip[16];

    system("clear");
    printf("La direccion del servidor es %s\n\n",argv[1]);
    puerto = atoi(argv[2]);
    strncpy(direccion_ip,argv[1],sizeof(direccion_ip));

    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);
    server.sin_addr.s_addr = inet_addr(direccion_ip);
    bzero(&(server.sin_zero), 8);

    // Definicion de socket
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error de apertura de socket");
        exit(-1);
    }

    // Avisar al sistema que se creo un socket
    if (bind(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        printf("error en bind() \n");
        exit(-1);
    }

    // Establecer el socket en modo que permita 5 conecciones. 
    if (listen(fd, 5) == -1)
    {
        printf("error en listen()\n");
        exit(-1);
    }

    printf("SERVIDOR EN ESPERA...\n");
    
    
    longitud_cliente = sizeof(struct sockaddr_in);
    if ((fd2 = accept(fd, (struct sockaddr *)&client, &longitud_cliente)) == -1)
    {
        printf("error en accept()\n");
        exit(-1);
    }

    printf("==== SESION INICIADA ====\n");
    printf("CLIENTE CONECTADO\n");
    strcpy(enviar, "SERVIDOR CONECTADO...");
    send(fd2, enviar, 1024, 0);

    // Ciclo para enviar y recibir mensajes
    while (1)
    {
        // El servidor espera el primer mensaje
        recv(fd2, buf, 1024, 0);
        if (strcmp(&buf, "salir") == 0)
        {
            break;
        }
        printf("Cliente: %s\n", buf);

        // El cliente recibe el mensaje del servidor
        printf("Escribir mensaje: ");
        scanf("%*c%[^\n]", enviar2);
        send(fd2, enviar2, 1024, 0);
        if (strcmp(enviar2, "salir") == 0)
        {
            break;
        }
    }
    close(fd2);
    close(fd);
    return 0;
}