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
#include <errno.h>


#define MACLEN 6


typedef unsigned char int8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint8_t  uint8;
typedef uint64_t  uint64;



void generate_random_mac(int8 *);
void change_mac(const char *ifname,int8 *mac);
