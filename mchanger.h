#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/ether.h>
#include<sys/socket.h>
#include <sys/ioctl.h>
#include<linux/ethtool.h>
#include <errno.h>
#include <linux/sockios.h>


#define MACLEN 6


typedef char int8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint8_t  uint8;
typedef uint64_t  uint64;


#define RED   "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET "\033[0m"


void print_mac(uint8 [MACLEN]) ;
void get_perm_address(int sockfd,uint8 mac[MACLEN],int8 *ifname);
void get_temp_mac(int sockfd,uint8 mac[MACLEN],int8 *ifname);
bool is_interface_up(int sockfd,int8 *ifname);
void generate_random_mac(uint8 *);
void change_mac(int,const int8 *,uint8 *);
void reset_mac(int,const int8 *);
void bring_interface_down(int,const int8*);
void bring_interface_up(int,const int8*);
