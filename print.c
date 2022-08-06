#include <stdio.h>
#include "libgps.h"

void print_summary()
{
    printf("\n-----GPS DATA SUMMARIZATION-----\n\n");
    printf("1. Show found bikes. \n");
    printf("2. Choose one of the bikes found and present the activities list. \n");
    printf("3. List all activities of each bike sorted by date. \n");
    printf("4. List all activities of each bike sorted by distance. \n");
    printf("5. List all activities sorted by accumulated ascent. \n");
    printf("6. Choose one of the bikes found and build a distance histogram. \n");
    printf("7. Resume all bikes \n\n");
}

void print_logs_sorted(log_t *log, int qtd_logs)
{
    int i;

    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("Data\t    | Distância(km) | Velocidade Média(km/h) | Velocidade Máxima(km/h) | Hr Médio(bpm) | Hr Max(bpm) | Cadência(rpm) | Subida Acumulada(m)\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------\n");

    i = 0;
    while (i < qtd_logs)
    {
        printf("%s %12.2f %19.2f ", log[i].date, log[i].distance, log[i].speed_t); 
        printf("%25.2f %19.0f ", log[i].speed_max, log[i].hr_t);
        printf("%14d %13.0f %19.2f\n", log[i].hr_max, log[i].cad_t, log[i].elev); 

        i++;
    }
    printf("\n\n");
}

void print_bikes_sorted(bike_t *bike, int qtd_bikes)
{
    int i,j;

    for(i = 0; i < qtd_bikes; i++)
    {
        printf("\nBike: %s\n", bike[i].name);

        printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Data\t    | Distância(km) | Velocidade Média(km/h) | Velocidade Máxima(km/h) | Hr Médio(bpm) | Hr Max(bpm) | Cadência | Subida Acumulada(m)\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
        
        for(j = 0; j < bike[i].qatv; j++)
        {
            printf("%s %12.2f %19.2f ", bike[i].logs[j].date, bike[i].logs[j].distance, bike[i].logs[j].speed_t); 
            printf("%25.2f %19.0f ", bike[i].logs[j].speed_max, bike[i].logs[j].hr_t);
            printf("%14d %11.0f %16.2f\n", bike[i].logs[j].hr_max, bike[i].logs[j].cad_t, bike[i].logs[j].elev);
        }
        printf("\n\n");
    }
}

void print_info_of_bike(bike_t *bike, int choose, int qtd_bikes)
{
    int i;

    if((choose >= 0) && (choose <= qtd_bikes))
    {
        choose--;
        printf("\nBike: %s\n", bike[choose].name);
        printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Data\t    | Distância(km) | Velocidade Média(km/h) | Velocidade Máxima(km/h) | Hr Médio(bpm) | Hr Max(bpm) | Cadência | Subida Acumulada(m)\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
        
        i = 0;
        while (i < bike[choose].qatv-1) // -1 : ??
        {
            printf("%s %12.2f %19.2f ", bike[choose].logs[i].date, bike[choose].logs[i].distance, bike[choose].logs[i].speed_t); 
            printf("%25.2f %19.0f ", bike[choose].logs[i].speed_max, bike[choose].logs[i].hr_t);
            printf("%14d %11.0f %16.2f\n", bike[choose].logs[i].hr_max, bike[choose].logs[i].cad_t, bike[choose].logs[i].elev); 
            
            i++;
        }
    }
    else
        printf("Número informado é inválido.\n");
}

void print_info_add(bike_t *bike, int choose, int qtd_bikes)
{
    int i = choose;

    i--;
    printf("\n\n");

    printf("Total Distance of  %s: %.2fkm \n", bike[i].name, bike[i].total_distance);

    printf("Number of Activities of %s: %d \n", bike[i].name, bike[i].qatv);

    printf("Longer Pedal of %s: %.2fkm \n", bike[i].name, bike[i].longer_pedal);

    printf("Shorter Pedal of %s: %.2fkm \n", bike[i].name, bike[i].shorter_pedal);

    printf("Average distance of %s: %.2fkm \n\n", bike[i].name, bike[i].av_distance);
}

