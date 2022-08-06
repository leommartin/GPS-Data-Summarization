#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "libgps.h"

#define TAM_ALLOC_BIKES 10
#define LINESIZE 51
#define LOGS_OF_BIKE 100

// Bike Operations (create, initialize, search, calculations)

bike_t* alloc_bike()
{
    bike_t *bike;

    bike = malloc(sizeof(bike_t)*TAM_ALLOC_BIKES);

    if(bike != NULL)
        return bike;
    return NULL;
}

log_t* alloc_logs(int nlogs)
{
    log_t *log;

    log = malloc(sizeof(log_t)*nlogs);

    if(log != NULL)
        return log;
    return NULL;
}

int calc_num_logs(char *dir_def)
{
    DIR *dir;
    struct dirent *entry;
    int cont;

    dir = opendir(dir_def);
    entry = readdir(dir);

    while(entry)
    {
        if(strstr(entry->d_name, ".log"))
            cont++;
        entry = readdir(dir);
    }
    return cont;  
}

char *bike_name(char *line)
{
    char *name;

    name = strtok(line, " ");
    name = strtok(NULL, "\0");

    return name;
}

void initialize_bike(bike_t *bike, int i)
{
    bike[i].total_distance = 0.0;
    bike[i].qatv = 0;
    bike[i].longer_pedal = 0.0;
    bike[i].shorter_pedal = 150000000.0; // distance between earth and sun
    bike[i].av_distance = 0.0;
}

bike_t create_bike(bike_t *bike, char *name, int *qtd_bikes, int nlogs)
{
    int i = *qtd_bikes;
    
    bike[i].logs =  malloc(sizeof(log_t) * nlogs);
    bike[i].name = malloc(sizeof(char) * LINESIZE);

    initialize_bike(bike, i);

    strcpy(bike[i].name, name);

    *qtd_bikes = *qtd_bikes + 1;

    return bike[i];
}

void initialize_log(log_t *log, int qtd_logs)
{
    int i = qtd_logs;

    log[i].distance = 0.0;
    log[i].elev = 0.0;
    log[i].speed = 0.0;
    log[i].speed_ant = 0.0;
    log[i].speed_max = 0.0;
    log[i].speed_t = 0.0;
    log[i].sec_effspeed = 0;
    log[i].alt_at = 0.0;
    log[i].alt_ant = 150000000.0;
    log[i].hr_ant = 0;
    log[i].hr_max = 0;
    log[i].hr = 0;
    log[i].hr = 0.0;
    log[i].sec_effhr = 0;
    log[i].cad_t = 0.0;
    log[i].cad = 0;
    log[i].qsec_ant = 0;
    log[i].qsec_at = 0;
    log[i].diff = 0;
    log[i].cad = 0;
    log[i].cad_ant = 0;
    log[i].cad_t = 0;
    log[i].sec_effcad = 0;
}

log_t create_log(log_t *log, int qtd_logs)
{
    int i = qtd_logs;
    
    log[i].date = malloc(sizeof(char) * LINESIZE);

    initialize_log(log, qtd_logs);

    return log[qtd_logs];
}

int search_bike(bike_t *bike, char *name, int *qtd_bikes, int *index)
{
    int i = 0, exists = 0;

    while (i < *qtd_bikes)
    {
        if ((strcmp(bike[i].name, name) == 0))
        {
            exists = 1;
            break;
        }
        i++;
    }
    *index = i;

    return exists;
}

void show_bikes(bike_t *bike, int qtd_bikes)
{
    int i;

    printf("Found Bikes: \n");

    i = 0;
    while (i < qtd_bikes)
    {
        printf("%d: %s\n", i + 1, bike[i].name);
        i++;
    }
    printf("\n");
}

void calc_av_distance_of_each_bike(bike_t *bike, int qtd_bikes)
{
    int i;

    for(i = 0; i < qtd_bikes; i++)
        bike[i].av_distance = bike[i].total_distance / bike[i].qatv;
}

// Data Processing

void find_date(FILE *arquivo, log_t *log, int qtd_logs)
{
    int i = qtd_logs;
    char *line, *key;

    line = malloc(sizeof(char) * LINESIZE);

    fgets(line, LINESIZE, arquivo);
    while (!strstr(line, "timestamp")) 
        fgets(line, LINESIZE, arquivo);

    key = strtok(line, " ");
    key = strtok(NULL, " ");

    strcpy(log[i].date, key);

    free(line);
}

void is_altitude(log_t *log, char *line, int qtd_logs)
{ 
    int i = qtd_logs;
    char *key;

    key = strtok(line, ":");
    key = strtok(NULL, ":");

    log[i].alt_at = atof(key);

    if (log[i].alt_at > log[i].alt_ant)
        log[i].elev = log[i].elev + (log[i].alt_at - log[i].alt_ant);

    log[i].alt_ant = log[i].alt_at;
}

void is_cadence(log_t *log, char *line, int qtd_logs)
{
    int i = qtd_logs;
    char *key;

    if (!(strstr(line, "fractional_cadence")))
    {
        key = strtok(line, ":");
        key = strtok(NULL, "\0");
        log[i].cad = atoi(key);
    }
}

void is_distance(log_t *log, char *line, int qtd_logs)
{
    int i = qtd_logs;
    char *key;

    key = strtok(line, ":");
    key = strtok(NULL, "\0");

    log[i].distance = atof(key) / 1000;
}

