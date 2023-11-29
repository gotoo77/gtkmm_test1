//
// Created by gotoo77 on 05/08/2023.
//

#ifndef COND_PRINT_HPP
#define COND_PRINT_HPP

#define STR(src) #src

#define DEBUG_IF(EXP) \
    do { if (EXP) \
            std::cout << "[DBG]: " << #EXP << std::endl; } \
    while (0)

#define INFO_IF(EXP) \
    do { if (EXP) \
            std::cout << "[INF]: " << #EXP << std::endl; } \
    while (0)

#define WARN_IF(EXP) \
     do { if (EXP) \
             std::cout << "[WRN]: " << #EXP << std::endl; } \
     while (0)

#define ERROR_IF(EXP) \
    do { if (EXP) \
            std::cerr << "[ERR]: " << #EXP << std::endl; } \
    while (0)

#endif //COND_PRINT_HPP
