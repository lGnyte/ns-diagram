#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <graphics.h>
#include <winbgim.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <cmath>
#define dim 1
#define stil 6
#define x_initial 30
#define y_initial 100
#define distanta 10
#define nr_culori 5
#define MAX_TEXT 100
using namespace std;
char text[MAX_TEXT], anterior[MAX_TEXT], copie[MAX_TEXT];
float ratie = 1.5;
int x_final, y_final,
    window_width=getmaxwidth(),
    window_height=getmaxheight();
int LMAX = window_width / 2 - 50; /// lungimea este adaptata la dimensiunea ferestrei
enum instructiune_speciala {CLASIC, CONDITIE, ADV, FALS, LOOP_FIRST, IN_LOOP, LOOP_LAST, SWITCH_CASE, CAZ, ROOT, IGNORA_DA, IGNORA_NU};
instructiune_speciala cod;

int culoare = 0;

struct nod
{
    char linie_text[MAX_TEXT];
    int x, y;
    int lungime;
    instructiune_speciala tip;
    int nr_fii = 0;
    int nr_spatii = -1;
    int spre_dreapta;
    nod * urm[30];
    nod * parinte = NULL;
} *inainte, *radacina;

struct rgb_loop
{
    int red[nr_culori] = {25, 20};
    int green[nr_culori] = {81, 71};
    int blue[nr_culori] = {104, 94};
} CuloareLoop;

struct rgb_in_loop
{
    int red[nr_culori] = {162, 182};
    int green[nr_culori] = {162, 182};
    int blue[nr_culori] = {161, 181};
} CuloareInLoop;

struct rgb_conditie
{
    int red[nr_culori] = {255, 255};
    int green[nr_culori] = {166, 144};
    int blue[nr_culori] = {0, 79};
} CuloareConditie;

struct rgb_NU
{
    int red[nr_culori] = {255, 255};
    int green[nr_culori] = {23, 1};
    int blue[nr_culori] = {0, 79};
} CuloareNU;

struct rgb_DA
{
    int red[nr_culori] = {77, 77};
    int green[nr_culori] = {105, 105};
    int blue[nr_culori] = {16, 95};
} CuloareDA;

struct rgb_FALS
{
    int red[nr_culori] = {245, 235};
    int green[nr_culori] = {82, 65};
    int blue[nr_culori] = {63, 94};
} CuloareFALS;

struct rgb_ADV
{
    int red[nr_culori] = {88, 99};
    int green[nr_culori] = {202, 141};
    int blue[nr_culori] = {99, 127};
} CuloareADV;

struct rgb_clasic
{
    int red[nr_culori] = {240, 235};
    int green[nr_culori] = {173, 183};
    int blue[nr_culori] = {120, 137};
} CuloareClasic;

///Universal position struct
struct poz{
    int x,y;
};

void color_loop(int x_stanga, int y_sus, int Lsus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareLoop.red[culoare], CuloareLoop.green[culoare], CuloareLoop.blue[culoare]));
    floodfill(x_stanga + Lsus/2 , y_sus + 2, WHITE);
    setbkcolor(COLOR(CuloareLoop.red[culoare], CuloareLoop.green[culoare], CuloareLoop.blue[culoare]));
}

void color_in_loop(int x_stanga, int y_sus, int Lsus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareInLoop.red[culoare], CuloareInLoop.green[culoare], CuloareInLoop.blue[culoare]));
    floodfill(x_stanga + Lsus/2 , y_sus + 2, WHITE);
    setbkcolor(COLOR(CuloareInLoop.red[culoare], CuloareInLoop.green[culoare], CuloareInLoop.blue[culoare]));
}

void color_conditie(int x_stanga, int y_sus, int Lsus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareConditie.red[culoare], CuloareConditie.green[culoare], CuloareConditie.blue[culoare]));
    floodfill(x_stanga + Lsus/2 , y_sus + 2, WHITE);
    setbkcolor(COLOR(CuloareConditie.red[culoare], CuloareConditie.green[culoare], CuloareConditie.blue[culoare]));
}

void color_DA(int x_stanga, int y_sus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareDA.red[culoare], CuloareDA.green[culoare], CuloareDA.blue[culoare]));
    floodfill(x_stanga + 1 , y_sus + 5, WHITE);
    setbkcolor(COLOR(CuloareDA.red[culoare], CuloareDA.green[culoare], CuloareDA.blue[culoare]));
}

