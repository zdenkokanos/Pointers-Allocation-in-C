#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE *f_v(FILE *file_data, char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count) // otvorí súbor pre celý program
{
    char data;             // komentár
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
        if (*ID_pole == NULL)          // ak súbor bol otvorený ale neboli alokované polia
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
        return file_data; // opäť vracia adresu na súbor
    }
}

void f_n(FILE *file_data, char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count) // spočíta počet záznamov, dynamicky alokuje polia
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
        if (*ID_pole == NULL) // porovnávam jedno pole pretože ak nie je jedno alokované tak nie sú ani ostatné
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
        }
    }
}

void f_c(int *p_y, char ***ID_pole, int *p_count, char ***DATE_pole)
{
    if (*ID_pole == NULL) // ak nebolo pole ešte alokované, nebolo stlačené "n" vypíšem....
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
        char **was_printed = (char **)calloc(*p_count, sizeof(char));     // alokujem pole do ktorého uložím číselné pozície zhodných záznamov zo súboru dataloger.txt
        char **ID_ciach = (char **)calloc((ciach_count), sizeof(char *)); // alokujem polia do ktorých uložím záznamy zo súboru ciachovanie.txt
        char **DATE_ciach = (char **)calloc((ciach_count), sizeof(char *));
        for (int i = 0; i < (ciach_count); i++) // alokácia
        {
            ID_ciach[i] = (char *)calloc(len_id + 1, sizeof(char));
            DATE_ciach[i] = (char *)calloc(len_date + 1, sizeof(char));
        }
        fseek(file_ciach, 0, SEEK_SET); // Nastaví pozíciu súbora na začiatok
        printf("Hodnota c: %d\n", *p_count);
        for (int i = 0; i < (ciach_count); i++)
        {
            fgets(line, sizeof(line), file_ciach);
            strncpy((ID_ciach)[i], line, len_id);
            fgets(line, sizeof(line), file_ciach);
            strncpy((DATE_ciach)[i], line, len_date);
            fgets(line, sizeof(line), file_ciach);
        }
        int print_count = 0; // definujem premennú j ktorú využívam ako počítadlo v nasledujúcom cykle
        int result = 0;
        int ciach_all_dates_match = 0;
        for (int i = 0; i < (ciach_count); i++)
        {
            for (int q = 0; q < *p_count; q++)
            {
                if (strcmp((*ID_pole)[q], (ID_ciach[i])) == 0)
                {
                }
            }
        }
        for (int i = 0; i < print_count; i++)
        {
            free(was_printed[i]);
        }
        free(was_printed);
    }
}

void f_s()
{
}

void f_h()
{
}

void f_z()
{
}

void f_k(FILE *file_data, char ***ID_pole, char ***POS_pole, char ***TYP_pole, char ***HOD_pole, char ***CAS_pole, char ***DATE_pole, int *p_count)
{
<<<<<<< HEAD
    if ()
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
=======
>>>>>>> main
}

int main()
{
    char vstup;
    int count;
    int y;
    int *p_count = &count;
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
            file_data = f_v(file_data, &ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count);
            break;
        case 'n':
            f_n(file_data, &ID_pole, &POS_pole, &TYP_pole, &HOD_pole, &CAS_pole, &DATE_pole, p_count);
            break;
        case 'c':
            f_c(&y, &ID_pole, p_count, &DATE_pole);
            break;
        case 's':
            f_s();
            break;
        case 'h':
            f_h();
            break;
        case 'z':
            f_z();
            break;
        case 'k':
            if (ID_pole != NULL)
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
                break;
            }
            else
            {
                return 0;
                break;
            }

        default:
            printf("Nesprávny znak skúste znova: \n");
            break;
        }
    }
    return 0;
}