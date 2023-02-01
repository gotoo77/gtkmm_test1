#include <string>

#define SPACE_STR   " \t\v\f\r\n"

char *left_trim(char *src);

char *right_trim(char *src);

char *trim(char *src);

std::string &left_trim(std::string &s);

std::string &right_trim(std::string &s);

std::string &trim(std::string &s);

bool getKeyVal(std::string &line, char sep, std::string &name, std::string &value, char comment = '#');