void color_NU(int x_dreapta, int y_sus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareNU.red[culoare], CuloareNU.green[culoare], CuloareNU.blue[culoare]));
    floodfill(x_dreapta - 1 , y_sus + 5, WHITE);
    setbkcolor(COLOR(CuloareNU.red[culoare], CuloareNU.green[culoare], CuloareNU.blue[culoare]));
}

void color_clasic(int x_stanga, int y_sus, int Lsus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareClasic.red[culoare], CuloareClasic.green[culoare], CuloareClasic.blue[culoare]));
    floodfill(x_stanga + Lsus/2 , y_sus + 2, WHITE);
    setbkcolor(COLOR(CuloareClasic.red[culoare], CuloareClasic.green[culoare], CuloareClasic.blue[culoare]));
}

void color_adv(int x_stanga, int y_sus, int Lsus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareADV.red[culoare], CuloareADV.green[culoare], CuloareADV.blue[culoare]));
    floodfill(x_stanga + Lsus/2 , y_sus + 2, WHITE);
    setbkcolor(COLOR(CuloareADV.red[culoare], CuloareADV.green[culoare], CuloareADV.blue[culoare]));
}

void color_fals(int x_stanga, int y_sus, int Lsus)
{
    setfillstyle(SOLID_FILL, COLOR(CuloareFALS.red[culoare], CuloareFALS.green[culoare], CuloareFALS.blue[culoare]));
    floodfill(x_stanga + Lsus/2 , y_sus + 2, WHITE);
    setbkcolor(COLOR(CuloareFALS.red[culoare], CuloareFALS.green[culoare], CuloareFALS.blue[culoare]));
}

int get_y_from_previous(nod *r)
{
    if(r == NULL)
        return 0;
    if(r->tip == CONDITIE)
    {
        int a, b;
        a = get_y_from_previous(r->urm[1]);
        if(r->nr_fii == 2)
        {
            b = get_y_from_previous(r->urm[2]);
            return max(a, b);
        }
        else
            return  a;
    }
    else
    {
        if (r->nr_fii)
            get_y_from_previous(r->urm[r->nr_fii]);
        else
            return r->y + ratie * textheight(r->linie_text);
    }
}

int nr_loops(nod *r)
{
    int contor = 0;
    if(r == NULL)
        return 0;
    r = r->parinte;
    while(r != NULL)
    {
        if(r->tip == LOOP_FIRST)
            contor++;
        r = r->parinte;
    }
    return contor;
}

