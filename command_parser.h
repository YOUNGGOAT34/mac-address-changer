#include "mchanger.h"

void parser(int,char *[]);
bool validate_mac(char *);
void parser_mac(char *,uint8 [MACLEN]);
void double_hyphen_for_long_opt_rule(int argc,char *argv[]);
void help();