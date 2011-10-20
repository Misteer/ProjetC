#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#define PORTSERVER 4444
//#define BROADCASTADDR "10.0.2.255"
#define PORTDEST   4444
  
                          
int main(){
        int sockfd; //notre socket
        struct sockaddr_in host_addr,broadcast_addr; // on defint les addresse
        socklen_t size;         //taille de nos structure ,utile pour certaines fonction
        int optSetsockOption = 1 ;      //option pour la fonction setsockopt
        char buffer[] = "HELLO WORLD";
        int addrSize = sizeof(struct sockaddr_in);
        int bufferLen = strlen(buffer);
        if((sockfd = socket(PF_INET,SOCK_DGRAM,0)) == -1){
                printf("error in creation socket\n");
                return -1;    
         }    
        if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optSetsockOption,sizeof(int))){  //SO_REUSEADDR forcer a utiliser un port meme deja utilisé
                printf("error in setting option SO_REUSEADDR to sockfd");
                return -1;
         }
        if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,&optSetsockOption, sizeof(int)))
                 printf("in setting option SO_BROADCAST to sockfd");
        memset(&host_addr,0,sizeof(struct sockaddr));
        memset(&broadcast_addr,0,sizeof(struct sockaddr));
        /*server
        host_addr.sin_family = AF_INET;
        host_addr.sin_port = htons(PORTSERVER);
        host_addr.sin_addr.s_addr = htonl(0);*/
        //destination
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(PORTDEST);
        broadcast_addr.sin_addr.s_addr =INADDR_BROADCAST;// htonl(BROADCASTADDR); 
        
        while(1){      //boucle
                
                int NbBytes = sendto(sockfd, buffer, bufferLen, 0, (struct sockaddr *) &broadcast_addr, addrSize);
                if (NbBytes < 0){
                 printf("sendto() error");
                 return -1;
                }
                else if (NbBytes != bufferLen){
                printf("sendto() sent unexpected number of bytes");
                return -1;
                }
                sleep(3);   // eviter le flood
        }
        
        return 0;            
}
