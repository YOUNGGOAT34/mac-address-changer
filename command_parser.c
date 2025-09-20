#include "mchanger.h"
#include <getopt.h>
#include "command_parser.h"

void parser(i32 argc,i8 *argv[]){
 
   static struct option long_options[]={
          {"change",required_argument,0,'c'},
          {"restore",no_argument,0,'R'},
          {"print",required_argument,0,'p'},
          {"permanent",no_argument,0,'P'},
          {"random",no_argument,0,'r'},
          {"status",no_argument,0,'s'},
          {"down",no_argument,0,'d'},
          {"up",no_argument,0,'u'},
          {"list",no_argument,0,'l'},
          {"help",no_argument,0,'h'},
          {0,0,0,0}
   };

if(argc<2){
        help();
 }

if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"--list")==0 || strcmp(argv[1],"-list")==0){
      double_hyphen_for_long_opt_rule(argc,argv);
      get_all_interfaces();
      exit(EXIT_SUCCESS);
   }else{
      double_hyphen_for_long_opt_rule(argc-1,argv+1);
   }
   
   i32 sockfd=socket(AF_INET,SOCK_DGRAM,0);
   if(sockfd<0){
      fprintf(stderr,RED"Socket creation failed\n"RESET);
      exit(1);
   }
   
   const i8 *ifname=argv[1];

   i32 option;
    u8 mac[MACLEN];
    srand(getpid());
   
   while((option=getopt_long(argc-1,argv+1,"c:Rp:Prhsdu",long_options,NULL))!=-1){
           switch(option){
               case 'c':
                    if(strcmp(optarg,"r")==0 || strcmp(optarg,"random")==0){
                         generate_random_mac(mac);
                         change_mac(sockfd,ifname,mac);
                      }else{
                          if(validate_mac(optarg)){
                             parse_mac(optarg,mac);
                             change_mac(sockfd,ifname,mac);
                          }else{
                            fprintf(stderr,RED"Invalid MAC address\n"RESET);
                            exit(0);
                          }
                      }
                    
                  break;
               case 'r':
                      generate_random_mac(mac);
                      change_mac(sockfd,ifname,mac);
                      break;
               case 'R':
                      reset_mac(sockfd,ifname);
                      break;
               case 'P':
                     get_perm_address(sockfd,mac,(i8 *)ifname);
                     printf(GREEN"Permanent MAC for %s: "RESET,ifname);
                     print_mac(mac);
                     break;

               case 'p':
                      
                      if(strcmp(optarg,"c")==0 || strcmp(optarg,"current")==0){
                             get_temp_mac(sockfd,mac,(i8 *)ifname);
                             printf(GREEN"current MAC for %s: "RESET,ifname);
                             print_mac(mac);
                      }else if(strcmp(optarg,"p")==0 || strcmp(optarg,"permanent")==0){
                              get_perm_address(sockfd,mac,(i8 *)ifname);
                              printf(GREEN"Permanent MAC for %s: "RESET,ifname);
                              print_mac(mac);
                      }
                      break;

               case 's':
                      if(is_interface_up(sockfd,(i8 *)ifname)){
                         printf(GREEN"UP\n"RESET);
                      }else{
                        printf(RED"DOWN\n"RESET);
                      }
                      break;

               case 'd':
                     bring_interface_down(sockfd,(const i8*)ifname);
                     break;

               case 'u':
                     bring_interface_up(sockfd,(const i8*)ifname);
                     break;
               case 'l':
                      get_all_interfaces();
                      break;

               case 'h':
                        help();
                        break;

               default:
                        fprintf(stderr,RED"Unknown option\n"RESET);
                        exit(EXIT_FAILURE);
                        break;
               
           }
   }

              
}


void get_all_interfaces(void){
   struct ifaddrs *if_head,*current_iff;
   i8 *seen[128];
   i32 iff_count=0;
   if(getifaddrs(&if_head)==-1){
      fprintf(stderr,RED"Error getting available interfaces: %s\n"RESET,strerror(errno));
      exit(EXIT_FAILURE);
   }

   for(current_iff=if_head;current_iff!=NULL;current_iff=current_iff->ifa_next){
              if(current_iff->ifa_addr==NULL) continue;
              i32 duplicate=0;
               for(i32 i=0;i<iff_count;i++){
                     if(strcmp(current_iff->ifa_name,seen[i])==0){
                        duplicate+=1;
                         break;
                     }
               }
               
               if(duplicate) continue;
               seen[iff_count++]=current_iff->ifa_name;
              printf(GREEN"%s\n"RESET,current_iff->ifa_name);
   }
}


bool validate_mac(i8 *mac){
    u32 bytes[MACLEN];
    i8 temp;
    if(sscanf(mac,"%x:%x:%x:%x:%x:%x%c",&bytes[0],&bytes[1],&bytes[2],&bytes[3],
                                      &bytes[4],&bytes[5],&temp
   )==7){
           return false;
   }
  

   if (sscanf(mac, "%x:%x:%x:%x:%x:%x",
      &bytes[0], &bytes[1], &bytes[2],
      &bytes[3], &bytes[4], &bytes[5]) != 6) {
     return false;
   }


   for(int i=0;i<MACLEN;i++){
      if(bytes[i]>255) return false;
   }

    if((bytes[0] & 0x01)) return false; // I/G
    if((bytes[0] & 0x02)) return false; // U/L

    return true;
}

void parse_mac(i8 *str,u8 mac[MACLEN]){
    sscanf(str,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
}

void double_hyphen_for_long_opt_rule(i32 argc,i8 *argv[]){
     const i8 *long_opts[]={"help","random","up", "down","print","permanent","change","restore","list"};
     for(i32 i=0;i<argc;i++){
        
         if(argv[i][0]=='-' && argv[i][1]!='-' && strlen(argv[i])>2){
             i32 size=sizeof(long_opts)/sizeof(long_opts[0]);
             for(i32 j=0;j<size;j++){
                  if(strcmp(argv[i]+1,long_opts[j])==0){
                      fprintf(stderr,RED"Invalid option %s .Did you mean --%s?\n"RESET,argv[i],long_opts[j]);
                      
                      exit(EXIT_FAILURE);
                  }
             }
         }
     }
}

void help(void){
   printf("Usage: ./main <interface> [options]\n");
   printf("Options:\n");
   printf("  -c, --change <MAC>     Change MAC address\n");
   printf("  -r, --random           Generate random MAC\n");
   printf("  -R, --restore          Restore original MAC\n");
   printf("  -p, --print [type]     Print MAC (optional type)\n");
   printf("  -P, --permanent        Use permanent MAC\n");
   printf("  -d, --down             bring an interface down\n");
   printf("  -u, --up               bring an interface up\n");
   printf("  -h, --help             Show this help message\n");
   exit(0);
}


