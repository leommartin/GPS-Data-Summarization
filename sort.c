#include "libgps.h"
#include <string.h>

void sort_per_elev(log_t *log, int qtd_logs)
{
    int i, j, menor;
    log_t troca;

    for (i = 0; i < qtd_logs - 1; i++)
    {
        menor = i;
        for (j = i + 1; j < qtd_logs; j++)
        {
            if (log[j].elev < log[menor].elev)
                menor = j;
        }
        if (i != menor)
        {
            troca = log[i];
            log[i] = log[menor];
            log[menor] = troca;
        }
    }
}

void sort_per_distance(bike_t *bike, int qtd_bikes)
{
    int i, j, cb, menor;
    log_t troca;    

    for(cb = 0; cb < qtd_bikes; cb++)
    {  
        for(i = 0; i < bike[cb].qatv-1; i++)
        {
            menor = i;
            for (j = i + 1; j < bike[cb].qatv; j++)
            {
                if (bike[cb].logs[j].distance < bike[cb].logs[menor].distance)
                    menor = j;
            }
            if (i != menor)
            {
                troca = bike[cb].logs[i];
                bike[cb].logs[i] = bike[cb].logs[menor];
                bike[cb].logs[menor] = troca;
            }
        }
    }
}

void sort_per_date(bike_t *bike, int qtd_bikes)
{
    int i, j, cb, menor;
    log_t troca;    

    for(cb = 0; cb < qtd_bikes; cb++)
    {  
        for(i = 0; i < bike[cb].qatv-1; i++)
        {
            menor = i;
            for (j = i + 1; j < bike[cb].qatv; j++)
            {
                if ( strcmp(bike[cb].logs[j].date, bike[cb].logs[menor].date) < 0 )
                    menor = j;
            }
            if (i != menor)
            {
                troca = bike[cb].logs[i];
                bike[cb].logs[i] = bike[cb].logs[menor];
                bike[cb].logs[menor] = troca;
            }
        }
    }
}