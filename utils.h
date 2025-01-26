#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#define SET_ENCODING() system("chcp 65001 > nul")
#else
#include <unistd.h>
#define CLEAR_SCREEN() system("clear")
#define SET_ENCODING() do {} while(0)
#endif

#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#endif // UTILS_H
