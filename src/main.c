#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <libgen.h>
#include <getopt.h>
#include <fcntl.h>

#include "md5sum.h"

#define LOG(LEVEL, FMT, ...)                                                     \
    do {                                                                         \
        fprintf(stderr, "(%s:%d) " FMT "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define PRINT_DEBUG(FMT, ...) LOG(LOG_DEBUG, FMT, ##__VA_ARGS__)
#define PRINT_ERROR(FMT, ...) LOG(LOG_ERR, FMT, ##__VA_ARGS__)

static void print_usage(const char *exe_name) {
    printf("File md5sum tools\r\n");
    printf("Usage: %s [options] <file>...\r\n", exe_name);
    printf("Options:\r\n");
    printf("    -h,--help                        Show this help message.\r\n");
    printf("    -o <OFFSET>,--offset=<OFFSET>    Offset of file to calc md5.\r\n");
    printf("    -l <LENGTH>,--length=<LENGTH>    Length of file from offset.\r\n");
}

int main(int argc, char **argv) {
    int ret = 0;
    int idx = 0;
    char *file = NULL;
    FILE *fp = NULL;
    int32_t filelen = 0;
    int32_t buflen = 0;
    uint8_t *input = NULL;
    uint8_t output[64] = {0};

    int opt = 0, opt_index = 0;

    int32_t b_offset = -1, b_length = -1;

    static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                           {"offset", required_argument, 0, 'o'},
                                           {"length", required_argument, 0, 'l'},
                                           {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "l:o:h", long_options, &opt_index)) != -1) {
        switch (opt) {
            case 0:
                if (strcmp("offset", long_options[opt_index].name) == 0) {
                    b_offset = atol(optarg);
                }
                if (strcmp("length", long_options[opt_index].name) == 0) {
                    b_length = atol(optarg);
                }
                break;
            case 'o':
                b_offset = atol(optarg);
                break;
            case 'l':
                b_length = atol(optarg);
                break;
            case 'h':
                ret = 1;
                goto err;
            default:
                PRINT_DEBUG("Unknown option -- %c\n\n", opt);
                goto err;
        }
    }

    if (optind >= argc) {
        ret = 1;
        goto err;
    }

    // argc -= optind;
    // argv += optind;

    file = argv[optind];

    fp = fopen(file, "r");
    if (fp == NULL) {
        PRINT_ERROR("Failed to open file [%s]!", file);
        ret = -1;
        goto err;
    }
    fseek(fp, 0, SEEK_END);
    filelen = ftell(fp);

    if (filelen <= 0) {
        PRINT_ERROR("Failed to get file [%s] size!", file);
        ret = -1;
        goto err;
    }

    if ((b_offset <= 0) || (b_offset >= filelen)) {
        b_offset = 0;
    }
    fseek(fp, b_offset, SEEK_SET);

    if (b_length <= 0) {
        b_length = filelen;
    }

    if (b_length > (filelen - b_offset)) {
        b_length = (filelen - b_offset);
    }
    buflen = b_length;

    PRINT_DEBUG("Get file [%s] size [%d], offset [%d], length [%d]!", file, filelen, b_offset, buflen);

    input = malloc(buflen);
    if (input == NULL) {
        PRINT_ERROR("Failed to malloc!");
        ret = -1;
        goto err;
    }
    memset(input, 0, buflen);

    filelen = fread(input, 1, buflen, fp);
    if (filelen != buflen) {
        PRINT_ERROR("Failed to read file to buff [%d] - [%u]!\n", filelen, buflen);
        ret = -1;
        goto err;
    }

    md5sum_str(input, buflen, output);
    PRINT_DEBUG("md5sum of file [%s]:", file);
    printf("%s\n", output);

    ret = 0;

err:
    if (input != NULL) {
        free(input);
    }
    if (fp != NULL) {
        fclose(fp);
    }
    if (ret) {
        print_usage(argv[0]);
    }
    return ret;
}
