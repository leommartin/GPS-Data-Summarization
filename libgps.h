#include <stdio.h>
#ifndef _GPS_t_H
#define _GPS_t_H

struct log {

    char *date;

    double alt_at;
    double alt_ant;

    double cad_t; 
    int cad;
    int cad_ant;
    long int sec_effcad;

    double distance;
    double elev;

    double speed;
    double speed_max;
    double speed_t;
    double speed_ant;
    long int sec_effspeed;

    double hr_t;
    int hr_max;
    int hr;
    int hr_ant;
    long int sec_effhr;
    
    long int qsec_at;
    long int qsec_ant;
    int diff;
};
typedef struct log log_t;

struct bike {
    char *name;
    int qatv;
    double total_distance;
    double longer_pedal;
    double shorter_pedal;
    double av_distance;
    log_t *logs;
};
typedef struct bike bike_t;

// Bikes Operations
bike_t* alloc_bike();

log_t* alloc_logs(int nlogs);

int calc_num_logs(char *dir_def);

char* bike_name(char *line);

void find_date(FILE *arquivo, log_t *log, int qtd_logs);

void initialize_log(log_t *log, int i);

void initialize_bike(bike_t *bike, int i);

log_t create_log(log_t *log, int qtd_logs);

bike_t create_bike(bike_t *bike, char *name, int *qtd_bikes, int nlogs);

int search_bike(bike_t *bike, char *name, int *qtd_bikes, int *index);

// Bikes Data

void show_bikes(bike_t *bike, int qtd_bikes);

void resume_all_bikes(bike_t *bike, int qtd_bikes);

void calc_av_distance_of_each_bike(bike_t *bike, int qtd_bikes);

// Logs Data

void is_altitude(log_t *log, char *line, int qtd_logs);

void is_cadence(log_t *log, char *line, int qtd_logs);

void is_distance(log_t *log, char *line, int qtd_logs);

void is_heart_rate(log_t *log, char *line, int qtd_logs);

void is_speed(log_t *log, char *line, int qtd_logs);

void is_timestamp(log_t *log, char *line, int qtd_logs);

void calc_averages(log_t *log, int qtd_logs);

void verify_longer_and_shorter_pedals(bike_t *bike, int i, double distance);

// Sort 

void sort_per_elev(log_t *log, int qtd_logs);

void sort_per_distance(bike_t *bike, int qtd_bikes);

void sort_per_date(bike_t *bike, int qtd_bikes);

// Print

void print_summary();

void print_info_of_bike(bike_t *bike, int choose, int qtd_bikes);

void print_info_add(bike_t *bike, int choose, int qtd_bikes);

void print_logs_sorted(log_t *log, int qtd_logs);

void print_bikes_sorted(bike_t *bike, int qtd_bikes);

void build_histogram(bike_t *bike, int ind_bike);

// Release Memory

void release_memory(bike_t *bike, log_t *log, int qtd_bikes, int qtd_logs);

#endif