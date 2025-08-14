#include "mchanger.h"



int main(){
    int8 mac[MACLEN];
   srand(getpid());
    generate_random_mac(mac);

    printf("setting %x:%x:%x:%x:%x:%x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    if(change_mac("wlan0",mac)==0){
         printf("successful\n");
    }else{ 
      printf("FAILED %s\n",strerror(errno));


    }
    return 0;

}