nod *creare_nod(char text[], nod *anterior, int i, instructiune_speciala cod, char text_anterior[])
{
    /// CREATE NODE
    nod *p = new nod;

    /// ADD GENERAL INFO
    strcpy(p->linie_text, text);
    p->nr_spatii = i;

    /// FIRST NODE
    if(anterior == NULL)
    {
        /// ADD ROOT
        nod *q = new nod;
        strcpy(q->linie_text, "---");
        q->nr_spatii = -1;
        q->nr_fii = 1;

        ///LINK ROOT TO FIRST NODE
        q->urm[q->nr_fii] = p;

        /// INITIALIZATION
        p->x = x_initial;
        p->y = y_initial;
        p->lungime = LMAX;

        /// TYPE ASSIGMENT
        p->tip = cod;

        /// LINK NODE TO ROOT
        p->parinte = q;

        /// ROOT
        radacina = q;
        radacina->tip = ROOT;

        /// RETURN NODE
        return p;
    }
    if(anterior->tip == CONDITIE)
    {
        /// CREATE NODE "DA"
        nod *q = new nod;
        strcpy(q->linie_text, "DA");
        q->tip = IGNORA_DA;

        /// PRIMUL FIU E "DA"
        anterior->nr_fii = 1;
        q->nr_spatii = -1;

        /// LINK "DA" TO CONDITIE
        anterior->urm[anterior->nr_fii] = q;
        q->parinte = anterior;

        /// LINK NODE TO "DA"
        q->nr_fii = 1; /// NODUL E PRIMUL FIU AL LUI "DA"
        q->urm[q->nr_fii] = p;
        p->parinte = q; /// parintele lui e "DA"

        /// COORDONATE
        p->x = anterior->x;
        p->y = anterior->y + 3 * textheight(anterior->linie_text);
        p->lungime = anterior->lungime/2; /// jumatate din lungimea conditie

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(strstr(text_anterior, "else")) /// !!!!!! am de modificat in fct de i (daca am mai multe if else)
    {
        /// CREATE NODE "NU"
        nod *q = new nod;
        strcpy(q->linie_text, "NU");
        q->nr_spatii = -1;
        q->tip = IGNORA_NU;

        /// GO TO CONDITIE:: (ADV -> "DA" -> CONDITIE)
        while(anterior->nr_spatii != i - 4)
            anterior = anterior->parinte->parinte;

        /// LINK "NU" TO CONDITIE
        anterior->nr_fii++;
        anterior->urm[anterior->nr_fii] = q;
        q->parinte = anterior;

        /// LINK NODE TO "NU"
        q->nr_fii = 1;
        q->urm[q->nr_fii] = p;
        p->parinte = q;

        /// COORDONATE
        p->x = anterior->x + anterior->lungime/2;
        p->y = anterior->y + 3 * textheight(anterior->linie_text);
        p->lungime = anterior->lungime/2;

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(anterior->nr_spatii == i)
    {
        /// AU ACELASI PARINTE
        p->parinte = anterior->parinte;

        /// AU ACEEASI LUNGIME
        p->lungime = anterior->lungime;

        /// COORDONATE
        p->x = anterior->x;
        p->y = anterior->y + ratie * textheight(anterior->linie_text);

        /// LEG DE PARINTELE FRATELUI
        anterior->parinte->nr_fii++;
        anterior->parinte->urm[anterior->parinte->nr_fii] = p;

        /// TIP
        if(cod == 0)
            p->tip = anterior->tip;
        else
            p->tip = cod; /// poate am IF, FOR, etc.

        /// RETURN NODE
        return p;
    }
    else if(anterior->tip == LOOP_FIRST && (i - anterior->nr_spatii) == 4)
    {
        /// LINK NODE TO PARENT
        p->parinte = anterior;
        anterior->nr_fii++;
        anterior->urm[anterior->nr_fii] = p;

        /// COORDONATE
        p->x = anterior->x;
        p->y = anterior->y + ratie*textheight(anterior->linie_text);
        p->lungime = anterior->lungime;

        /// TIP
        if (cod != CLASIC)
            p->tip = cod;
        else
            p->tip = ADV;

        /// RETURN NODE
        return p;
    }
    else if(i < anterior->nr_spatii && cod != FALS)
    {
        /// START FROM ROOT
        nod *start = radacina->urm[radacina->nr_fii];

        /// GO DOWN TO SIBLING
        while(i != start->nr_spatii)
            start = start->urm[start->nr_fii];

        /// COORDONATE
        p->x = start->x; /// iau coordonata pe Ox a fratelui
        p->y = get_y_from_previous(start);
        p->lungime = start->lungime;

        /// TIP
        if(cod == CLASIC) /// daca inca nu are atribuit un cod
        {
            switch (start->parinte->tip) /// vedem daca are un parinte "special"
            {
            case IGNORA_DA: /// parintele e un nod "DA"

                p->tip = ADV;
                break;

            case IGNORA_NU: /// parintele e un nod "NU"

                p->tip = FALS;
                break;

            case LOOP_FIRST: /// parintele e un for/while
                p->tip = IN_LOOP;
                break;

            }
        }
        else
            p->tip = cod; /// poate e if, for, while, etc...

        /// GO TO PARENT
        start = start->parinte;

        /// LINK NODE TO PARENT
        start->nr_fii++;
        start->urm[start->nr_fii] = p;
        p->parinte = start;

        /// RETURN NODE
        return p;
    }
    else if(i < anterior->nr_spatii && cod == FALS)
    {
        /// START FROM ROOT
        nod *start = radacina->urm[radacina->nr_fii];

        /// GO DOWN TO PARENT
        while(i-4 != start->nr_spatii)
            start = start->urm[start->nr_fii];

        /// CREATE NODE "NU"
        nod *q = new nod;
        strcpy(q->linie_text, "NU");
        q->nr_spatii = -1;
        q->tip = IGNORA_NU;

        /// LINK "NU" TO CONDITIE
        start->nr_fii++;
        start->urm[start->nr_fii] = q;
        q->parinte = start;

        /// LINK NODE TO "NU"
        q->nr_fii = 1;
        q->urm[q->nr_fii] = p;
        p->parinte = q;

        /// COORDONATE
        p->x = start->x + start->lungime/2;
        p->y = start->y + 3 * textheight(start->linie_text);
        p->lungime = start->lungime/2;

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
}

void elimin_spatii_multiple(char text[])
{
    /// SEARCH FIRST ' '
    char *spatiu = strchr(text, ' ');

    /// FIND IT'S POSITION
    int poz = (int)(spatiu - text);

    /// GO TO NEXT CHARACTER
    poz++;

    /// ELIMIN ' '
    while(text[poz] == ' ')
        strcpy(text + poz, text + poz + 1);

    /// SEARCH '  '
    char *exista = strstr(text, "  "); /// 2 spatii

    /// MAI AM DE ELIMINAT SPATII MULTIPLE
    if(exista)
    {
        int start = (int)(exista - text); /// SEARCH POSITION
        elimin_spatii_multiple(text + start);
    }
}

void prelucrare_text_conditie(char text[])
{
    /// ELIMIN "IF"
    strcpy(text, text+2);

    /// ELIMIN SPATIILE DINTRE IF SI CONDITIE
    while(text[0] == ' ')
        strcpy(text, text+1);

    /// GASESC UNDE INCHEPE "THEN"
    char *capat = strstr(text, "then");
    int poz = (int)(capat - text);

    /// CARACTERUL ANTERIOR
    poz--;

    /// ELIMIN SPATIILE DINTRE CONDITIE SI THEN
    while(text[poz] == ' ')
        poz--;

    /// ADD ? AT THE END
    text[++poz] = '?';
    text[++poz] = '\0';

    /// DACA ARE SPATII MULTIPLE INAUNTRU
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);
}

void prelucrare_text_while_for(char text[])
{
    /// GASESC UNDE INCHEPE "DO"
    char *capat = strstr(text, "do");
    int poz = (int)(capat - text);

    /// CARACTERUL ANTERIOR
    poz--;

    /// ELIMIN SPATIILE DINAINTE DE "DO"
    while(text[poz] == ' ')
        poz--;

    /// ADD END OF STRING
    text[++poz] = '\0';

    /// DACA ARE SPATII MULTIPLE INAUNTRU
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);
}

void prelucrare_text_until(char text[])
{
    /// GASESC UNDE INCEPE ';'
    char *capat = strstr(text, ";");
    int poz = (int)(capat - text);

    /// ADD END OF STRING
    text[++poz] = '\0';

    /// DACA ARE SPATII MULTIPLE INAUNTRU
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);

    /// GO TO END
    int lung = strlen(text) - 1;

    /// DACA INAINTE DE ';' E UN SPATIU
    if(text[lung - 1] == ' ')
        strcpy(text + lung - 1, text + lung); /// SCAP DE SPATIUL ACELA
}

