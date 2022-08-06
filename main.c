#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "libgps.h"

#define TAM_ALLOC_BIKES 10
#define P_DIR 2
#define TAM_ALLOC_LOGS 100
#define PATHSIZE 30
#define LINESIZE 51

int main(int argc, char **argv)
{   
    // Verifica se os argumentos foram devidamente passados
    if( (argc < 2 || strcmp (argv[1], "-d")) || (argv[P_DIR] == NULL) )
    {
        fprintf(stderr, "\nError: Try ./gps -d <directory> . \n\n");
        exit(0);
    } 
    strcat(argv[P_DIR], "/");

    // Declaração de variáveis, estruturas necessárias e testes de alocação
    DIR *dir_log;
    FILE* arquivo;
    struct dirent *entry;

    bike_t *bike = alloc_bike();

    int nlogs = calc_num_logs(argv[P_DIR]);
    log_t *log = alloc_logs(nlogs);

    if(bike == NULL || log == NULL)
    {
        perror("Error: malloc failed. Aborting... \n");
        exit(1);
    }

    char *path = malloc(sizeof(char)*PATHSIZE);
    char *line = malloc(sizeof(char)*LINESIZE);
    char *name;

    int  tam_alloc_bikes = TAM_ALLOC_BIKES;
    int  choose, qtd_bikes = 0, qtd_logs = 0, exists, i = 0, j = 0;
    double distance = 0.00;

    // Inicialização das primeiras estruturas
    initialize_bike(bike, qtd_bikes);
    initialize_log(log, qtd_logs);

    // Abertura de diretório e leitura da primeira entrada
    dir_log = opendir ( argv[P_DIR] );
    if ( ! dir_log )
    {
        perror ("Could not open directory! Error");
        exit (1);
    }
    else printf("Running Program...\n\nOpening log directory...\n\n");

    entry = readdir (dir_log);

    if(entry)
        printf("Processing files...\n");

    while(entry)
    {
        strcpy(path, argv[P_DIR]);

        strcat(path, entry->d_name);

        if (entry->d_type == DT_REG)
        {
            // printf("%s\n", path); // Descomente para imprimir o caminho

            arquivo = fopen (path, "r");
            if ( ! arquivo )
            {
                perror ("Erro ao abrir arquivo");
                exit (1);
            }

            // Lê a primeira linha do arquivo
            fgets (line, LINESIZE, arquivo);
            line[strcspn (line, "\n")] = '\0';
            
            // Extrai o nome da bicicleta 
            name = bike_name(line);

            i = 0;
            
            // Verifica se a bicicleta já existe
            exists = search_bike(bike, name, &qtd_bikes, &i);

            // Cria se não existir
            if(! exists)
                bike[qtd_bikes] = create_bike(bike, name, &qtd_bikes, nlogs);

            if(qtd_bikes >= tam_alloc_bikes)
            {
                bike = realloc(bike, (TAM_ALLOC_BIKES + tam_alloc_bikes) * sizeof(bike_t));
                tam_alloc_bikes += TAM_ALLOC_BIKES;
            }

            log[qtd_logs] = create_log(log, qtd_logs);            

            // Extrai a data do log e coloca em uma variável da estrutura.
            find_date(arquivo, log, qtd_logs);
            
            while (! feof (arquivo))         
            {
                // Laço com as verificações necessárias e processamento dos dados. 
                if(strstr(line, "_alt"))
                    is_altitude(log, line, qtd_logs);
                else if(strstr(line, "cadence"))
                    is_cadence(log, line, qtd_logs);   
                else if(strstr(line, "heart"))
                    is_heart_rate(log, line, qtd_logs);
                else if(strstr(line, "timestamp"))
                    is_timestamp(log, line, qtd_logs);
                else if(strstr(line, "_speed"))
                    is_speed(log, line, qtd_logs);
                else if(strstr(line, "distance"))
                    is_distance(log, line, qtd_logs);

                // Continua lendo as linhas do arquivo
                fgets (line, LINESIZE, arquivo);
            }
            
            distance = log[qtd_logs].distance;
            bike[i].total_distance += distance;

            // Calcula-se as médias (hr, cad e speed)
            calc_averages(log, qtd_logs);

            verify_longer_and_shorter_pedals(bike, i, distance);
            
            // O log é copiado para sua respectiva bike, facilitando o processamento de dados.
            j = bike[i].qatv;

            bike[i].logs[j] = log[qtd_logs];

            bike[i].qatv++;

            qtd_logs++;

            fclose(arquivo);
        }

        // Lê a próxima entrada do diretório e continua com a verificação e processamento dos dados.
        entry = readdir (dir_log); 
    }

    calc_av_distance_of_each_bike(bike, qtd_bikes);

    // Lista as opções do sumário
    
    print_summary();

    // Usuário escolhe alguma opção do sumário para visualizar os dados desejados.

    printf("Enter one of the options: ");
    scanf("%d", &choose);
    printf("\n");

    switch(choose)
    {
        case 1: 
            show_bikes(bike, qtd_bikes); 
            break;

        case 2:
            show_bikes(bike, qtd_bikes);

            printf("Your choose: ");
            scanf("%d", &choose);

            if((choose >= 1) && (choose <= qtd_bikes))
            {
                print_info_of_bike(bike, choose, qtd_bikes);
                print_info_add(bike, choose, qtd_bikes);
            }
            else
                printf("Numero inválido. \n");
        break;

        case 3:
            sort_per_date(bike, qtd_bikes);
            print_bikes_sorted(bike, qtd_bikes);
        break;

        case 4: 
            sort_per_distance(bike, qtd_bikes);
            print_bikes_sorted(bike, qtd_bikes);
        break;

        case 5:
            sort_per_elev(log, qtd_logs);
            print_logs_sorted(log, qtd_logs);
        break;

        case 6:
            show_bikes(bike, qtd_bikes);

            printf("Your choose: ");                  
            scanf("%d", &choose);
            
            if((choose >= 1) && (choose <= qtd_bikes))
                build_histogram(bike, choose);
            else
                printf("Numero inválido. \n");
        break;

        case 7:
            resume_all_bikes(bike, qtd_bikes);
        break;

        default: 
            printf("Número Inválido. \n");
            exit(1);
    }

    (void) closedir (dir_log);

    release_memory(bike, log, qtd_bikes, qtd_logs);

    free(bike);
    free(log);
    free(path);
    free(line);

    return 0;
}