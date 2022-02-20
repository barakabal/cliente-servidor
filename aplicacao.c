#include <stdio.h>          /* printf */
#include <stdlib.h>         /* exit */
#include <string.h>         /* bzero */
#include <sys/socket.h>     /* struct sockaddr, socket */
#include <netinet/in.h>     /* struct sockaddr_in */
#include <arpa/inet.h>      /* inet_pton, htons */
#include <unistd.h>         /* read */
#include <sys/types.h>
#include <pthread.h>

#define BUFFSIZE 32
#define SA struct sockaddr

typedef struct cel {

  int listenfd;
  int sockfd;
  int serverlen;
  int clientlen;
  struct sockaddr_in *servaddr;
  struct sockaddr_in *me_addr;
  struct sockaddr_in *client;
        
    	
}CELULA;

void error(char *msg);

/*Recebe dados de um servidor. Considere essa thread como recev de um cliente*/
void *thread_leitora (void *arg) {

  int n, sockfd, serverlen, bytes;
  char buffer[BUFFSIZE];
  FILE *pFile;
  struct sockaddr_in *servaddr;
  CELULA *socket_and_server = ((CELULA *) arg);
  
  sockfd = socket_and_server->sockfd;
  servaddr = socket_and_server->servaddr;
  serverlen = socket_and_server->serverlen;

  while (1) {
    
    if ((bytes = recvfrom(sockfd, buffer, BUFFSIZE-1, 0, (SA *) servaddr, &serverlen )) < 0)
      error("Erro ao receber os dados do servidor\n");

    printf("Ip de servidor que enviou: %s\n", inet_ntoa(servaddr->sin_addr));
    pFile = fopen ("myfile.txt","w"); 
    if (pFile!=NULL)
      {
	fputs (buffer,pFile);
	fclose (pFile);
      }
    buffer[bytes] = 0;   
    printf("\nServidor diz :%s\n", buffer);
    printf ("\n[1]Lista de usuários da RCA\n[2]Lista de arquivos dos usuarios\n[3]Baixar Aquivos de um servidor\n");

    //     if ((strcmp(buffer, "321657"))
     
  }

  return(NULL);

}

/*Recebe a estrutura CELULA que contem o socket e a referencia do servidor, DADOS ENVIADO PARA UM SERVIDOR, considere sendto do cliente*/
void *thread_escritora (void *arg) { 
  
  
  int tam_string, sockfd, n, serverlen, descAlterado = 0;
  char string[BUFFSIZE], ip[BUFFSIZE],menu;

  struct sockaddr_in *servaddr;
  fd_set readDescs, teste; /*descritores*/
  struct timeval tv; /*temporizador*/
  CELULA *socket_and_server = ((CELULA *) arg);
  
  
  sockfd = socket_and_server->sockfd;
  servaddr = socket_and_server->servaddr;
  serverlen = socket_and_server->serverlen;


  while (1) {

    FD_ZERO(&readDescs);
    FD_SET(sockfd, &readDescs);
    FD_SET(0, &readDescs);

    tv.tv_sec = 1; /*temporizador de 1 segundo espera o servidor retornar em um periodo de 1 segundo*/
    tv.tv_usec = 0;

    teste = readDescs;    
    //    printf ("Informe uma palavra:\n");

    scanf (" %[^\n]", string);
    tam_string = strlen(string); /*pega o tamanho da string*/
    string[tam_string] = 0;
    menu = string[0];
    switch(menu)
      {
      case '1':
	break;
      case '2':
	break;
      case '3':

	strcat(ip,"3");
	strcat(ip,",");
	printf("Informe o endereco de ip do servidor: ");
	scanf (" %[^\n]", string);
	socket_and_server->servaddr->sin_addr.s_addr = inet_addr(string);
	strcat(ip,string);
	strcat(ip,",");	
	printf("Informe o nome do arquivo");
	scanf (" %[^\n]", string);
	strcat(ip,string);
	tam_string = strlen(ip);
	
	if ((sendto(sockfd, ip, tam_string, 0, (SA *) servaddr, serverlen))  != tam_string )
	  error("Erro ao enviar os dados para o servidor\n");
    	break;

      }
    if (strcmp(string, "1") == 0 || strcmp(string, "2") == 0 || strcmp(string, "3") == 0 ) {
    
      if (strcmp(string, "3") == 0) {
	
	printf("Informe o endereco de IP de um servidor e o nome do arquivo\n");
	scanf (" %[^\n]", ip);
	/* envia para um determinado servidor */
	socket_and_server->servaddr->sin_addr.s_addr = inet_addr(ip);

      }
      else {
	   
	/* 255.255.255.255 broadcast*/
	socket_and_server->servaddr->sin_addr.s_addr = htonl(-1);

      }

      if ((sendto(sockfd, string, tam_string, 0, (SA *) servaddr, serverlen))  != tam_string )
	error("Erro ao enviar os dados para o servidor\n");

      descAlterado = select(FD_SETSIZE, &teste, NULL, NULL, &tv);

      if(descAlterado == 0)
	printf("O servidor nao respondeu, pois nao teve nehum descritor alterado !!!Reenvie novamente\n");

      tv.tv_sec = 1; /*temporizador de 1 segundo*/
      tv.tv_usec = 0;
   
    }// fim se strcmp(string, '1') ....


  }

  return(NULL);

}