instructiune_speciala simplify(char text[], int start)
{
    /// ELIMIN SPATIILE DINAINTE
    strcpy(text, text + start);

    /// daca linia este un comentariu SAU e doar begin / end / else
    if((text[0] == '/' && text[1] == '/') || strstr(text, "begin") || strstr(text, "end") || strstr(text, "else"))
    {
        text[0] = '\0';
        return CLASIC;
    }

    /// ELIMIN COMENTARIILE
    char *p;
    p = strtok(text, "//"); /// pt a elimina comentariile de tip //

    /// NU AM NIMIC PE LINIE
    if(text[0] == '\n')
    {
        text[0] = '\0';
        return CLASIC;
    }

    /// CONDITIE
    if((int)(strstr(text, "if ") - text) == 0)
    {
        prelucrare_text_conditie(text);
        return CONDITIE;
    }

    /// WHILE
    if((int)(strstr(text, "while ") - text) == 0)
    {
        prelucrare_text_while_for(text);
        return LOOP_FIRST;
    }

    /// REPEAT
    if(strstr(text, "repeat")) /// las until
    {
        text[0] = '\0';
        return CLASIC;
    }

    /// FOR
    if((int)(strstr(text, "for ") - text) == 0)
    {
        prelucrare_text_while_for(text);
        return LOOP_FIRST;
    }

    /// REPREAT UNTIL
    if((int)(strstr(text, "until") - text) == 0)
    {
        prelucrare_text_until(text);
        return LOOP_LAST;
    }

    /// PUN '\0' LA FINAL
    int lung = strlen(text) - 1;
    text[lung] = '\0';

    /// DACA AM SPATII MULTIPLE
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);

    /// DACA NU E O INSTRUCTIUNE SPECIALA
    return CLASIC;
}


