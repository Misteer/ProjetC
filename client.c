#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBUFFERLEN 512
int creer_socket_udp();
int  bind_socket_udp(int sockfd,int port);
int handleClient(int,struct sockaddr_in,socklen_t*);
int server_ClientSide();

int main(){
    if( server_ClientSide() == -1)
        return -1;
        return 0;
        
}
int server_ClientSide(){
        char buffer[MAXBUFFERLEN];
        int nbrReceiv;
        struct sockaddr_in client_addr;
        socklen_t size;
        int sockfdClient ; //socket pour accueilleur un nouveau slot/client
        int sockfd ;
        if((sockfd = creer_socket_udp()) == -1){
                printf("[.] Debug server_clientSide 1 : erreur creating socket\n");
                return -1;
        }
        else
                 printf("[X] Creation of socket done.\n");
        if(bind_socket_udp(sockfd,4445) == -1)  {
                printf("[.] Debug server_clientSide 2 : erreur binding\n");
                return -1;
         }
         else
                printf("[X] Binding port done.\n");
             
        size = sizeof(struct sockaddr);
        while(1){
         if ((nbrReceiv=recvfrom(sockfd, buffer, MAXBUFFERLEN, 0,(struct sockaddr *)&client_addr, &size)) == -1) {
             printf("[.] Debug server_clientSide 3 : Error receiving\n");
             return -1;
          }
          handleClient(sockfd,client_addr,&size);
        }
}      

       
//Renvoie un socket non connecté (udp) ,ou -1 si le creation a echoué
int creer_socket_udp(int port){
        int optSetsockOption = 1 ;
        int sockfd;      //option pour la fonction setsockopt
        if((sockfd = socket(PF_INET,SOCK_DGRAM,0)) == -1){
                printf("[.] Debug creer_socket_udp() 1:Error in creation socket\n");
                return -1;    
         }    
        if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optSetsockOption,sizeof(int))){  //SO_REUSEADDR forcer a utiliser un port meme deja utilisé,en cas du
        // redemarrage du serveur
                printf("[.] Debug creer_socket_udp() 2: error in setting option SO_REUSEADDR to sockfd");
                return -1;
         }
        return sockfd;
}
//lie la socket au port PORT et a l'interface INADDR_aNY
int bind_socket_udp(int sockfd,int port){
	struct sockaddr_in host_addr;	// informations sur l'adresse de l'hote
	memset(&host_addr,0,sizeof(struct sockaddr)); //on initialise toute la structure a zero;
	host_addr.sin_family = AF_INET;		
	host_addr.sin_port = htons(port);	 // host to network
	host_addr.sin_addr.s_addr = INADDR_ANY; // repmplit automatiquement avec l'ip de mon interface
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1){
		printf("[.] DEBUG bind_socket_udp() 1: Erreur binding socket udp \n");
		return -1;
	}
	return 0;
}

//s'occupe de recevoir ce que envoit le client
int handleClient(int sockfd,struct sockaddr_in client_addr,socklen_t *size){
     fprintf(stdout,"===== IP : %s === PORT : %d === ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
     sendto(sockfd, "HELLO WORLD!\n", 12,0,  (struct sockaddr *)&client_addr, *size);
     return 0;
}
