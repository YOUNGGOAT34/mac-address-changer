#include "mchanger.h"

void generate_random_mac(int8 *mac){
      mac[0]=((rand()%256) & 0xFE) | 0x02;
      mac[1]=(rand()%256);
      mac[2]=(rand()%256);
      mac[3]=(rand()%256);
      mac[4]=(rand()%256);
      mac[5]=(rand()%256);
}

void change_mac(const char *ifname,int8 *mac){
   struct ifreq ifrr;
  
   
   int sockfd=socket(AF_INET,SOCK_DGRAM,0);
   memset(&ifrr,0,sizeof(ifrr));    
   strncpy(ifrr.ifr_name,ifname,IF_NAMESIZE);
 
   //if the interface is down ,it has to be turned up for its MAC to be changed
   if(ioctl(sockfd,SIOCGIFFLAGS,&ifrr)<0){

      fprintf(stderr,"Error getting the interface flags: %s\n",strerror(errno));
      exit(1);
   }
   
     
      if(ifrr.ifr_flags & IFF_UP){
         ifrr.ifr_flags &=~IFF_UP;

         if(ioctl(sockfd,SIOCSIFFLAGS,&ifrr)<0){
             fprintf(stderr,"Error bringing down the interface: %s\n",strerror(errno));
             exit(1);
         }
         printf("Interface brought down successfully\n");
         sleep(1);
      }
      
   memset(&ifrr,0,sizeof(ifrr)); 
   ifrr.ifr_hwaddr.sa_family=ARPHRD_ETHER;
   strncpy(ifrr.ifr_name,ifname,IF_NAMESIZE);    
   memcpy(ifrr.ifr_hwaddr.sa_data,mac,6);
   if(ioctl(sockfd,SIOCSIFHWADDR,&ifrr)){
      fprintf(stderr,"Error setting the new MAC address: %s\n",strerror(errno));
      exit(1);
   }
   printf("MAC changed successfully\n");
   sleep(1);

   
   //bring the interface back up
   memset(&ifrr,0,sizeof(ifrr)); 
   strncpy(ifrr.ifr_name,ifname,IF_NAMESIZE);  
   if(!(ifrr.ifr_flags & IFF_UP)){
       ifrr.ifr_flags |=IFF_UP;
       if(ioctl(sockfd,SIOCSIFFLAGS,&ifrr)<0){
          fprintf(stderr,"Error  bringing the interface up: %s\n",strerror(errno));
          exit(1);
       }
      
       printf("Interface brought up successfully\n");
       sleep(1);

   }

   
   
}
