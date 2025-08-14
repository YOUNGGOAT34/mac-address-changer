#include "mchanger.h"

void generate_random_mac(int8 *mac){
      mac[0]=((rand()%256) & 0xFE) | 0x02;
      mac[1]=(rand()%256);
      mac[2]=(rand()%256);
      mac[3]=(rand()%256);
      mac[4]=(rand()%256);
      mac[5]=(rand()%256);
}

int change_mac(const char *ifname,int8 *mac){
   struct ifreq ifrr;
  
   
   int sockfd=socket(AF_INET,SOCK_DGRAM,0);
   strncpy(ifrr.ifr_name,ifname,IF_NAMESIZE);
   // mac_bytes=ether_aton(mac);
   memcpy(ifrr.ifr_hwaddr.sa_data,mac,6);
   ifrr.ifr_hwaddr.sa_family=ARPHRD_ETHER;

   int res=ioctl(sockfd,SIOCSIFHWADDR,&ifrr);
   
   return res;
}