void DrawRectangle(char text[], int x_stanga, int y_sus, int Lsus) /// Lsus = Lungimea dreptunghiului desenat
{
    /// textwidth
    int Lungime = textwidth(text);

    /// latime dreptunghi desenat
    int latime = textheight(text)*ratie;

    /// coltul dreapta jos
    int y_jos = y_sus + latime;
    int x_dreapta = x_stanga + Lsus;

    /// LINII
    line(x_stanga, y_jos, x_stanga, y_sus);
    line(x_stanga, y_sus, x_dreapta, y_sus);
    line(x_dreapta, y_sus, x_dreapta, y_jos);
}

void DrawTriangle_true(int x_stanga, int y_stanga, int x_mijloc, int y_mijloc)
{
    char text[3];
    strcpy(text, "DA");
    int latime = textheight(text);
    line(x_stanga, y_stanga, x_stanga, y_mijloc);
    line(x_stanga, y_mijloc, x_mijloc, y_mijloc);
    line(x_stanga, y_stanga, x_mijloc, y_mijloc);
    ///outtextxy(x_st + 2, y_mijl - latime - 1, text);
}

void DrawTriangle_false(int x_dreapta, int y_sus, int x_mijloc, int y_mijloc)
{
    char text[3];
    strcpy(text, "NU");
    int Lungime = textwidth(text);
    int latime = textheight(text);
    line(x_dreapta, y_sus, x_dreapta, y_mijloc);
    line(x_dreapta, y_mijloc, x_mijloc, y_mijloc);
    line(x_mijloc, y_mijloc, x_dreapta, y_sus);
    ///outtextxy(x_dr - Lungime - 2, y_mijl - latime - 1, text);
}

void DrawConditie(char text[], int x, int y, int Lsus)
{
    int Lungime = textwidth(text);
    int latime = textheight(text);
    line(x, y, x + Lsus, y);
    DrawTriangle_true(x, y, x + Lsus/2, y + 3*latime);
    DrawTriangle_false(x+Lsus, y, x + Lsus/2, y + 3*latime);
    ///outtextxy(x + (Lsus - Lungime)/2, y + 2, text);
}

void DrawTriangle_false_text(int x_dreapta, int y_sus, int x_mijloc, int y_mijloc)
{
    char text[3];
    strcpy(text, "NU");
    int Lungime = textwidth(text);
    int latime = textheight(text);
    color_NU(x_dreapta, y_sus);
    outtextxy(x_dreapta - Lungime - 2, y_mijloc - latime - 1, text);
}

void DrawTriangle_true_text(int x_stanga, int y_stanga, int x_mijloc, int y_mijloc)
{
    char text[3];
    strcpy(text, "DA");
    int latime = textheight(text);
    color_DA(x_stanga, y_stanga);
    outtextxy(x_stanga + 2, y_mijloc - latime - 1, text);
}

void DrawConditie_Text(char text[], int x, int y, int Lsus)
{
    int Lungime = textwidth(text);
    int latime = textheight(text);

    /// WRITE CENTERED
    color_conditie(x, y, Lsus);
    outtextxy(x + (Lsus - Lungime)/2, y + 2, text);

    DrawTriangle_true_text(x, y, x + Lsus/2, y + 3*latime);
    DrawTriangle_false_text(x+Lsus, y, x + Lsus/2, y + 3*latime);
}

void DrawRectangle_text(char text[], int x_stanga, int y_sus, int Lsus) /// Lsus = Lungimea dreptunghiului desenat
{
    /// textwidth
    int Lungime = textwidth(text);

    /// latime dreptunghi desenat
    int latime = textheight(text)*ratie;

    outtextxy(x_stanga + (Lsus - Lungime)/2, y_sus + latime/5, text);
}
void chenar(int x_stanga, int y_sus, int x_dreapta, int y_jos)
{
    rectangle(x_stanga, y_sus, x_dreapta, y_jos);
}