void *thread_server_escritora (void *arg) {

   

}

/*Quando estiver na caracteristica de Servidor, aqui leremos o seguinte um cliente diz. DADOS RECEBIDO DE UM CLIENTE*/
void *thread_server_leitora (void *arg) {

  char buffer[BUFFSIZE], b[BUFFSIZE] = "lan_jes";
  char ip_cliente[20], ip_servidor[20],menu,*dados;
  int clientlen, listenfd, n;    
  CELULA *socket_and_server = ((CELULA *) arg);
  struct sockaddr_in *client, *me_addr, *servaddr;
  time_t ticks;
  FILE *pFile;
  long lSize;
  char *buff;
  size_t result;
    
  listenfd = socket_and_server->listenfd;
  client = socket_and_server->client;
  clientlen = socket_and_server->clientlen;
  //clientlen = sizeof(client);
  me_addr = socket_and_server->me_addr;
  servaddr = socket_and_server->servaddr;

  while (1) {
    
    if((n = recvfrom(listenfd, buffer, BUFFSIZE-1, 0, (struct sockaddr *)client, &clientlen )) < 0)
      printf("Falhou ao aceitar a conexao do cliente\n");
      
    /*SERA NECESSARIO ARMAZENAS ESSAS INFORMACOES EM ARQUIVOS*/
    strcpy(ip_cliente, inet_ntoa(client->sin_addr)); // ip do cliente estah armazenado em ip_cliente
    strcpy(ip_servidor, inet_ntoa(me_addr->sin_addr)); // ip do servidor(meu ip quando sou servidor)
    printf("IP cliente %s\nMeuIP %s\n", ip_cliente, ip_servidor); // no ip servidor ele fica tudo zerado
    ticks = time(NULL);// data autal que sera usada para log das informacoes
  

    buffer[n] = '\0';
    menu = buffer[0];
    switch(menu)
      {
      case '1':
	break;
      case '2':
	break;
      case '3':
	dados = strtok(buffer,",");
	dados = strtok(NULL,",");
	dados = strtok(NULL,",");
	pFile = fopen ( dados , "r" );
	if (pFile==NULL) {
	  printf("entrou");
	  fputs ("File error",stderr); exit (1);}
	
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buff = (char*) malloc (sizeof(char)*lSize);
	if (buff == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buff,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose (pFile);
	   
	if ((sendto(listenfd, buff, lSize, 0, (SA *) client, clientlen))  != result )
	  error("Erro ao enviar os dados para o servidor\n");
	break;

	
           
      // sendto(listenfd, buff, lSize, 0,(struct sockaddr *)&client,clientlen);
	   
    }
    
    if (strcmp(buffer, "1") == 0) {
      printf("O cliente solcitou as paradas lista RCA\n");
    }
    else if (strcmp(buffer, "2") == 0) {
	   
      printf("O cliente solicitou Arquivos do diretorio\n");
    }
    else if (strcmp(buffer, "3") == 0) {
      printf("O cliente quer baixar um determinado arquivo deste servidor\n");
     
    }

    n = strlen(b);

    // if(sendto(listenfd, b, n, 0,(struct sockaddr *)client, clientlen) != n )
    //	printf ("Erro ao enviar os dados de volta para o clientes \n");
     
    //printf("recebemos dados do cliente\n");
  
  }

  return(NULL);

}

int main(int argc, char *argv[])
{

  int sockfd, listenfd, bytes = 0, tamanho, fd, setar = 1, config;
  struct sockaddr_in servaddr, me_addr, client;
  char buffer[BUFFSIZE] = "Alan";
  int serverlen = sizeof(servaddr);
  int clientlen = sizeof(client);
  pthread_t escritor, leitor, serv_escritor, serv_leitor;
  CELULA *socket_and_server;
  
  if (argc != 1)
    error("Use: ./executavel");
  
  /*SOCKET SERVIDOR*/
  if((listenfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    error("Falha ao criar o socket");
  
  /*SOCKET CLIENTE*/
  if((sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    error("Falha ao criar o socket");
  
  /* Inicializando a estrutura cliente-server do servidor que vou conectart*/
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = PF_INET;
  servaddr.sin_addr.s_addr = htonl(-1); // endereco do ip do cara que vou conectar 255.255.255.255
  servaddr.sin_port = htons(5555); // endereco da porta do cara que eu vou falar

  /*Inicializando a estrutura minha localidade, servidor*/	
  bzero(&me_addr, sizeof(me_addr));
  me_addr.sin_family = AF_INET;
  me_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
  me_addr.sin_port = htons(5555); // endereco da minha porta

  config = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &setar, sizeof(int));
  
  /* Associa o socket a porta do SO */
  if(bind(listenfd, (SA *) &me_addr, sizeof(me_addr)) < 0)
    error("Falha ao observar o socket do servidor");


  // pegamos o socket e a estrutura do servidor
  socket_and_server = (CELULA *) malloc(sizeof(CELULA));

  socket_and_server->listenfd = listenfd;
  socket_and_server->sockfd = sockfd;
  socket_and_server->servaddr = &servaddr; // estrutura do servidor que vou conecta
  socket_and_server->me_addr = &me_addr; // minha estrutura
  socket_and_server->serverlen = serverlen;
  socket_and_server->clientlen = clientlen;
  socket_and_server->client = &client;

  printf ("\n[1]Lista de usuários da RCA\n[2]Lista de arquivos dos usuarios\n[3]Baixar Aquivos de um servidor\n");
  if (socket_and_server == NULL) {
    error("Erro ao copiar a estrutura do servidor e o socket memoria insuficiente");
  }


  printf("IPDEsSAmerda %s\n", inet_ntoa(me_addr.sin_addr));

  pthread_create(&escritor, NULL, &thread_leitora,  socket_and_server);
  pthread_create(&leitor, NULL, &thread_escritora,  socket_and_server);
  pthread_create(&serv_leitor, NULL, &thread_server_leitora,  socket_and_server);
   
  pthread_join(escritor, NULL);
  pthread_join(leitor, NULL);
  pthread_join(serv_leitor, NULL);

}


/* Imprime mensagens de erro */
void error(char *msg)
{
  printf("%s\n", msg);
  exit(0);

  return;
}
