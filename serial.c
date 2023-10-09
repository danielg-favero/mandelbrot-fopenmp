#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 15360
#define HEIGHT 8640

unsigned char * mandelbrot() {
    int i, j;
    
    int iterations = 100;
    int k;

    double x;
    double y;
    double zReal;
    double zImg;
    int value;
    double r2;
    double i2;
    double dist2;
    unsigned char color;

    unsigned char *image = (unsigned char *)malloc(WIDTH * HEIGHT * sizeof(unsigned char));

    for(i = 0; i < HEIGHT; i++) {
        for(j = 0; j < WIDTH; j++) {
            zReal = 0;
            zImg = 0;

            value = iterations;

            x = (j - WIDTH / 2.0) * 4.0 / WIDTH;
            y = (i - HEIGHT / 2.0) * 4.0 / WIDTH;

            for (k = 0; k < iterations; k++) {
                r2 = zReal * zReal;
                i2 = zImg * zImg;
                dist2 = r2 + i2;

                if (dist2 >= 4.0) {
                    value = k;
                    k = iterations;
                }

                zImg = 2 * zReal * zImg + y;
                zReal = r2 - i2 + x;
            }

            color = (unsigned char)(255 * (value / (double)iterations));
            image[i * WIDTH + j] = color;
        }
    }

    return image;
}

int main() {
    unsigned char *image  = mandelbrot();

    FILE *file = fopen("mandelbrot.pgm", "wb");
    fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, sizeof(unsigned char), WIDTH * HEIGHT, file);
    fclose(file);

    free(image);

    return 0;
}