void desen(nod *r)
{
    delay(100);
    if(r != NULL)
    {
        if(r!= radacina && r->tip != IGNORA_DA && r->tip != IGNORA_NU)
        {
            /// CALCULEZ CAT MA DUC SPRE DREAPTA CU BLOCUL
            r->spre_dreapta = nr_loops(r) * distanta;

            /// SCHIMB COORDONATA PE Ox && LUNGIMEA
            r->x += r->spre_dreapta;
            r->lungime -= r->spre_dreapta;

            /// PT RAMURILE TRUE SI FALSE E DIFERIT
            if(r->parinte->tip == IGNORA_DA)
                {
                    r->x = r->parinte->parinte->x;
                    r->lungime = r->parinte->parinte->lungime / 2;
                }
            else if(r->parinte->tip == IGNORA_NU)
            {
                r->x = r->parinte->parinte->urm[1]->urm[1]->x + r->parinte->parinte->urm[1]->urm[1]->lungime;
                r->lungime = r->parinte->parinte->lungime/2;
            }
            //cout << r->linie_text << "   " << r->x << " " << r->lungime << '\n';

            /// DRAWING
            switch (r->tip)
            {
            case CONDITIE:
                DrawConditie(r->linie_text, r->x, r->y, r->lungime);

                /// chenar pt true
                chenar(r->x, r->urm[1]->urm[1]->y, r->x + r->lungime/2, get_y_from_previous(r));

                /// chenar pt false
                chenar(r->x + r->lungime/2, r->urm[1]->urm[1]->y, r->x + r->lungime, get_y_from_previous(r));
                break;
            default:
                DrawRectangle(r->linie_text, r->x, r->y, r->lungime);

                /// chenar pt instructiunile FOR, WHILE
                if(r->tip == LOOP_FIRST)
                    chenar(r->x, r->y, r->x + r->lungime, get_y_from_previous(r));
                break;
            }
        }
        else if (r == radacina)
            /// bordura
            {
                chenar(x_initial, y_initial, x_initial + LMAX, get_y_from_previous(radacina));
                color_clasic(x_initial, y_initial, LMAX);
            }

        /// GO TO CHILDREN
        for(int k = 1; k <= r->nr_fii; k++)
            desen(r->urm[k]);
    }
}

