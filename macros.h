#ifndef MACROS_H
#define MACROS_H

#define LOG_ERROR(msg,...) printf("[-] " msg "\n",##__VA_ARGS__)
#define LOG_OKAY(msg,...) printf("[+] " msg "\n",##__VA_ARGS__)

#endif