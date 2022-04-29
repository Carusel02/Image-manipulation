#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main() {
    FILE *input = fopen("Imagini/test0.ppm", "rb");
    char type[4];
    int size;
    int size2;
    int max;
    int8_t pixel;
    fscanf(input, "%s%d%d%d", type, &size, &size2, &max);
    printf("%s %d %d %d\n", type, size, size2, max);
    fread(&pixel, sizeof(unsigned char), 1, input);
    printf("%"PRId8"\n", pixel);




    return 0;
}