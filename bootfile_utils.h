#ifndef BOOTFILE_UTILS_H
#define BOOTFILE_UTILS_H

#include <stdio.h>
#include <stdbool.h>

#define BOOTFILE_EXCEPTIONS (sizeof(exceptions) / sizeof(exceptions[0]))

extern const unsigned char SCES_017_03[];
#define SCES_017_03_SIZE 11

extern const unsigned char SCES_017_04[];
#define SCES_017_04_SIZE 11

extern const unsigned char SCES_018_82[];
#define SCES_018_82_SIZE 11

bool handle_bootfile_exceptions(unsigned int bin_size, FILE *bin, char *bootfile);

#endif