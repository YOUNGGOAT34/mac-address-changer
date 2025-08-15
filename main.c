#include "mchanger.h"

//ability to restore the original mac ??????????


int main(int argc,char *argv[]){
    if(argc<2){
         printf("Usage: ./main <Network Interface i.e wlan0>\n");
         return 2;
    }
     
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
     if(sockfd<0){
         fprintf(stderr,"Socket creation failed\n");
         exit(1);
     }
    
    get_permanet_mac(sockfd,argv[1]);
    
    int8 mac[MACLEN];
    srand(getpid());
    generate_random_mac(mac);
    change_mac(sockfd,argv[1],mac);
    return 0;
}