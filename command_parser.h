#include "mchanger.h"
#include <ifaddrs.h>

void parser(int,char *[]);
void get_all_interfaces(void);
bool validate_mac(char *);
void parser_mac(char *,uint8 [MACLEN]);
void double_hyphen_for_long_opt_rule(int argc,char *argv[]);
void help(void);