void Add_Color_and_Text(nod *r)
{
    if(r != NULL)
    {
        if(r!= radacina && r->tip != IGNORA_DA && r->tip != IGNORA_NU)
        {
            switch (r->tip)
            {
            case CONDITIE:
                DrawConditie_Text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case ADV:
                color_adv(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case FALS:
                color_fals(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case LOOP_FIRST:
                color_loop(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case IN_LOOP:
                color_in_loop(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            default:
                color_clasic(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            }
        }

        /// GO TO CHILDREN
        for(int k = 1; k <= r->nr_fii; k++)
            Add_Color_and_Text(r->urm[k]);
    }
}

void procesare()
{
    /// OPEN FILE
    FILE *fis1;
    fis1 = fopen("text.in", "r");

    while(fgets(text, MAX_TEXT, fis1))
    {
        /// COPY
        strcpy(copie, text);

        /// COUNT NR OF ' '
        int i = 0;
        while(text[i] == ' ')
            i++;

        /// SIMPLIFY
        cod = simplify(text, i);

        /// TYPE
        if(strstr(anterior, "else") && cod == CLASIC)
            cod = FALS;
        else if(strstr(anterior, "repeat") && cod == CLASIC)
            cod = IN_LOOP;
        else if(inainte != NULL && i > inainte->nr_spatii && cod == CLASIC) /// SIGUR NU E CLASIC
        {
            switch(inainte->tip)
            {
            case CONDITIE: /// DUPA CONDITIE SIGUR AM O INSTRUCTIUNE ADEVARATA
                cod = ADV;
                break;
            case LOOP_FIRST: /// DUPA WHILE, FOR...
                cod = IN_LOOP;
                break;
            case SWITCH_CASE:
                cod = CAZ;
                break;
            }
        }

        /// DACA AM CE AFISA
        if(strlen(text) > 1)
        {
            /// ADAUGARE NOD LA ARBORE + RETINEREA ULTIMULUI DREPT REPER
            inainte = creare_nod(text, inainte, i, cod, anterior);
        }

        /// RETINE TEXTUL ANTERIOR
        strcpy(anterior, copie);
    }

    /// CLOSE FILE
    fclose(fis1);
}

void windowInit(){
    ///initwindow(1400, 790);
    initwindow(window_width, window_height,"NS-diagram drawer");
    line(window_width/2, 0, window_width/2, window_height); ///vertical separation line
}
void titlePrint(char title[]){
    settextstyle(10,0,7); ///Text Style
    int txt_height = textheight(title), ///Title Height
    txt_width = textwidth(title);       ///Title Width

    /// Adjusting font size for smaller screens/window
    if(window_width/2 < txt_width){
        settextstyle(10,0,3);
        txt_height = textheight(title), ///Updating
        txt_width = textwidth(title);   ///text sizes
    }

    ///Centering the Title (calculations)
    int txt_startPos = (window_width/2 - txt_width)/2 + window_width/2,  /// centered between window left and window center (vertical line)
    txt_verticalPos = 10;                               /// vertical position

    ///Printing the Centered Title
    outtextxy(txt_startPos, txt_verticalPos, title);

    ///Underline
    line(txt_startPos, txt_verticalPos + txt_height - 5 ,txt_startPos + txt_width, txt_verticalPos + txt_height - 5);
}
poz getCenteredTextXY(int ctrWidth, int ctrHeight, char text[]){    ///Vertical + Horizontal centering
    poz txtCenteredPoz; ///Position to be returned
    int txtHeight, txtWidth;
    txtHeight = textheight(text);
    txtWidth = textwidth(text);

    txtCenteredPoz.x = (ctrWidth - txtWidth) / 2;   ///Centered x coord
    txtCenteredPoz.y = (ctrHeight - txtHeight) / 2; ///Centered y coord

    return txtCenteredPoz; ///returns coordinates
}
int getCenteredTextX(int ctrWidth, char text[]){    ///Horizontal centering
    return (ctrWidth - textwidth(text)) / 2;        ///returns x coordinate
}

int getCenteredTextY(int ctrHeight, char text[]){   ///Vertical centering
    return (ctrHeight - textheight(text)) / 2;      ///returns y coordinate
}

void drawButton(poz btnPoz, int btnWidth, int btnHeight, char btnText[]){
    settextstyle(10,0,3);
    poz txtCentered;
    txtCentered = getCenteredTextXY(btnWidth, btnHeight, btnText); ///Centering the text within the button

    rectangle(btnPoz.x, btnPoz.y, btnPoz.x + btnWidth, btnPoz.y+btnHeight);
    outtextxy(btnPoz.x + txtCentered.x, btnPoz.y + txtCentered.y, btnText);
}

void drawFrame(poz frmPoz, int frmWidth, int frmHeight){
    rectangle(frmPoz.x, frmPoz.y, frmPoz.x + frmWidth, frmPoz.y + frmHeight);
}

void settingsSection(){
    poz secPoz;
    int secWidth, secHeight;
    secPoz.x = 5 * (window_width / 2) / 100 + window_width/2;    ///Starts 5% indented relative to the left half width
    secPoz.y = window_height/5;                                  ///Starts 20% down from the top
    secWidth = 90 * (window_width / 2) / 100;   /// 90% of the left half's width (centered horizontally)
    secHeight = 65 * window_height / 100;       /// 65% of screen's height

    ///Section's frame
    drawFrame(secPoz, secWidth, secHeight);

    ///Section's title
    settextstyle(10,0,5);
    poz titlePoz;
    char secTitle[]="Settings";
    titlePoz.x = secPoz.x + getCenteredTextX(secWidth, secTitle);
    titlePoz.y = secPoz.y + 15;
    outtextxy(titlePoz.x, titlePoz.y, secTitle);

    ///Buttons
    poz btn1;
    btn1.x = 20 + secPoz.x;
    btn1.y = 80 + secPoz.y;
    drawButton(btn1, 100, 50, "test1235");

    ///Moving the console
    HWND consoleWindow = GetConsoleWindow();

    MoveWindow(consoleWindow, secPoz.x, secPoz.y + secHeight/2 , 101*secWidth/100 , 110*secHeight/200, TRUE);
	SetForegroundWindow(consoleWindow);
}
int main()
{
    windowInit();
    titlePrint("NS Diagram Drawer");
    settingsSection();
    settextstyle(stil, HORIZ_DIR, dim);
    procesare();
    desen(radacina);
    Add_Color_and_Text(radacina);
    getch();
    closegraph();
    return 0;
}
