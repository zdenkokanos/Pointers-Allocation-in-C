#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

FILE *f_v(FILE *file_data, char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count, bool *p_was_alokated) // otvorí súbor pre celý program
{
    char data;
    if (file_data == NULL) // ak súbor nebol otvorený
    {
        file_data = fopen("dataloger.txt", "r"); // otvorí súbor
        if (file_data == NULL)                   // ak zlyhalo otvorenie vypíše...
        {
            printf("Neotvoreny subor!\n");
            exit(1);
        }
        return file_data; // ak otvorí súbor vráti jeho adresu
    }
    else
    {
        fseek(file_data, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
        if (*p_was_alokated == false)  // ak súbor bol otvorený ale neboli alokované polia
        {
            for (data = getc(file_data); data != EOF; data = getc(file_data)) // vypíše do terminálu záznamy priamo zo súboru
            {
                printf("%c", data);
            }
        }
        else // ak boli alokované polia
        {
            for (int i = 0; i < *p_count; i++) // vypíše záznamy z týchto polí
            {
                if ((*ID_pole)[i] != NULL)
                {
                    printf("%s", (*ID_pole)[i]);
                    printf("\n");
                    printf("%s", (*POS_pole)[i]);
                    printf("\n");
                    printf("%s", (*TYP_pole)[i]);
                    printf("\n");
                    printf("%s", (*HOD_pole)[i]);
                    printf("\n");
                    printf("%s", (*CAS_pole)[i]);
                    printf("\n");
                    printf("%s", (*DATE_pole)[i]);
                    printf("\n");
                    printf("\n");
                }
            }
        }
        return file_data; // opäť vracia adresu na súbor
    }
}

void f_n(FILE *file_data, char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count, bool *p_was_alokated, int *p_erased_count) // spočíta počet záznamov, dynamicky alokuje polia
{
    char data;
    int len_id = 5; // dĺžky riadkov v záznamoch, sú fixne dané a preto sú dané ako čísla
    int len_pos = 14;
    int len_typ = 2;
    int len_hod = 6;
    int len_cas = 4;
    int len_date = 8;
    if (file_data == NULL) // ak ešte nebol stlačený príkaz "v" vypíše....
    {
        printf("Neotvorený súbor!\n");
    }
    else
    {
        if (*p_was_alokated == false) // porovnávam jedno pole pretože ak nie je jedno alokované tak nie sú ani ostatné
        {
            *p_count = 1;
            fseek(file_data, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
            for (data = fgetc(file_data); data != EOF; data = fgetc(file_data))
            {
                if (data == '\n')
                {
                    (*p_count)++; // spočítam počet riadkov
                }
            }
            *p_count = (*p_count / 7); // v jednom zázname je 6 typov hodnôt + jeden prázdny riadok preto delím počet riadkov 7
            *ID_pole = (char **)calloc((*p_count), sizeof(char *));
            *POS_pole = (char **)calloc((*p_count), sizeof(char *));
            *TYP_pole = (char **)calloc((*p_count), sizeof(char *));
            *HOD_pole = (char **)calloc((*p_count), sizeof(char *)); // alokujem pre všetky polia miesto v pamäti
            *CAS_pole = (char **)calloc((*p_count), sizeof(char *));
            *DATE_pole = (char **)calloc((*p_count), sizeof(char *));

            for (int i = 0; i < *p_count; i++)
            {
                (*ID_pole)[i] = (char *)calloc((len_id + 1), sizeof(char)); //+1 pretoze string končí \0
                (*POS_pole)[i] = (char *)calloc((len_cas + 1), sizeof(char));
                (*TYP_pole)[i] = (char *)calloc((len_typ + 1), sizeof(char));
                (*HOD_pole)[i] = (char *)calloc((len_hod + 1), sizeof(char)); // alokujem pre všetky polia miesto v pamäti
                (*CAS_pole)[i] = (char *)calloc((len_cas + 1), sizeof(char));
                (*DATE_pole)[i] = (char *)calloc((len_date + 1), sizeof(char));
            }
            char line[100];
            int poloha = 0;
            fseek(file_data, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
            for (int i = 0; i < *p_count; i++)
            {
                fgets(line, sizeof(line), file_data); // načíta string zo súboru a vloží ho do pomocného stringu line
                strncpy((*ID_pole)[i], line, len_id); // z pomocného stringu line skopíruje do alokovaného pola
                fgets(line, sizeof(line), file_data);
                strncpy((*POS_pole)[i], line, len_pos);
                fgets(line, sizeof(line), file_data);
                strncpy((*TYP_pole)[i], line, len_typ);
                fgets(line, sizeof(line), file_data); // pokračujem s načítaním do všetkých polí tak ako pri prvom
                strncpy((*HOD_pole)[i], line, len_hod);
                fgets(line, sizeof(line), file_data);
                strncpy((*CAS_pole)[i], line, len_cas);
                fgets(line, sizeof(line), file_data);
                strncpy((*DATE_pole)[i], line, len_date);
                fgets(line, sizeof(line), file_data); // načíta string v ktorom sa nachádza prázdny riadok, tento nezapisujem nikde, posunie pointer v súbore
            }
            *p_was_alokated = true;
            printf("Nacitanie do pola prebehlo úspešne\n"); // pokial alokácia prebehne úspešne, vypíše...
        }
        else // ak už pole bolo alokované, bol stlačený príkaz "n"
        {
            printf("Pole bolo uz alokované\n");
            fseek(file_data, 0, SEEK_SET);     // Nastaví pozíciu súbora na začiatok
            for (int i = 0; i < *p_count; i++) // dealokujem všetky polia
            {
                free((*ID_pole)[i]);
                free((*POS_pole)[i]);
                free((*TYP_pole)[i]);
                free((*HOD_pole)[i]);
                free((*CAS_pole)[i]);
                free((*DATE_pole)[i]);
            }

            free(*ID_pole); // dealokujem všetky polia
            free(*POS_pole);
            free(*TYP_pole);
            free(*HOD_pole);
            free(*CAS_pole);
            free(*DATE_pole);
            *ID_pole = NULL; // všetky hodnoty v poliach nastavím na null aby nenastali leaky
            *POS_pole = NULL;
            *TYP_pole = NULL;
            *HOD_pole = NULL;
            *CAS_pole = NULL;
            *DATE_pole = NULL;
            fseek(file_data, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
            if (*p_was_alokated == true)   // porovnávam jedno pole pretože ak nie je jedno alokované tak nie sú ani ostatné
            {
                *p_count = 1;
                fseek(file_data, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
                for (data = fgetc(file_data); data != EOF; data = fgetc(file_data))
                {
                    if (data == '\n')
                    {
                        (*p_count)++; // spočítam počet riadkov
                    }
                }
            }
            *p_count = (*p_count / 7);                              // v jednom zázname je 6 typov hodnôt + jeden prázdny riadok preto delím počet riadkov 7
            *ID_pole = (char **)calloc((*p_count), sizeof(char *)); // znovu alokujem pre všetky polia miesto v pamäti
            *POS_pole = (char **)calloc((*p_count), sizeof(char *));
            *TYP_pole = (char **)calloc((*p_count), sizeof(char *));
            *HOD_pole = (char **)calloc((*p_count), sizeof(char *)); // alokujem pre všetky polia miesto v pamäti
            *CAS_pole = (char **)calloc((*p_count), sizeof(char *));
            *DATE_pole = (char **)calloc((*p_count), sizeof(char *));

            for (int i = 0; i < *p_count; i++)
            {
                (*ID_pole)[i] = (char *)calloc((len_id + 1), sizeof(char)); //+1 pretoze string končí \0
                (*POS_pole)[i] = (char *)calloc((len_cas + 1), sizeof(char));
                (*TYP_pole)[i] = (char *)calloc((len_typ + 1), sizeof(char));
                (*HOD_pole)[i] = (char *)calloc((len_hod + 1), sizeof(char)); // alokujem pre všetky polia miesto v pamäti
                (*CAS_pole)[i] = (char *)calloc((len_cas + 1), sizeof(char));
                (*DATE_pole)[i] = (char *)calloc((len_date + 1), sizeof(char));
            }
            char line[100];
            fseek(file_data, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
            for (int i = 0; i < *p_count; i++)
            {
                fgets(line, sizeof(line), file_data);
                strncpy((*ID_pole)[i], line, len_id); // postupujem tak ako v "if"
                fgets(line, sizeof(line), file_data);
                strncpy((*POS_pole)[i], line, len_pos);
                fgets(line, sizeof(line), file_data);
                strncpy((*TYP_pole)[i], line, len_typ);
                fgets(line, sizeof(line), file_data);
                strncpy((*HOD_pole)[i], line, len_hod);
                fgets(line, sizeof(line), file_data);
                strncpy((*CAS_pole)[i], line, len_cas);
                fgets(line, sizeof(line), file_data);
                strncpy((*DATE_pole)[i], line, len_date);
                fgets(line, sizeof(line), file_data); // posúva pointer v súbore cez prázdny riadok
            }
            *p_was_alokated = true;
            *p_erased_count = 0;
        }
    }
}

void f_c(int *p_y, char ***ID_pole, int *p_count, char ***DATE_pole, bool *p_was_alokated)
{
    if (*p_was_alokated == false) // ak nebolo pole ešte alokované, nebolo stlačené "n" vypíšem....
    {
        printf("Dynamické polia nie sú vytvorené\n");
    }
    else
    {
        printf("Zadaj celé číslo y: "); // pýtam si vstup
        scanf("%d", p_y);
        FILE *file_ciach;
        int len_id = 5; // opäť fixne zadám dĺžku riadkov v súbore, ktorých dĺžka je vopred známa
        int len_date = 8;
        int ciach_count = 1; // hodnota ktorou počítam počet záznamov v súbore ciachovanie.txt
        char data;
        int result = 0;
        file_ciach = fopen("ciachovanie.txt", "r"); // otvorím súbor ciachovanie
        for (data = fgetc(file_ciach); data != EOF; data = fgetc(file_ciach))
        {
            if (data == '\n')
            {
                (ciach_count)++; // spočítam počet riadkov
            }
        }
        ciach_count = ciach_count / 3; // v súbore sa vždy nachádzajú dva riadky a jeden prázdny riadok čo je vopred známe
        char line[100];
        // char **was_printed = (char **)calloc(*p_count, sizeof(char));     // alokujem pole do ktorého uložím číselné pozície zhodných záznamov zo súboru dataloger.txt
        char **ID_ciach = (char **)calloc((ciach_count), sizeof(char *)); // alokujem polia do ktorých uložím záznamy zo súboru ciachovanie.txt
        char **DATE_ciach = (char **)calloc((ciach_count), sizeof(char *));
        for (int i = 0; i < (ciach_count); i++) // alokácia
        {
            ID_ciach[i] = (char *)calloc(len_id + 1, sizeof(char));
            DATE_ciach[i] = (char *)calloc(len_date + 1, sizeof(char));
        }
        fseek(file_ciach, 0, SEEK_SET);         // Nastaví pozíciu súbora na začiatok
        for (int i = 0; i < (ciach_count); i++) // načíta do 2D polí súbor ciachovanie
        {
            fgets(line, sizeof(line), file_ciach);
            strncpy((ID_ciach)[i], line, len_id);
            fgets(line, sizeof(line), file_ciach);
            strncpy((DATE_ciach)[i], line, len_date);
            fgets(line, sizeof(line), file_ciach);
        }
        // int print_count = 0; // definujem premennú print count ktorú využívam ako počítadlo v nasledujúcom cykle
        //  int result = 0;
        int ciach_all_dates_match = 0;
        for (int i = 0; i < (ciach_count); i++)
        {
            for (int q = 0; q < *p_count; q++)
            {
                if (strcmp((ID_ciach[i]), (*ID_pole)[q]) == 0)
                {
                    int DATE_ciach_num = atoi((DATE_ciach[i])); // konvertuje string na interger
                    int DATE_pole_num = atoi((*DATE_pole)[q]);
                    if (DATE_ciach_num != DATE_pole_num)
                    {
                        result = DATE_pole_num - DATE_ciach_num;
                        result = abs(result);
                        result = round((double)result / 100);
                        if (result >= *p_y)
                        {
                            printf("ID. mer. modulu %s má %d mesiacov po ciachovaní\n", (*ID_pole)[q], result);
                        }
                    }
                    break;
                }
            }
        }
        bool found = false;
        for (int i = 0; i < (*p_count); i++) // zistí ktoré IDcka neboli ciachované
        {
            found = false;
            for (int q = 0; q < ciach_count; q++)
            {
                found = false;
                if (strcmp((ID_ciach[q]), (*ID_pole)[i]) == 0)
                {
                    found = true;
                    break;
                }
            }
            if (found == false)
            {
                printf("ID. mer. modulu %s nebolo ciachované.\n", (*ID_pole)[i]);
            }
            else
            {
                for (int i = 0; i < (ciach_count); i++)
                {
                    for (int q = 0; q < *p_count; q++)
                    {
                        if (strcmp((ID_ciach[i]), (*ID_pole)[q]) == 0)
                        {
                            int DATE_ciach_num = atoi((DATE_ciach[i])); // konvertuje string na interger
                            int DATE_pole_num = atoi((*DATE_pole)[q]);
                            if (DATE_ciach_num == DATE_pole_num)
                            {
                                ciach_all_dates_match++;
                            }
                        }
                    }
                }
                if (ciach_all_dates_match == *p_count)
                {
                    printf("Dáta sú korektné.\n");
                }
            }
        }
    }
}

void f_s(char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count, bool *p_was_alokated)
{
    if (*p_was_alokated == false) // ak nebolo pole ešte alokované, nebolo stlačené "n" vypíšem....
    {
        printf("Dynamické polia nie sú vytvorené\n");
    }
    else
    {
        char ID_input[6] = {0};
        char TYP_input[4] = {0};
        char *temp = {0};
        temp = (char *)calloc(1, sizeof(char));
        printf("Zadaj ID a typ: ");
        scanf("%s %s", ID_input, TYP_input);
        long *S_POLE_CAS_DATE;
        char **S_LONGITUDE;
        char **S_LATITUDE;
        double *S_HODNOTA;
        double *S_LONGITUDE_double;
        double *S_LATITUDE_double;
        char **S_POS;
        bool exists = false;
        int S_counter = 0;
        S_POLE_CAS_DATE = (long *)calloc(*p_count, sizeof(long));
        S_POS = (char **)calloc((*p_count), sizeof(char *));
        S_LONGITUDE = (char **)calloc((*p_count), sizeof(char *));
        S_LATITUDE = (char **)calloc((*p_count), sizeof(char *));
        S_HODNOTA = (double *)calloc(*p_count, sizeof(double));
        S_LONGITUDE_double = (double *)calloc(*p_count, sizeof(double));
        S_LATITUDE_double = (double *)calloc(*p_count, sizeof(double));
        for (int i = 0; i < *p_count; i++)
        {
            (S_POS)[i] = (char *)calloc(6, sizeof(char));
            (S_LONGITUDE)[i] = (char *)calloc(8, sizeof(char));
            (S_LATITUDE)[i] = (char *)calloc(8, sizeof(char));
        }

        for (int i = 0; i < *p_count; i++)
        {
            temp = NULL;
            if ((strcmp(ID_input, (*ID_pole)[i]) == 0) && (strcmp(TYP_input, (*TYP_pole)[i]) == 0))
            {
                temp = strcat((*DATE_pole)[i], (*CAS_pole)[i]);
                S_POLE_CAS_DATE[S_counter] = atol(temp);
                strcpy(S_POS[S_counter], (*POS_pole)[i]);
                S_HODNOTA[S_counter] = strtod((*HOD_pole)[i], NULL);
                char *token = strtok((S_POS)[S_counter], "+-");
                S_LATITUDE[S_counter] = token;
                token = strtok(NULL, "+-");
                S_LONGITUDE[S_counter] = token;
                S_LATITUDE_double[S_counter] = (strtod(S_LATITUDE[S_counter], NULL) / 10000);
                S_LONGITUDE_double[S_counter] = (strtod(S_LONGITUDE[S_counter], NULL) / 10000);
                S_counter++;
                exists = true;
            }
        }
        if (exists == false)
        {
            printf("Pre dany vstup neexistujú záznamy.\n");
        }
        long temporar_date;
        double temporar_lat_lon;

        for (int i = 0; i < S_counter - 1; i++)
        {
            for (int j = i + 1; j < S_counter; j++)
            {
                if (S_POLE_CAS_DATE[j] < S_POLE_CAS_DATE[i])
                {
                    temporar_date = S_POLE_CAS_DATE[i];
                    S_POLE_CAS_DATE[i] = S_POLE_CAS_DATE[j];
                    S_POLE_CAS_DATE[j] = temporar_date;

                    temporar_lat_lon = S_LATITUDE_double[i];
                    S_LATITUDE_double[i] = S_LATITUDE_double[j];
                    S_LATITUDE_double[j] = temporar_lat_lon;

                    temporar_lat_lon = S_LONGITUDE_double[i];
                    S_LONGITUDE_double[i] = S_LONGITUDE_double[j];
                    S_LONGITUDE_double[j] = temporar_lat_lon;

                    temporar_lat_lon = S_HODNOTA[i];
                    S_HODNOTA[i] = S_HODNOTA[j];
                    S_HODNOTA[j] = temporar_lat_lon;
                }
            }
        }

        FILE *fw;
        fw = fopen("vystup.txt", "w");
        if (fw == NULL)
        {
            printf("Pre daný vstup nie je vytvorený txt súbor.");
        }
        for (int i = 0; i < S_counter; i++)
        {
            fprintf(fw, "%ld\t%.5lf\t+%.4lf\t+%.4lf\n", S_POLE_CAS_DATE[i], S_HODNOTA[i], S_LATITUDE_double[i], S_LONGITUDE_double[i]);
        }
        int close_result = fclose(fw);
        if (close_result == 1)
        {
            printf("Pre daný vstup nie je vytvorený txt súbor.");
        }
        printf("Pre daný vstup je vytvorený txt súbor.");
    }
}

void f_h(char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count, bool *p_was_alokated)
{
    if (*p_was_alokated == false)
    {
        printf("Polia nie sú vytvorené.\n");
    }
    else
    {
        int q;
        char types_mer_vel[6][3] = {"RD", "RM", "RO", "PI", "PE", "PA"};
        int types_count = 0;
        double max = 0;
        double value;
        double min;
        printf("Typ mer. vel\tPočetnosť\tMinimum\t   Maximum\n");
        for (int i = 0; i < 6; i++)
        {
            types_count = 0;
            max = 0;
            min = strtod((*HOD_pole)[0], NULL);
            for (q = 0; q < *p_count; q++)
            {
                if (strcmp(types_mer_vel[i], (*TYP_pole)[q]) == 0)
                {
                    types_count++;
                    value = strtod((*HOD_pole)[q], NULL);
                    if (value > max)
                    {
                        max = value;
                    }
                    if (value < min)
                    {
                        min = value;
                    }
                }
            }
            if (types_count != 0)
            {
                printf("     %s\t\t    %d\t\t%.2lf\t    %.2lf\n", types_mer_vel[i], types_count, min, max);
            }
        }
    }
}

void f_z(char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count, bool *p_was_alokated, int *p_erased_count)
{
    if (*p_was_alokated == false)
    {
        printf("Polia nie sú vytvorené.\n");
    }
    else
    {
        char ID_input[6] = {0};
        printf("Zadaj ID: ");
        scanf("%s", ID_input);
        for (int i = 0 + (*p_erased_count); i < *p_count; i++)
        {
            if (strcmp(ID_input, (*ID_pole)[i]) == 0)
            {
                free((*ID_pole)[i]);
                free((*POS_pole)[i]);
                free((*TYP_pole)[i]);
                free((*HOD_pole)[i]);
                free((*CAS_pole)[i]);
                free((*DATE_pole)[i]);
                (*ID_pole)[i] = NULL;
                (*POS_pole)[i] = NULL;
                (*TYP_pole)[i] = NULL;
                (*HOD_pole)[i] = NULL;
                (*CAS_pole)[i] = NULL;
                (*DATE_pole)[i] = NULL;

                (*p_erased_count)++;
            }
        }
        printf("Vymazalo sa: %d záznamov!\n", *p_erased_count);
        //*p_count = *p_count - *p_erased_count;
    }
}

int main()
{
    char vstup;
    int count;
    int y;
    int *p_count = &count;
    int erase_count = 0;
    int *p_erase_count = &erase_count;
    bool was_alokated = false;
    bool *p_was_alokated = &was_alokated;
    FILE *file_data = NULL;
    char **ID_pole = NULL;   // spravím pointer pre string do ktorého môže ísť 10 znakov //Id meracieho modulu
    char **POS_pole = NULL;  // Pozícia modulu
    char **TYP_pole = NULL;  // Typ meracej veličiny
    char **HOD_pole = NULL;  // hodnota - reálne číslo
    char **CAS_pole = NULL;  // Cas merania
    char **DATE_pole = NULL; // Dátum merania
    while (1)
    {
        scanf(" %c", &vstup);
        switch (vstup)
        {
        case 'v':
            file_data = f_v(file_data, &ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count, p_was_alokated);
            break;
        case 'n':
            f_n(file_data, &ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count, p_was_alokated, p_erase_count);
            break;
        case 'c':
            f_c(&y, &ID_pole, p_count, &DATE_pole, p_was_alokated);
            break;
        case 's':
            f_s(&ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count, p_was_alokated);
            break;
        case 'h':
            f_h(&ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count, p_was_alokated);
            break;
        case 'z':
            f_z(&ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count, p_was_alokated, p_erase_count);
            break;
        case 'k':
            if (was_alokated == true)
            {
                for (int i = 0; i < *p_count; i++) // dealokujem všetky polia
                {
                    free((ID_pole)[i]);
                    free((POS_pole)[i]);
                    free((TYP_pole)[i]);
                    free((HOD_pole)[i]);
                    free((CAS_pole)[i]);
                    free((DATE_pole)[i]);
                }

                free(ID_pole); // dealokujem všetky polia
                free(POS_pole);
                free(TYP_pole);
                free(HOD_pole);
                free(CAS_pole);
                free(DATE_pole);
                ID_pole = NULL; // všetky hodnoty v poliach nastavím na null aby nenastali leaky
                POS_pole = NULL;
                TYP_pole = NULL;
                HOD_pole = NULL;
                CAS_pole = NULL;
                DATE_pole = NULL;

                return 0;
            }
            else
            {
                return 0;
            }

        default:
            printf("Nesprávny znak skúste znova: \n");
            break;
        }
    }
}