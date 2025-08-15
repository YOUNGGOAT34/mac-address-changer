#include "mchanger.h"

void generate_random_mac(int8 *mac){
      mac[0]=((rand()%256) & 0xFE) | 0x02;
      mac[1]=(rand()%256);
      mac[2]=(rand()%256);
      mac[3]=(rand()%256);
      mac[4]=(rand()%256);
      mac[5]=(rand()%256);
}

void change_mac(int sockfd,const char *ifname,int8 *mac){
   struct ifreq ifrr;
  
   
   
   bring_interface_down(sockfd,ifname);
      
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


   bring_interface_up(sockfd,ifname);

}



void get_permanet_mac(int sockfd,const char *ifname){
   struct ethtool_perm_addr *permanent_addr;
   struct ifreq ifrr;
   permanent_addr=malloc(sizeof(struct ethtool_perm_addr *)+32);
   permanent_addr->cmd=ETHTOOL_GPERMADDR;
   permanent_addr->size=32;
 

   
   strncpy(ifrr.ifr_name,ifname,IFNAMSIZ-1);
   ifrr.ifr_data=(caddr_t)permanent_addr;
   
   if(ioctl(sockfd,SIOCETHTOOL,&ifrr)==-1){
      fprintf(stderr,"Error  retriving the original MAC: %s\n",strerror(errno));
      free(permanent_addr);
      exit(1);
   }

   
   bring_interface_down(sockfd,ifname);
   
   memset(&ifrr,0,sizeof(ifrr)); 
   ifrr.ifr_hwaddr.sa_family=ARPHRD_ETHER;
   strncpy(ifrr.ifr_name,ifname,IF_NAMESIZE);    
   memcpy(ifrr.ifr_hwaddr.sa_data,permanent_addr->data,6);
  
  if(ioctl(sockfd,SIOCSIFHWADDR,&ifrr)){
      fprintf(stderr,"Error setting the back the old MAC address: %s\n",strerror(errno));
      exit(1);
   }


   printf("MAC restored successfully\n");
   sleep(1);

   bring_interface_up(sockfd,ifname);



}


void bring_interface_up(int sockfd,const char* ifname){
   struct ifreq ifrr;
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


void bring_interface_down(int sockfd,const char* ifname){
   struct ifreq ifrr;
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

}



