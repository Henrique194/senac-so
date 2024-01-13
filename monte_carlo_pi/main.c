#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <unistd.h>

static const uint32_t NUM_POINTS = 500000;
static const char *FILE_NAME = "file_points.txt";

static double randfrom(double min, double max) {
    if (min == max) {
        exit(EXIT_FAILURE);
    }

    double rand_num = ( (double)rand()/RAND_MAX ) * (max - min);

    return min + rand_num;
}

static void create_file_points() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        perror("Falha ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

static void read_file_points(uint32_t *inside, uint32_t *outside) {
    if (!inside || !outside) {
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        perror("Falha ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    char file_inside_str[20] = {0};
    char file_outside_str[20] = {0};

    fscanf(file, "INSIDE: %19s\nOUTSIDE: %19s", file_inside_str, file_outside_str);

    *inside = atoi(file_inside_str);
    *outside = atoi(file_outside_str);

    fclose(file);
}

static void write_file_points(uint32_t inside, uint32_t outside) {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        perror("Falha ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "INSIDE: %d\nOUTSIDE: %d", inside, outside);

    fclose(file);
}

static void wait_processes() {
    int status = 0;
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE) {
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    create_file_points();

    pid_t rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Falha ao criar processo\n");
        exit(EXIT_FAILURE);
    }

    // Garantimos que cada processo deve possuir seed
    // diferente ao usar ^ getpid()
    srand((unsigned int) time(NULL) ^ getpid());

    // Cada processo gera NUM_POINTS pontos.
    uint32_t points_inside = 0;
    uint32_t points_outside = 0;

    for (uint32_t i = 0; i < NUM_POINTS; i++) {
        double x = randfrom(0, 1);
        double y = randfrom(0, 1);

        if (pow(x, 2) + pow(y, 2) <= 1) {
            points_inside++;
        }
        else {
            points_outside++;
        }
    }

    // Espera que todos processos filho terminem.
    wait_processes();

    uint32_t file_points_inside = 0;
    uint32_t file_points_outside = 0;
    read_file_points(&file_points_inside, &file_points_outside);

    points_inside += file_points_inside;
    points_outside += file_points_outside;

    // Caso seja um processo filho, atualiza FILE_NAME
    if (rc == 0) {
        write_file_points(points_inside, points_outside);
    }
    else {
        uint32_t total = points_inside + points_outside;
        printf("PI = %.6lf", 4 * ((double)points_inside/ total));
    }

    return EXIT_SUCCESS;
}
