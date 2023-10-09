#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define WIDTH 15360
#define HEIGHT 8640

unsigned char * mandelbrot(int numThreads) {
    float startTime, endTime;
    float parallelTime[numThreads];
    int i, j;

    for(i = 0; i < numThreads; i++) {
        parallelTime[i] = 0;
    }
    

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

    #pragma omp parallel for private(i, j, zReal, zImg, value, x, y, r2, i2, dist2, color, k, startTime, endTime) shared(image, parallelTime) num_threads(numThreads)
    for(i = 0; i < HEIGHT; i++) {
        for(j = 0; j < WIDTH; j++) {
            startTime = omp_get_wtime();
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
            endTime = omp_get_wtime();

            parallelTime[omp_get_thread_num()] += (endTime - startTime);

            #pragma omp critical
            {
                color = (unsigned char)(255 * (value / (double)iterations));
                image[i * WIDTH + j] = color;
            }
        }
    }

    for(i = 0; i < numThreads; i++) {
        printf("Thread: %d\t %.2fs\n", i, parallelTime[i]);
    }

    return image;
}

int main() {
    unsigned char *image  = mandelbrot(8);

    FILE *file = fopen("mandelbrot.pgm", "wb");
    fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, sizeof(unsigned char), WIDTH * HEIGHT, file);
    fclose(file);

    free(image);

    return 0;
}