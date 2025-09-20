#include "mchanger.h"
#include <ifaddrs.h>

void parser(i32,i8 *[]);
void get_all_interfaces(void);
bool validate_mac(i8 *);
void parser_mac(i8 *,u8 [MACLEN]);
void double_hyphen_for_long_opt_rule(i32 argc,i8 *argv[]);
void help(void);