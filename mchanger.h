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

typedef char i8;
typedef signed int i32;

typedef unsigned char u8;
typedef unsigned short int  u16;
typedef unsigned int  u32;
typedef unsigned long  u64;


#define RED   "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET "\033[0m"


void print_mac(u8 [MACLEN]) ;
void get_perm_address(int sockfd,u8 mac[MACLEN],i8 *ifname);
void get_temp_mac(int sockfd,u8 mac[MACLEN],i8 *ifname);
bool is_interface_up(int sockfd,i8 *ifname);
void generate_random_mac(u8 *);
void change_mac(int,const i8 *,u8 *);
void reset_mac(int,const i8 *);
void bring_interface_down(int,const i8*);
void bring_interface_up(int,const i8*);
