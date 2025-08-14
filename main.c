#include "mchanger.h"



int main(){
    int8 mac[MACLEN];
   srand(getpid());
    generate_random_mac(mac);
    change_mac("wlan0",mac);
    return 0;

}