void is_heart_rate(log_t *log, char *line, int qtd_logs)
{
    char *key;
    int i = qtd_logs;

    key = strtok(line, ":");
    key = strtok(NULL, ":");
    log[i].hr = atoi(key);

    if (log[i].hr > log[i].hr_max)
        log[i].hr_max = log[i].hr;
}

void is_speed(log_t *log, char *line, int qtd_logs)
{
    char *key;
    int i = qtd_logs;

    key = strtok(line, ":");
    key = strtok(NULL, ":");
    log[i].speed = atof(key) * 3.60;

    if (log[i].speed > log[i].speed_max)
        log[i].speed_max = log[i].speed;
}

void is_timestamp(log_t *log, char *line, int qtd_logs)
{
    char *time_at;
    int i = qtd_logs;
    int hour, min, sec;

    time_at = strtok(line, ":");
    time_at = strtok(NULL, " ");
    time_at = strtok(NULL, " ");
    hour = atoi(strtok(time_at, ":"));
    min = atoi(strtok(NULL, ":"));
    sec = atoi(strtok(NULL, "\n"));

    log[i].qsec_at = (hour * 3600) + (min * 60) + sec;

    if (log[i].qsec_ant != 0)
    {
        log[i].diff = log[i].qsec_at - log[i].qsec_ant;

        if (log[i].diff == 0)
            log[i].diff++;
    }

    if (log[i].cad_ant > 0)
    {
        log[i].cad_t = log[i].cad_t + (log[i].cad_ant * log[i].diff);
        log[i].sec_effcad += log[i].diff;
    }

    if (log[i].hr_ant > 0)
    {
        log[i].hr_t = log[i].hr_t + (log[i].hr_ant * log[i].diff);
        log[i].sec_effhr += log[i].diff;
    }

    if (log[i].speed_ant > 0)
    {
        log[i].speed_t = log[i].speed_t + (log[i].speed_ant * log[i].diff);
        log[i].sec_effspeed += log[i].diff;
    }

    log[i].qsec_ant = log[i].qsec_at;

    log[i].cad_ant = log[i].cad;
    log[i].hr_ant = log[i].hr;
    log[i].speed_ant = log[i].speed;
}

void calc_averages(log_t *log, int qtd_logs)
{
    if(log[qtd_logs].cad_t > 0)
                log[qtd_logs].cad_t =  (log[qtd_logs].cad_t) / (log[qtd_logs].sec_effcad);
            
    if(log[qtd_logs].hr_t > 0)
        log[qtd_logs].hr_t =  (log[qtd_logs].hr_t) / (log[qtd_logs].sec_effhr);

    if(log[qtd_logs].speed_t > 0)
        log[qtd_logs].speed_t =  (log[qtd_logs].speed_t) / (log[qtd_logs].sec_effspeed);
}

void verify_longer_and_shorter_pedals(bike_t *bike, int i, double distance)
{
    if(distance > bike[i].longer_pedal)
        bike[i].longer_pedal = distance;
            
    if(distance < bike[i].shorter_pedal)
        bike[i].shorter_pedal = distance;
}

// Histogram

void fill_with_asterisks(bike_t *bike, int i, int intervalo)
{
    int c, j, lim_intervalo;

    lim_intervalo = intervalo + 10;
    j = 0;

    for(c = 0; c < bike[i].qatv; c++)
    {
        if( (bike[i].logs[j].distance >= intervalo) && (bike[i].logs[j].distance < lim_intervalo))
            printf("*");
        j++;
    }
}

void build_histogram(bike_t *bike, int ind_bike)
{
    int i = ind_bike, cl;
    i--;

    int lim_inf = 10;
    int lim_sup = 10;
    int intervalo1, intervalo2;

    printf("\n\n");

    while(lim_inf <= bike[i].shorter_pedal)
        lim_inf += 10;
    
    if(lim_inf > 10)
        lim_inf -= 10;
    else 
        if((lim_inf == 10) && (lim_inf != bike[i].shorter_pedal))
            lim_inf -= 10; 
    
    while(lim_sup <= bike[i].longer_pedal)
        lim_sup += 10;

    int tam_l = (lim_sup - lim_inf) / 10;

    intervalo1 = lim_inf; 
    intervalo2 = lim_inf + 9;
    for(cl = 0; cl < tam_l; cl++)
    {
        printf("%3d - %3d | ", intervalo1, intervalo2);
        fill_with_asterisks(bike, i, intervalo1);
        intervalo1 += 10;
        intervalo2 += 10;
        printf("\n");
    }

    printf("\nDistância | Quantidade\n\n");
}

// Free's

void release_memory(bike_t *bike, log_t *log, int qtd_bikes, int qtd_logs)
{
    int i;

    for(i = 0; i < qtd_bikes; i++)
    {
        free(bike[i].logs);
        free(bike[i].name);
    }

    for(i = 0; i < qtd_logs; i++)
    {
        free(log[i].date);
    }
}

// Additional Functions

void resume_all_bikes(bike_t *bike, int qtd_bikes)
{
    int i;

    i = 0;
    while (i < qtd_bikes)
    {
        printf("\nBike: %s \n", bike[i].name);

        printf("Total Distance: %.2fkm \n", bike[i].total_distance);
        
        printf("Number of Activities: %d \n", bike[i].qatv);

        printf("Longer Pedal: %.2fkm \n", bike[i].longer_pedal);

        printf("Shorter Pedal: %.2fkm \n", bike[i].shorter_pedal);

        printf("Average distance: %.2fkm \n\n", bike[i].av_distance);

        i++;
    }
}


