/*
 * mandelmovie.c
 * Logan Amar
 * CPE 2600 Lab 11 - Multiprocessing
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_FRAMES 50
#define START_SCALE 4.0
#define SCALE_FACTOR 0.7

// Print the command line options
static void show_help (char *prog){
    printf("Usage: %s [-n num_processes] [-h]\n", prog);
    printf("  -n  number of child processes to run at once (default 1)\n");
    printf("  -h  show this help\n");
}

int main (int argc, char *argv[]){

    int num_procs = 1;
    int c;

    // Process command line options
    while ((c = getopt(argc, argv, "n:h")) != -1) {
        switch (c) {
            case 'n':
                num_procs = atoi(optarg);
                break;
            case 'h':
                show_help(argv[0]);
                exit(0);
            default:
                show_help(argv[0]);
                exit(1);
        }
    }

    if (num_procs < 1) {
        fprintf(stderr, "Number of processes must be at least 1\n");
        exit(1);
    }

    // Center of the zoom
    const char *xstr = "-0.743643887037151";
    const char *ystr = "0.131825904205330";

    double scale = START_SCALE;
    int running = 0;

    for (int i = 0; i < NUM_FRAMES; i++) {
        char sstr[32];
        char fname[32];
        snprintf(sstr, sizeof(sstr), "%f", scale);
        snprintf(fname, sizeof(fname), "mandel%d.jpg", i + 1);

        if (running >= num_procs) {   // all slots busy, wait for one
            wait(NULL);
            running--;
        }
        pid_t pid = fork();
        if (pid == 0) {
            execl("./mandel", "mandel", "-x", xstr, "-y", ystr,
                "-s", sstr, "-W", "800", "-H", "600",
                "-o", fname, NULL);
            perror("execl");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
        running++;
        scale *= SCALE_FACTOR;        // zoom in for the next frame
    }

    while (running > 0) {         // wait for the stragglers
        wait(NULL);
        running--;
    }

    return 0;
}