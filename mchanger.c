#include "mchanger.h"

void generate_random_mac(uint8 *mac){
      mac[0]=((rand()%256) & 0xFE) | 0x02;
      mac[1]=(rand()%256);
      mac[2]=(rand()%256);
      mac[3]=(rand()%256);
      mac[4]=(rand()%256);
      mac[5]=(rand()%256);
}

void change_mac(int sockfd,const int8 *ifname,uint8 *mac){
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

void get_temp_mac(int sockfd,uint8 mac[MACLEN],int8 *ifname){
   struct ifreq ifrr;
   strncpy(ifrr.ifr_name,ifname,IFNAMSIZ-1);

   if(ioctl(sockfd,SIOCGIFHWADDR,&ifrr)==-1){
      fprintf(stderr,"Error retriving the current MAC: %s\n",strerror(errno));
      exit(1);
   }
   memcpy(mac,ifrr.ifr_hwaddr.sa_data,MACLEN);
}

void get_perm_address(int sockfd,uint8 mac[MACLEN],int8 *ifname){
   struct ethtool_perm_addr *permanent_addr;
   struct ifreq ifrr;
   permanent_addr=malloc(sizeof(struct ethtool_perm_addr )+32);
   permanent_addr->cmd=ETHTOOL_GPERMADDR;
   permanent_addr->size=32;
    
   strncpy(ifrr.ifr_name,ifname,IFNAMSIZ-1);
   ifrr.ifr_data=(caddr_t)permanent_addr;
   
   if(ioctl(sockfd,SIOCETHTOOL,&ifrr)==-1){
      fprintf(stderr,"Error  retriving the original MAC: %s\n",strerror(errno));
      free(permanent_addr);
      exit(1);
   }
    

   memcpy(mac,permanent_addr->data,MACLEN); 
   free(permanent_addr);


}

bool is_interface_up(int sockfd,int8 *ifname){
      struct ifreq ifrr;
      strncpy(ifrr.ifr_name,ifname,IFNAMSIZ-1);

      if(ioctl(sockfd,SIOCGIFFLAGS,&ifrr)==-1){
         fprintf(stderr,"Error  retriving the interface flags : %s\n",strerror(errno));
         exit(0);
      }

      return ((ifrr.ifr_flags & IFF_UP) && (ifrr.ifr_flags & IFF_RUNNING));
}


void print_mac(uint8 mac[MACLEN]) {
   printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void reset_mac(int sockfd,const int8 *ifname){
   struct ethtool_perm_addr *permanent_addr;
   struct ifreq ifrr;
   permanent_addr=malloc(sizeof(struct ethtool_perm_addr )+32);
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


void bring_interface_up(int sockfd,const int8 *ifname){
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


void bring_interface_down(int sockfd,const int8 *ifname){
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



