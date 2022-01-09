#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <graphics.h>
#include <fstream>
#include <winbgim.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <cmath>
#define dim 1
#define stil 3
#define x_initial 30
#define y_initial 10
#define distanta 10
#define nr_culori 5
#define thickness 4
#define MAX_TEXT 50
using namespace std;
char text[MAX_TEXT], anterior[MAX_TEXT], predecesor_anterior[MAX_TEXT], copie[MAX_TEXT];
float ratie = 1.5;
char info_engl[] = "\tA Nassi-Shneiderman diagram in computer programming is a graphical design representation for structured programming./This type of diagram was developed in 1972 by Isaac Nassi and Ben Shneiderman./\tFollowing a top-down design, the problem at hand is reduced into smaller and smaller subproblems, until only simple/statements and control flow constructs remain. Nassi–Shneiderman diagrams reflect this top-down decomposition in a/straightforward way, using nested boxes to represent subproblems./\n\tDiagram types:/\t1. Process blocks: it represents the simplest of steps and requires no analysis.When a process/block is encountered, the action inside the block is performed and we move onto the next block./\t2. Branching blocks: there are two types of branching blocks. First (figure on the left) is the simple Yes-No branching/block which offers the program two paths to take depending on whether or not a condition has been fulfilled. Second/(figure on the right) is a multiple branching block. This block usually contains a select case. The block provides/the program with an array of choices./\t3. Testing loops: this block allows the program to loop one or a set of processes until a particular condition is/fulfilled. The process blocks covered by each loop are subset with a side-bar extending out from the condition. There are/two main types of testing loops, test first (figure on the left) and test last blocks(figure on the left). The only/difference between the two is the order in which the steps involved are completed.";
int x_final, y_final,
    window_width=getmaxwidth(),
    window_height=getmaxheight();
int LMAX = window_width / 2 - 50; /// lungimea este adaptata la dimensiunea ferestrei
enum instructiune_speciala {CLASIC, CONDITIE, ADV, FALS, LOOP_FIRST, IN_LOOP, LOOP_LAST, SWITCH_CASE, CAZ, ROOT, IGNORA_DA, IGNORA_NU, IGNORA_LOOP_LAST, IN_CAZ};
instructiune_speciala cod;
int culoare_chenar_meniu;
int culoare = 0;
struct nod
{
    char linie_text[MAX_TEXT];
    int x, y;
    int lungime;
    instructiune_speciala tip;
    int nr_fii = 0;
    int nr_spatii = -1;
    nod * urm[30];
    nod * parinte = NULL;
} *inainte, *radacina;

struct rgb_button_menu
{
    int red = 222;
    int green = 149;
    int blue = 67;
} Culoare_meniu_chenar;

struct rgb_button_menu_bk
{
    int red = 221;
    int green = 195;
    int blue = 165;
} Culoare_hover_meniu;

struct rgb_switch
{
    int red[nr_culori] = {79, 59};
    int green[nr_culori] = {52, 23};
    int blue[nr_culori] = {102, 4};
} CuloareSwitch;

struct rgb_case
{
    int red[nr_culori] = {147, 118};
    int green[nr_culori] = {56, 70};
    int blue[nr_culori] = {95, 4};
} CuloareCase;

struct rgb_in_case
{
    int red[nr_culori] = {159, 187};
    int green[nr_culori] = {107, 123};
    int blue[nr_culori] = {153, 61};
} CuloareInCase;

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
struct poz
{
    int x,y;
};


void color_middle_top(int x_stanga, int y_sus, int Lsus, int colour)
{
    setfillstyle(SOLID_FILL, colour);
    floodfill(x_stanga + Lsus/2, y_sus + 2, WHITE);
    setbkcolor(colour);
}

void color_switch(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareSwitch.red[culoare], CuloareSwitch.green[culoare], CuloareSwitch.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_case(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareCase.red[culoare], CuloareCase.green[culoare], CuloareCase.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_in_case(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareInCase.red[culoare], CuloareInCase.green[culoare], CuloareInCase.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_loop(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareLoop.red[culoare], CuloareLoop.green[culoare], CuloareLoop.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_in_loop(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareInLoop.red[culoare], CuloareInLoop.green[culoare], CuloareInLoop.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);;
}

void color_conditie(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareConditie.red[culoare], CuloareConditie.green[culoare], CuloareConditie.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_DA(int x_stanga, int y_sus)
{
    int colour = COLOR(CuloareDA.red[culoare], CuloareDA.green[culoare], CuloareDA.blue[culoare]);
    setfillstyle(SOLID_FILL, colour);
    floodfill(x_stanga + 1, y_sus + 5, WHITE);
    setbkcolor(colour);
}

void color_NU(int x_dreapta, int y_sus)
{
    int colour = COLOR(CuloareNU.red[culoare], CuloareNU.green[culoare], CuloareNU.blue[culoare]);
    setfillstyle(SOLID_FILL, colour);
    floodfill(x_dreapta - 1, y_sus + 5, WHITE);
    setbkcolor(colour);
}

void color_clasic(int x_stanga, int y_sus, int Lsus)
{
    int colour =  COLOR(CuloareClasic.red[culoare], CuloareClasic.green[culoare], CuloareClasic.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_adv(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareADV.red[culoare], CuloareADV.green[culoare], CuloareADV.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_fals(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareFALS.red[culoare], CuloareFALS.green[culoare], CuloareFALS.blue[culoare]);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
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
    else if(r->tip == SWITCH_CASE)
    {
        int v[30] = {0};
        int maxx = 0;
        for(int k = 1; k <= r->nr_fii; k++)
            v[k] = get_y_from_previous(r->urm[k]);
        for(int k = 1; k <= r->nr_fii; k++)
            if(maxx < v[k])
                maxx = v[k];
        return maxx;
    }
    else
    {
        if (r->nr_fii)
            get_y_from_previous(r->urm[r->nr_fii]);
        else
            return r->y + ratie * textheight(r->linie_text);
    }
}

nod *creare_nod(char text[], nod *anterior, int i, instructiune_speciala cod, char text_anterior[], char text_predecesor_anterior[])
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

        if(strstr(text_anterior, "repeat")) /// CHANGE NODE ORDER
        {
            /// CREATE NODE "until"
            nod *until = new nod;

            /// LINK NODE "until" to ROOT
            q->urm[1] = until;
            until->parinte = q;

            /// INITIALIZATION
            strcpy(until->linie_text, "-until-");

            /// TIP
            until->tip = IGNORA_LOOP_LAST;

            /// LINK "until" NODE TO CHILDREN
            until->nr_fii = 1;
            until->urm[1] = p;
            p->parinte = until;
        }

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
        p->y = anterior->y + 3 * textheight(anterior->linie_text);

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(cod == CAZ)
    {
        /// SEARCH FOR PARENT
        while(anterior->tip != SWITCH_CASE)
            anterior = anterior->parinte;

        /// LINK NODE TO PARENT
        anterior->nr_fii++;
        anterior->urm[anterior->nr_fii] = p;
        p->parinte = anterior;

        /// COORDONATE
        if(anterior->nr_fii == 1)
            p->y = anterior->y + ratie * textheight(text);
        else
            p->y = anterior->urm[1]->y;

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(strstr(text_anterior, "else"))
    {
        /// CREATE NODE "NU"
        nod *q = new nod;
        strcpy(q->linie_text, "NU");
        q->nr_spatii = -1;
        q->tip = IGNORA_NU;

        /// GO TO CONDITIE:: (ADV -> "DA" -> CONDITIE)
        while(anterior->nr_spatii != i - 4)
            anterior = anterior->parinte;

        /// LINK "NU" TO CONDITIE
        anterior->nr_fii++;
        anterior->urm[anterior->nr_fii] = q;
        q->parinte = anterior;

        /// LINK NODE TO "NU"
        q->nr_fii = 1;
        q->urm[q->nr_fii] = p;
        p->parinte = q;

        /// COORDONATE
        p->y = anterior->y + 3 * textheight(anterior->linie_text);

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(strstr(text, "until"))
    {
        /// SEARCH FOR PARENT
        while(anterior->tip != IGNORA_LOOP_LAST)
            anterior = anterior->parinte;

        ///LINK NODE TO PARENT
        anterior->nr_fii++;
        anterior->urm[anterior->nr_fii] = p;
        p->parinte = anterior;

        /// COORDONATE
        p->y = get_y_from_previous(anterior->urm[anterior->nr_fii - 1]);

        /// TIP
        p->tip = LOOP_LAST;

        /// RETURN NODE
        return p;
    }
    else if(strstr(text_anterior, "repeat"))
    {
        /// CREATE NODE "until"
        nod *q = new nod;
        strcpy(q->linie_text, "-until-");
        q->tip = IGNORA_LOOP_LAST;

        /// LINK "until" TO CHILDREN
        q->nr_fii = 1;
        q->urm[1] = p;
        p->parinte = q;

        if(!strstr(predecesor_anterior, "else"))
        {
            p->y = anterior->y + ratie * textheight(anterior->linie_text);

            /// START FROM ROOT
            nod *start = radacina->urm[radacina->nr_fii];

            /// GO DOWN TO SIBLING
            while(i - 4 != start->nr_spatii)
                start = start->urm[start->nr_fii];

            /// GO TO PARENT
            start = start->parinte;

            /// LINK "until" TO PARENT
            start->nr_fii++;
            start->urm[start->nr_fii] = q;
            q->parinte = start;
        }
        else /// else...repreat...(instuctiune)
        {
            /// CREATE NODE "NU"
            nod *nu = new nod;
            strcpy(nu->linie_text, "NU");
            nu->nr_spatii = -1;
            nu->tip = IGNORA_NU;

            /// GO TO CONDITIE
            while(anterior->nr_spatii != i - 8) /// -8 pt ca e intr-un repreat until dintr-un else
                anterior = anterior->parinte;

            /// LINK "NU" TO CONDITIE
            anterior->nr_fii++;
            anterior->urm[anterior->nr_fii] = nu;
            nu->parinte = anterior;

            /// LINK "until" TO "NU"
            nu->nr_fii = 1;
            nu->urm[1] = q;
            q->parinte = nu;

            /// COORDONATE
            p->y = anterior->y + 3 * textheight(anterior->linie_text);
        }

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(anterior->nr_spatii == i)
    {
        /// COORDONATE
        p->y = anterior->y + ratie * textheight(anterior->linie_text);

        /// AU ACELASI PARINTE


        if(anterior->tip != LOOP_LAST)
        {
            /// LINK TO PARENT
            p->parinte = anterior->parinte;

            /// LEG DE PARINTELE FRATELUI
            anterior->parinte->nr_fii++;
            anterior->parinte->urm[anterior->parinte->nr_fii] = p;

            /// TIP
            if(cod == CLASIC)
                p->tip = anterior->tip;
            else
                p->tip = cod; /// poate am IF, FOR, etc.
        }
        else
        {
            /// LINK TO PARENT
            p->parinte = anterior->parinte->parinte; /// loop_last -> -until- -> parent

            /// LEG DE PARINTELE FRATELUI
            p->parinte->nr_fii++;
            p->parinte->urm[p->parinte->nr_fii] = p;

            /// TIP
            if(cod == CLASIC)
                if(p->parinte->tip == IGNORA_NU)
                    p->tip = FALS;
                else if(p->parinte->tip == IGNORA_DA)
                    p->tip = ADV;
                else if(p->parinte->tip == LOOP_FIRST)
                    p->tip = IN_LOOP;
                else if(p->parinte->tip == CAZ)
                    p->tip = IN_CAZ;
                else
                    p->tip = CLASIC;
            else
                p->tip = cod; /// poate am IF, FOR, etc.
        }

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
        p->y = anterior->y + ratie*textheight(anterior->linie_text);

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
        p->y = get_y_from_previous(start);

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
        p->y = start->y + 3 * textheight(start->linie_text);

        /// TIP
        p->tip = cod;

        /// RETURN NODE
        return p;
    }
    else if(anterior->tip == CAZ)
    {
        /// LINK NODE TO PARENT
        anterior->nr_fii++;
        anterior->urm[anterior->nr_fii] = p;
        p->parinte = anterior;

        /// COORDONATE
        p->y = anterior->y + ratie * textheight(text);

        /// TIP
        if(cod == CLASIC)
            p->tip = IN_CAZ;
        else
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
    text[poz] = '\0';

    /// DACA ARE SPATII MULTIPLE INAUNTRU
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);

    /// GO TO END
    int lung = strlen(text);

    /// DACA INAINTE DE ';' E UN SPATIU
    if(text[lung - 1] == ' ')
        strcpy(text + lung - 1, text + lung); /// SCAP DE SPATIUL ACELA
}
void prelucrare_text_switch(char text[])
{
    /// ELIMIN "SWITCH"
    strcpy(text, text + strlen("switch"));

    /// ELIMIN SPATIILE DINTRE SWITCH SI CONDITIE
    while(text[0] == ' ')
        strcpy(text, text+1);

    /// ELIMIN SPATIILE DIN CELALALT CAPAT
    int lung = strlen(text) - 1;
    while(text[lung] == ' ')
        lung--;

    if(text[lung] == '\n')
        text[lung] = '\0';

    /// ADD END OF STRING
    text[lung+1] = '\0';

    /// DACA ARE SPATII MULTIPLE INAUNTRU
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);
}

void prelucrare_text_case(char text[])
{
    /// ELIMIN "CASE"
    strcpy(text, text + strlen("case"));

    /// ELIMIN SPATIILE DINTRE "CASE" SI cazul curent
    while(text[0] == ' ')
        strcpy(text, text+1);

    /// GASESC UNDE INCEPE ":"
    char *capat = strstr(text, ":");
    int poz = (int)(capat - text);

    /// CARACTERUL ANTERIOR
    poz--;

    /// ELIMIN SPATIILE DINTRE cazul curent SI ":"
    while(text[poz] == ' ')
        poz--;

    /// ADD END OF STRING
    text[++poz] = '\0';

    /// DACA ARE SPATII MULTIPLE INAUNTRU
    if(strstr(text, "  "))
        elimin_spatii_multiple(text);
}
instructiune_speciala simplify(char text[], int start)
{
    /// ELIMIN SPATIILE DINAINTE
    strcpy(text, text + start);

    /// daca linia este un comentariu SAU e doar begin / end / else
    if(strstr(text, "begin") || strstr(text, "end") || strstr(text, "else") || strstr(text, "repeat"))
    {
        text[0] = '\0';
        return CLASIC;
    }

    /// ELIMIN COMENTARIILE
    char *p = strstr(text, "//");/// pt a elimina comentariile de tip //
    if(p != NULL)
    {
        int pozitie_comentariu = (int)(p - text);/// gasesc de unde incepe comentariul
        text[pozitie_comentariu] = '\0';
    }

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

    /// FOR
    if((int)(strstr(text, "for ") - text) == 0)
    {
        prelucrare_text_while_for(text);
        return LOOP_FIRST;
    }
    /// SWITCH
    if((int)(strstr(text, "switch ") - text) == 0)
    {
        prelucrare_text_switch(text);
        return SWITCH_CASE;
    }

    /// CASE
    if((int)(strstr(text, "case ") - text) == 0)
    {
        prelucrare_text_case(text);
        return CAZ;
    }
    /// DEFAULT
    if((int)(strstr(text, "default") - text) == 0)
    {
        strcpy(text, "default");
        return CAZ;
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
}

void DrawConditie(char text[], int x, int y, int Lsus)
{
    int Lungime = textwidth(text);
    int latime = textheight(text);
    line(x, y, x + Lsus, y);
    DrawTriangle_true(x, y, x + Lsus/2, y + 3*latime);
    DrawTriangle_false(x+Lsus, y, x + Lsus/2, y + 3*latime);
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

void set_x_and_lungime(nod *r, int nr_fiu)
{
    if(r != NULL)
    {
        if(r != radacina)
        {
            switch(r->parinte->tip)
            {
            case LOOP_FIRST: /// PARINTELE ESTE UN FOR, WHILE

                /// MA DUC SPRE DREAPTA CU O DISTANTA
                r->x = r->parinte->x + distanta;

                /// SCAD DISTANTA DIN LUNGIME
                r->lungime = r->parinte->lungime - distanta;
                break;
            case IGNORA_LOOP_LAST: /// FACE PARTE DINTR-UN REPEAT...UNTIL

                if(r->parinte->parinte->tip != IGNORA_NU)
                {
                    /// MA DUC SPRE DREAPTA CU O DISTANTA
                    r->x = r->parinte->x + distanta;

                    /// SCAD DISTANTA DIN LUNGIME
                    r->lungime = r->parinte->lungime - distanta;
                }
                else
                {
                    ///COORDONATA NORMALA PE AXA OX
                    r->x = r->parinte->parinte->parinte->urm[1]->urm[1]->x + r->parinte->parinte->parinte->urm[1]->urm[1]->lungime;

                    /// MA DUC SPRE DREAPTA CU O DISTANTA
                    r->x += distanta;

                    /// ARE ACEEASI LUNGIME CU CEA DE PE RAMURA TRUE
                    r->lungime = r->parinte->parinte->parinte->urm[1]->urm[1]->lungime;

                    /// SCAD DISTANTA DIN LUNGIME
                    r->lungime -= distanta;
                }
                if(r->tip == LOOP_LAST)
                {
                    r->x -= distanta;
                    r->lungime += distanta;
                }
                break;
            case ROOT:
                /// INITIALIZARE STANDARD
                r->x = x_initial;
                r->lungime = LMAX;
                break;
            case IGNORA_NU: /// PT INSTRUCTIUNILE DE PE RAMURA FALSE
                /// CAUT PRIMA INSTRUCTIUNE DE PE RAMURA TRUE SI MA RAPORTEZ LA EA

                /// COORDONATA PE AXA OX
                r->x = r->parinte->parinte->urm[1]->urm[1]->x + r->parinte->parinte->urm[1]->urm[1]->lungime;

                /// ARE ACEEASI LUNGIME CU CEA DE PE RAMURA TRUE
                r->lungime = r->parinte->parinte->urm[1]->urm[1]->lungime;
                break;
            case IGNORA_DA: /// PT INSTRUCTIUNILE DE PE RAMURA TRUE
                /// MA RAPORTEZ LA CONDITIE
                r->x = r->parinte->parinte->x; /// ACEEASI COORDONATA PE AXA OX

                /// JUMATATE DIN LUNGIMEA CONDITIEI
                r->lungime = r->parinte->parinte->lungime/2;
                break;
            case SWITCH_CASE:
                if(nr_fiu == 1)
                {
                    r->lungime = r->parinte->lungime / r->parinte->nr_fii;
                    r->x = r->parinte->x;
                }
                else if(nr_fiu == r->parinte->nr_fii)
                {
                    r->x = r->parinte->x + (nr_fiu-1) * r->parinte->urm[1]->lungime;
                    r->lungime = r->parinte->lungime + r ->parinte->x - r->x;
                }
                else
                {
                    r->lungime = r->parinte->urm[1]->lungime;
                    r->x = r->parinte->x + (nr_fiu-1) * r->lungime;
                }
                break;
            default:
                /// NU APAR SCHIMBARI
                r->x = r->parinte->x;
                r->lungime = r->parinte->lungime;
                break;
            }
        }
        else
        {
            /// pt radacina
            r->x = x_initial;
            r->lungime = LMAX;
        }

        for(int k = 1; k <= r->nr_fii; k++) /// GO TO CHILDREN
            set_x_and_lungime(r->urm[k], k);
    }
}

void desen(nod *r)
{
    if(r != NULL)
    {
        if(r!= radacina && r->tip != IGNORA_DA && r->tip != IGNORA_NU && r->tip != IGNORA_LOOP_LAST)
        {
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
            case CAZ:
                DrawRectangle(r->linie_text, r->x, r->y, r->lungime);

                /// chenar
                chenar(r->x, r->y, r->x + r->lungime, get_y_from_previous(r->parinte));
                break;
            case LOOP_LAST:

                /// chenar mic interior
                chenar(r->x + distanta, r->parinte->urm[1]->y, r->x + r->lungime, r->y);

                /// chenar mare exterior
                chenar(r->x, r->parinte->urm[1]->y, r->x + r->lungime, r->y + ratie * textheight(r->linie_text));
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
        if(r!= radacina && r->tip != IGNORA_DA && r->tip != IGNORA_NU && r->tip != IGNORA_LOOP_LAST)
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
            case SWITCH_CASE:
                color_switch(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case CAZ:
                color_case(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case IN_CAZ:
                color_in_case(r->x, r->y, r->lungime);
                DrawRectangle_text(r->linie_text, r->x, r->y, r->lungime);
                break;
            case LOOP_LAST:
                color_loop(r->x, r->y, r->lungime);
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

void procesare(char userfilename[])
{

    /// OPEN FILE
    FILE *fis1;
    fis1 = fopen(userfilename, "r");

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
            }
        }

        /// DACA AM CE AFISA
        if(strlen(text) >= 1)
        {
            /// ADAUGARE NOD LA ARBORE + RETINEREA ULTIMULUI DREPT REPER
            inainte = creare_nod(text, inainte, i, cod, anterior, predecesor_anterior);
        }

        /// RETINE TEXTUL ANTERIOR
        strcpy(predecesor_anterior, anterior);
        strcpy(anterior, copie);
    }

    /// CLOSE FILE
    fclose(fis1);
}

void windowInit()
{
    ///initwindow(1400, 790);
    ///initwindow(window_width, window_height,"NS-diagram drawer");
    setbkcolor(BLACK);
    cleardevice();
    delay(200);
    line(window_width/2, 0, window_width/2, window_height); ///vertical separation line
}
void titlePrint(char title[])
{
    settextstyle(10,0,7); ///Text Style
    int txt_height = textheight(title), ///Title Height
        txt_width = textwidth(title);       ///Title Width

    /// Adjusting font size for smaller screens/window
    if(window_width/2 < txt_width)
    {
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
    line(txt_startPos, txt_verticalPos + txt_height - 5,txt_startPos + txt_width, txt_verticalPos + txt_height - 5);
}
poz getCenteredTextXY(int ctrWidth, int ctrHeight, char text[])     ///Vertical + Horizontal centering
{
    poz txtCenteredPoz; ///Position to be returned
    int txtHeight, txtWidth;
    txtHeight = textheight(text);
    txtWidth = textwidth(text);

    txtCenteredPoz.x = (ctrWidth - txtWidth) / 2;   ///Centered x coord
    txtCenteredPoz.y = (ctrHeight - txtHeight) / 2; ///Centered y coord

    return txtCenteredPoz; ///returns coordinates
}
int getCenteredTextX(int ctrWidth, char text[])     ///Horizontal centering
{
    return (ctrWidth - textwidth(text)) / 2;        ///returns x coordinate
}

int getCenteredTextY(int ctrHeight, char text[])    ///Vertical centering
{
    return (ctrHeight - textheight(text)) / 2;      ///returns y coordinate
}

void drawButton(poz btnPoz, int btnWidth, int btnHeight, char btnText[])
{
    settextstyle(10,0,3);
    poz txtCentered;
    txtCentered = getCenteredTextXY(btnWidth, btnHeight, btnText); ///Centering the text within the button

    rectangle(btnPoz.x, btnPoz.y, btnPoz.x + btnWidth, btnPoz.y+btnHeight);
    outtextxy(btnPoz.x + txtCentered.x, btnPoz.y + txtCentered.y, btnText);
}

void drawFrame(poz frmPoz, int frmWidth, int frmHeight)
{
    rectangle(frmPoz.x, frmPoz.y, frmPoz.x + frmWidth, frmPoz.y + frmHeight);
}
void scriere_fisier()
{
    FILE *prgmNS;
    prgmNS = fopen("program.ns", "w");

    char ch;
    while( (ch = getchar()) != EOF )
    {
        putc(ch, prgmNS);
    }

    fclose(prgmNS);
}
///Buttons List
struct btn
{
    poz coord;
    char txt[31];
    int width, height, event;
} b[20];

char bttns[20][31];
void generate_buttons (btn b[], poz ctr)
{
    int i=0;
    strcpy(bttns[0], "New");
    strcpy(bttns[1], "Generate");

    while(bttns[i][0]!=NULL)
    {
        b[i].coord.x = 20 + 170*(i) + ctr.x;
        b[i].coord.y = 80 + ctr.y;
        b[i].width = 120;
        b[i].height = 50;
        b[i].event = i+1;
        strcpy(b[i].txt, bttns[i]);
        drawButton(b[i].coord, b[i].width, b[i].height, b[i].txt);
        i++;
    }
}
void bringConsoleToFront()
{
    HWND consoleWindow = GetConsoleWindow();
    SetForegroundWindow(consoleWindow);
}
void settingsSection()
{
    poz secPoz;
    int secWidth, secHeight;
    secPoz.x = 5 * (window_width / 2) / 100 + window_width/2;    ///Starts 5% indented relative to the right half width
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
    generate_buttons(b,secPoz);

    ///Moving the console
    HWND consoleWindow = GetConsoleWindow();

    MoveWindow(consoleWindow, secPoz.x, secPoz.y + secHeight/2, 101*secWidth/100, 110*secHeight/200, TRUE);
    bringConsoleToFront();
}

void delete_tree(nod *r)
{
    if(r != NULL)
    {
        for(int k = 1; k <= r->nr_fii; k++)
            delete_tree(r->urm[k]);
        if(r != radacina)
        {
            delete(r);
        }
        else /// RADACINA
        {
            for(int k = 1; k <= r->nr_fii; k++)
                r->urm[k] = NULL;

            ///RESET NR OF CHILDREN
            r->nr_fii = 0;

            /// RESET inainte
            inainte = NULL;
        }
    }
}

void clearLeftHalf()
{
    setbkcolor(BLACK);
    ///SELECT LEFT HALF
    setviewport(0, 0, window_width/2 - 2, window_height, 1);
    clearviewport();
    ///BACK TO ALL SCREEN
    setviewport(0, 0, window_width, window_height, 0);
}

void diagram_generation()
{
    /// CLEAR LEFT HALF
    clearLeftHalf();

    /// DELETE TREE
    delete_tree(radacina);


    bringConsoleToFront();
    settextstyle(stil, HORIZ_DIR, dim);
    procesare("program.ns");
    set_x_and_lungime(radacina, 1);
    desen(radacina);
    Add_Color_and_Text(radacina);
}
void event_handler(poz mouse)
{
    //int i=0;
    //while(bttns[i][0]!=NULL){
    if (mouse.x>=b[0].coord.x && mouse.x<=b[0].coord.x + b[0].width && mouse.y>=b[0].coord.y && mouse.y<=b[0].coord.y + b[0].height)
        scriere_fisier();
    else if (mouse.x>=b[1].coord.x && mouse.x<=b[1].coord.x + b[1].width && mouse.y>=b[1].coord.y && mouse.y<=b[1].coord.y + b[1].height)
        diagram_generation();
    //}
}
void console_handler()
{
    ///Setting console title
    SetConsoleTitle("NS-diagram Console");

    do
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            bringConsoleToFront();
            //fflush(stdin);
            poz mouse;
            mouse.x=mousex();
            mouse.y=mousey();
            event_handler(mouse);
        }
    }
    while(1);

    //scriere_fisier();
    //diagram_generation();
}
#define nr_butoane_meniu 4
struct buton
{
    poz coordonate;
    char mesaj[31];
    int width = 300, height = 70;
} b_meniu[nr_butoane_meniu + 1], b_limbi[nr_butoane_meniu + 1];

void drawButton_menu_lang(int numar_buton, buton b[])
{
    settextstyle(10, 0, 4);
    poz txtCentered = getCenteredTextXY(b[numar_buton].width, b[numar_buton].height, b[numar_buton].mesaj); ///Centering the text within the button

    /// SELECT COLOR
    setcolor(culoare_chenar_meniu);

    /// cele 4 colturi ale chenarului ingrosat
    poz left_top, right_top, right_down, left_down;
    left_top.x = left_down.x = b[numar_buton].coordonate.x - thickness;
    right_top.x = right_down.x = b[numar_buton].coordonate.x + b[numar_buton].width + thickness;
    left_top.y = right_top.y = b[numar_buton].coordonate.y - thickness;
    left_down.y = right_down.y = b[numar_buton].coordonate.y + b[numar_buton].height + thickness;

    setfillstyle(SOLID_FILL, culoare_chenar_meniu);

    /// CHENAR
    bar(left_top.x, left_top.y, right_top.x, right_top.y + thickness);
    bar(right_top.x - thickness, right_top.y, right_down.x, right_down.y);
    bar(left_down.x, left_down.y - thickness, right_down.x, right_down.y);
    bar(left_top.x, left_top.y, left_down.x + thickness, left_down.y);

    outtextxy(b[numar_buton].coordonate.x + txtCentered.x, b[numar_buton].coordonate.y + txtCentered.y, b[numar_buton].mesaj);

    /// RESTORE COLOR
    setcolor(WHITE);
}

void AddBorder(int numar_buton, buton b[])
{

    int border = thickness / 2 + 1; /// grosimea bordurii
    int inner_distance_from_button = thickness - 1;
    int outer_distance_from_button = 3 * inner_distance_from_button + border;

    /// cele 4 colturi ale bordurii
    poz left_top, right_top, right_down, left_down;
    left_top.x = left_down.x = b[numar_buton].coordonate.x - outer_distance_from_button;
    right_top.x = right_down.x = b[numar_buton].coordonate.x + b[numar_buton].width + outer_distance_from_button;
    left_top.y = right_top.y = b[numar_buton].coordonate.y - outer_distance_from_button;
    left_down.y = right_down.y = b[numar_buton].coordonate.y + b[numar_buton].height + outer_distance_from_button;

    setfillstyle(SOLID_FILL, culoare_chenar_meniu);

    /// BORDURA
    bar(left_top.x, left_top.y, right_top.x, right_top.y + inner_distance_from_button);
    bar(right_top.x - inner_distance_from_button, right_top.y, right_down.x, right_down.y);
    bar(left_down.x, left_down.y - inner_distance_from_button, right_down.x, right_down.y);
    bar(left_top.x, left_top.y, left_down.x + inner_distance_from_button, left_down.y);
}

int printTitle_meniu(char title[])
{
    int dimensiue_text = 9;
    settextstyle(10, 0, dimensiue_text);
    int text_width = textwidth(title); /// Width

    /// daca textul e prea mare
    while(window_width < text_width)
    {
        dimensiue_text--;
        settextstyle(10, 0, dimensiue_text);
        ///actualizare
        text_width = textwidth(title);
    }
    int text_height = textheight(title); /// Height

    ///coordonate pt outtextxy
    poz coordonate_text;
    coordonate_text.x = (window_width - text_width)/2;
    coordonate_text.y = 30;

    /// Centered Title
    outtextxy(coordonate_text.x, coordonate_text.y, title);

    /// poz Underline -> Oy
    int linie_y = coordonate_text.y + text_height - 5;

    /// Underline
    line(coordonate_text.x, linie_y, coordonate_text.x + text_width, linie_y);

    /// returneaza poz linie (pt cum pui butoanele)
    return linie_y;
}

void generare_butoane_mijloc(int start_y, buton b[], int nr_butoane)
{
    for(int i = 0; i <= nr_butoane - 1; i++)
    {
        b[i].coordonate.x = (window_width - b[0].width) / 2;
        b[i].coordonate.y = start_y + i * 2 * b[0].height;
        drawButton_menu_lang(i, b);
    }
}

void txt_menu_fran()
{
    /// button text
    strcpy(b_meniu[0].mesaj, "Debut");
    strcpy(b_meniu[1].mesaj, "Informations");
    strcpy(b_meniu[2].mesaj, "Langue");
    strcpy(b_meniu[3].mesaj, "Sortie");
    strcpy(b_meniu[4].mesaj, "Retour");
}

int meniu_fran()
{
    txt_menu_fran();
    /// printing Title + start poz for buttons
    int y_underline = printTitle_meniu("Dessinateur diagramme N-S");

    /// set where the buttons start
    int start_y = y_underline + ((window_height - y_underline) - (2 * nr_butoane_meniu - 1) * b_meniu[0].height) / 3; /// incepe la 1/3 din spatiul exterior disponibil blocului de butoane

    return start_y;

}

void txt_menu_engl()
{
    /// button text
    strcpy(b_meniu[0].mesaj, "Start");
    strcpy(b_meniu[1].mesaj, "Information");
    strcpy(b_meniu[2].mesaj, "Language");
    strcpy(b_meniu[3].mesaj, "Exit");
    strcpy(b_meniu[4].mesaj, "Back");
}

int meniu_engl()
{
    txt_menu_engl();
    /// printing Title + start poz for buttons
    int y_underline = printTitle_meniu("NS-diagram drawer");

    /// set where the buttons start
    int start_y = y_underline + ((window_height - y_underline) - (2 * nr_butoane_meniu - 1) * b_meniu[0].height) / 3; /// incepe la 1/3 din spatiul exterior disponibil blocului de butoane

    return start_y;

}

void txt_menu_ro()
{
    /// button text
    strcpy(b_meniu[0].mesaj, "Incepe");
    strcpy(b_meniu[1].mesaj, "Informatii");
    strcpy(b_meniu[2].mesaj, "Limba");
    strcpy(b_meniu[3].mesaj, "Iesire");
    strcpy(b_meniu[4].mesaj, "Inapoi");
}

int meniu_ro()
{
    txt_menu_ro();
    /// printing Title + start poz for buttons
    int y_underline = printTitle_meniu("Desenator diagrama N-S");

    /// set where the buttons start
    int start_y = y_underline + ((window_height - y_underline) - (2 * nr_butoane_meniu - 1) * b_meniu[0].height) / 3; /// incepe la 1/3 din spatiul exterior disponibil blocului de butoane

    return start_y;

}

bool is_hover(int x, int y, int numar_buton, buton b[])
{
    if(x >= b[numar_buton].coordonate.x && x <= (b[numar_buton].coordonate.x + b[numar_buton].width) && y >= b[numar_buton].coordonate.y && y <= (b[numar_buton].coordonate.y + b[numar_buton].height))
        return true;
    return false;
}

void clear_button(int numar_buton, buton b[])
{
    setbkcolor(BLACK);
    ///SELECT BUTTON ZONE
    setviewport(b[numar_buton].coordonate.x - thickness, b[numar_buton].coordonate.y - thickness, b[numar_buton].coordonate.x + b[numar_buton].width + thickness, b[numar_buton].coordonate.y + b[numar_buton].height + thickness, 1);
    clearviewport();
    ///BACK TO ALL SCREEN
    setviewport(0, 0, window_width, window_height, 0);
}

void drawButton_meniu_hover(int numar_buton, buton b[])
{
    /// SELECT COLOR
    setcolor(culoare_chenar_meniu);

    settextstyle(10, 0, 4);
    poz txtCentered = getCenteredTextXY(b[numar_buton].width, b[numar_buton].height, b[numar_buton].mesaj); ///Centering the text within the button

    int culoare = COLOR(Culoare_hover_meniu.red, Culoare_hover_meniu.green, Culoare_hover_meniu.blue);
    setfillstyle(SOLID_FILL, culoare);
    floodfill(b[numar_buton].coordonate.x + 2, b[numar_buton].coordonate.y + 2, culoare_chenar_meniu);
    setbkcolor(culoare);
    outtextxy(b[numar_buton].coordonate.x + txtCentered.x, b[numar_buton].coordonate.y + txtCentered.y, b[numar_buton].mesaj);

    /// RESTORE COLOR
    setcolor(WHITE);
}

void button_hover(int numar_buton, buton b[])
{
    drawButton_meniu_hover(numar_buton, b);
}

void back_to_normal(int numar_buton, buton b[])
{
    clear_button(numar_buton, b);
    drawButton_menu_lang(numar_buton, b);
}

bool hover_meniu_and_back[nr_butoane_meniu + 1];
bool hover_limbi[nr_butoane_meniu + 1];
struct limba
{
    bool ro;
    bool engl;
    bool fran;
} LimbaSelectata;

void generate_button_back()
{
    int distantata_de_margine = 20;
    int nr_buton = 4;
    /// coordonate
    b_meniu[nr_buton].coordonate.x = window_width - b_meniu[nr_buton].width - distantata_de_margine;
    b_meniu[nr_buton].coordonate.y = window_height - b_meniu[nr_buton].height - distantata_de_margine;

    drawButton_menu_lang(nr_buton, b_meniu);
}

void Draw_conditie_exemplu(int y_start, char text_conditie[], char text_adv[], char text_fals[])
{
    int Lsus = 3 * textwidth(text_conditie);
    /// conditie
    DrawConditie(text_conditie, (window_width/2 - Lsus)/2, y_start, Lsus);
    DrawConditie_Text(text_conditie, (window_width/2 - Lsus)/2, y_start, Lsus);
    /// adv
    rectangle((window_width/2 - Lsus)/2, y_start + 3 * textheight(text_conditie), window_width/4, y_start + 3 * textheight(text_conditie) + ratie*textheight(text_adv));
    color_adv((window_width/2 - Lsus)/2, y_start + 3 * textheight(text_conditie), Lsus/2);
    DrawRectangle_text(text_adv, (window_width/2 - Lsus)/2, y_start + 3 * textheight(text_conditie), Lsus/2);
    /// fals
    rectangle(window_width/4, y_start + 3 * textheight(text_conditie), window_width/4 + Lsus/2, y_start + 3 * textheight(text_conditie) + ratie*textheight(text_fals));
    color_fals(window_width/4, y_start + 3 * textheight(text_conditie), Lsus/2);
    DrawRectangle_text(text_fals, window_width/4, y_start + 3 * textheight(text_conditie), Lsus/2);
}

void Draw_switch_case_exemplu(int y_start, char text_caz[], char text_in_case[])
{
    int Lsus = 9 * textwidth(text_caz);
    /// coordonate start
    poz coordonate;
    coordonate.x = window_width/2 + (window_width/2 - Lsus)/2;
    coordonate.y = y_start;

    /// chenar bloc intreb
    rectangle(coordonate.x, coordonate.y, coordonate.x + Lsus, coordonate.y + 3 * textheight(text_caz) * ratie);

    /// desen pt tipurile case si in_case
    for(int j = 0; j <= 2; j++)
    {
        DrawRectangle(text_caz, coordonate.x + Lsus/3 * j, coordonate.y + textheight(text_caz) * ratie, Lsus/3);
        DrawRectangle(text_in_case, coordonate.x + Lsus/3 * j, coordonate.y + 2 * textheight(text_caz) * ratie, Lsus/3);
    }

    /// switch
    color_switch(coordonate.x, coordonate.y, Lsus);
    DrawRectangle_text(text_caz, coordonate.x, coordonate.y, Lsus);

    char copie_caz[15], copie_in_caz[15];
    strcpy(copie_caz, text_caz);
    strcpy(copie_in_caz, text_in_case);

    for(int j = 0; j <= 2; j++)
    {
        switch(j) /// add number of case
        {
        case 0:
            strcat(copie_caz, " 1");
            strcat(copie_in_caz, " 1");
            break;
        case 1:
            strcat(copie_caz, " 2");
            strcat(copie_in_caz, " 2");
            break;
        case 2:
            strcat(copie_caz, " 3");
            strcat(copie_in_caz, " 3");
            break;
        }
        /// case
        color_case(coordonate.x + Lsus/3 * j, coordonate.y + textheight(text_caz) * ratie, Lsus/3);
        DrawRectangle_text(copie_caz, coordonate.x + Lsus/3 * j, coordonate.y + textheight(text_caz) * ratie, Lsus/3);
        color_in_case(coordonate.x + Lsus/3 * j, coordonate.y + 2 * textheight(text_caz) * ratie, Lsus/3);
        /// in_case
        DrawRectangle_text(copie_in_caz, coordonate.x + Lsus/3 * j, coordonate.y + 2 * textheight(text_caz) * ratie, Lsus/3);
        strcpy(copie_caz, text_caz);
        strcpy(copie_in_caz, text_in_case);
    }
}

void Draw_example_loop_last(int y_start, char text_loop[], char text_in_loop[])
{
    int Lsus = window_width/3;
    /// coordonate
    poz inceput;
    inceput.x = window_width/2 + (window_width/2 - Lsus)/2;
    inceput.y = y_start;

    /// chenare (unul exterior si unul interior)
    chenar(inceput.x, inceput.y, inceput.x + Lsus, inceput.y + 3 * ratie*textheight(text_loop));
    chenar(inceput.x + distanta, inceput.y, inceput.x + Lsus, inceput.y + 2 * ratie*textheight(text_loop));

    /// draw in_loop
    for(int j = 0; j <= 1; j++)
        DrawRectangle(text_in_loop, inceput.x + distanta, inceput.y + j *ratie*textheight(text_loop), Lsus - distanta);

    char copie_actiune[15];
    strcpy(copie_actiune, text_in_loop);
    for(int j = 0; j <= 1; j++) /// in_loop
    {
        switch(j) /// add number of action
        {
        case 0:
            strcat(copie_actiune, " 1");
            break;
        case 1:
            strcat(copie_actiune, " 2");
            break;
        }
        /// action
        color_in_loop(inceput.x + distanta, inceput.y + j *ratie*textheight(text_loop), Lsus - distanta);
        DrawRectangle_text(copie_actiune, inceput.x + distanta, inceput.y +  j *ratie*textheight(text_loop), Lsus - distanta);
        strcpy(copie_actiune, text_in_loop);
    }
    /// loop
    color_loop(inceput.x, inceput.y + 2 * ratie*textheight(text_loop), Lsus);
    DrawRectangle_text(text_loop, inceput.x, inceput.y + 2 * ratie*textheight(text_loop), Lsus);
}

void Draw_example_loop_first(int y_start, char text_loop[], char text_in_loop[])
{
    int Lsus = window_width/3;
    /// coordonate
    poz inceput;
    inceput.x = (window_width/2 - Lsus)/2;
    inceput.y = y_start;

    /// chenar exterior
    chenar(inceput.x, inceput.y, inceput.x + Lsus, inceput.y + 3 * ratie*textheight(text_loop));

    /// draw in_loop
    for(int j = 0; j <= 1; j++)
        DrawRectangle(text_in_loop, inceput.x + distanta, inceput.y + (j+1) *ratie*textheight(text_loop), Lsus - distanta);

    /// loop
    color_loop(inceput.x, inceput.y, Lsus);
    DrawRectangle_text(text_loop, inceput.x, inceput.y, Lsus);

    char copie_actiune[15];
    strcpy(copie_actiune, text_in_loop);
    for(int j = 0; j <= 1; j++) /// in_loop
    {
        switch(j) /// add number of action
        {
        case 0:
            strcat(copie_actiune, " 1");
            break;
        case 1:
            strcat(copie_actiune, " 2");
            break;
        }
        ///in _loop
        color_in_loop(inceput.x + distanta, inceput.y + (j+1) *ratie*textheight(text_loop), Lsus - distanta);
        DrawRectangle_text(copie_actiune, inceput.x + distanta, inceput.y + (j+1) *ratie*textheight(text_loop), Lsus - distanta);
        strcpy(copie_actiune, text_in_loop);
    }
}

void Draw_example_clasic(poz start, char text_exemplu[])
{
    /// coordonate
    poz capat;
    capat.x = window_width - 10;
    capat.y = start.y + 2* textheight(text_exemplu) - 5;
    /// drawing
    rectangle(start.x, start.y, capat.x, capat.y);
    color_clasic(start.x, start.y, capat.x - start.x);
    DrawRectangle_text(text_exemplu, start.x, start.y, capat.x - start.x);

    setbkcolor(BLACK);
}

void diagram_types(char text[], int marime, poz &start)
{
    settextstyle(10, 0, marime + 3); /// new text size
    outtextxy(start.x, start.y, text); /// display text

    /// line
    start.y = start.y + textheight(text);
    line(start.x, start.y, start.x + textwidth(text), start.y);

    /// restore text size
    settextstyle(10, 0, marime);
}

void afisare_info(int marime, char info_limba[])
{
    char info[2000];
    strcpy(info, info_limba);
    settextstyle(10, 0, marime);
    int distanta_randuri = textheight(info);
    poz start;
    start.y = 10;
    char *p = strtok(info, "/");
    while(p)
    {
        start.x = 5;
        if(strstr(p, "\t")) /// tab
            start.x += 20;
        if(strstr(p, "\n")) /// empty row
            start.y += distanta_randuri;
        if(textwidth(p) + start.x > window_width) /// for smaller screens
        {
            setbkcolor(BLACK);
            cleardevice();
            afisare_info(marime - 1, info_limba);
            return;
        }
        start.y += distanta_randuri;
        if(strstr(p, "3."))
        {
            char condition[15], path_1[15], path_2[15], caz[15], path[15];
            if(LimbaSelectata.engl) /// cuvinte in engleza
            {
                strcpy(condition, "Condition?");
                strcpy(path_1, "Path 1");
                strcpy(path_2, "Path 2");
                strcpy(path, "Path");
                strcpy(caz, "Case");
            }
            else if(LimbaSelectata.ro) /// cuvinte in romana
            {
                strcpy(condition, "Conditie?");
                strcpy(path_1, "Calea 1");
                strcpy(path_2, "Calea 2");
                strcpy(path, "Calea");
                strcpy(caz, "Caz");
            }
            else /// cuvinte in franceza
            {
                strcpy(condition, "Condition?");
                strcpy(path_1, "Route 1");
                strcpy(path_2, "Route 2");
                strcpy(path, "Route");
                strcpy(caz, "Cas");
            }
            Draw_conditie_exemplu(start.y, condition, path_1, path_2); /// blocul if-else
            Draw_switch_case_exemplu(start.y, caz, path); /// blocul switch case
            setbkcolor(BLACK);
            start.y += 3 * textheight(caz) * ratie + 5; /// adaugam dimensiunea blocurilor + 5
        }
        if(strstr(p, "Diagram types:") || strstr(p, "Types de diagrammes:") || strstr(p, "Tipuri de diagrame:"))
        {
            if(LimbaSelectata.engl)
                diagram_types("Diagram types:", marime, start);
            else if(LimbaSelectata.ro)
                diagram_types("Tipuri de diagrame", marime, start);
            else
                diagram_types("Types de diagrammes:", marime, start);
        }
        else
        {
            outtextxy(start.x, start.y, p); /// display text
        }
        if(strstr(p, "1."))
        {
            start.x = start.x + textwidth(p) + 5; /// where the block starts

            /// select language
            if(LimbaSelectata.engl)
                Draw_example_clasic(start, "Action");
            else if(LimbaSelectata.ro)
                Draw_example_clasic(start, "Actiune");
            else
                Draw_example_clasic(start, "Action");
        }
        p = strtok(NULL, "/");
    }

    start.y += distanta_randuri; /// next line
    start.y += 5; /// un pic mai jos

    if(LimbaSelectata.engl)
    {
        Draw_example_loop_first(start.y, "Condition", "Action");
        Draw_example_loop_last(start.y, "Condition", "Action");
    }
    else if(LimbaSelectata.ro)
    {
        Draw_example_loop_first(start.y, "Conditie", "Actiune");
        Draw_example_loop_last(start.y, "Conditie", "Actiune");
    }
    else
    {
        Draw_example_loop_first(start.y, "Condition", "Action");
        Draw_example_loop_last(start.y, "Condition", "Action");
    }
}

void afisare_info()
{
    if(LimbaSelectata.engl)
        afisare_info(2, info_engl);
}

void fereastra_instrctiuni()
{
    setbkcolor(BLACK);
    cleardevice();
    generate_button_back();
    afisare_info();
    poz mouse;
    int i = 4;
    bool inapoi = 0;
    do /// check back button
    {
        mouse.x = mousex();
        mouse.y = mousey();
        if(is_hover(mouse.x, mouse.y, i, b_meniu))
        {
            if(hover_meniu_and_back[i] == 0) /// daca nu era cursorul deja pe buton
            {
                hover_meniu_and_back[i] = 1; /// actualizare stare
                button_hover(i, b_meniu); /// add hover effect
            }
        }
        else
        {
            if(hover_meniu_and_back[i])
            {
                back_to_normal(i, b_meniu);
                hover_meniu_and_back[i] = 0;
            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            if(is_hover(mouse.x, mouse.y, i, b_meniu))
            {
                AddBorder(i, b_meniu);
                delay(350);
                inapoi = 1;
            }
        }
    }
    while(!inapoi);
}

void buttons_languages_mesaj_fran()
{
    strcpy(b_limbi[0].mesaj, "Roumain");
    strcpy(b_limbi[1].mesaj, "Anglais");
    strcpy(b_limbi[2].mesaj, "Francais");
}

void buttons_languages_mesaj_engl()
{
    strcpy(b_limbi[0].mesaj, "Romanian");
    strcpy(b_limbi[1].mesaj, "English");
    strcpy(b_limbi[2].mesaj, "French");
}

void buttons_languages_mesaj_ro()
{
    strcpy(b_limbi[0].mesaj, "Romana");
    strcpy(b_limbi[1].mesaj, "Engleza");
    strcpy(b_limbi[2].mesaj, "Franceza");
}

void generate_buttons_languages()
{
    /// select text
    if(LimbaSelectata.ro)
        buttons_languages_mesaj_ro();
    else if(LimbaSelectata.engl)
        buttons_languages_mesaj_engl();
    else if(LimbaSelectata.fran)
        buttons_languages_mesaj_fran();

    generare_butoane_mijloc((window_height - 5 * b_limbi[0].height) / 3, b_limbi, 3);

    /// add border
    if(LimbaSelectata.ro)
        AddBorder(0, b_limbi);
    else if(LimbaSelectata.engl)
        AddBorder(1, b_limbi);
    else if(LimbaSelectata.fran)
        AddBorder(2, b_limbi);
}

void fereastra_limbi()
{
    setbkcolor(BLACK);
    cleardevice();
    generate_buttons_languages();
    generate_button_back();
    poz mouse;
    int i = 4;
    bool inapoi = 0, refresh = 0;
    do
    {
        mouse.x = mousex();
        mouse.y = mousey();
        /// button back
        if(is_hover(mouse.x, mouse.y, i, b_meniu))
        {
            if(hover_meniu_and_back[i] == 0) /// daca nu era cursorul deja pe buton
            {
                hover_meniu_and_back[i] = 1; /// actualizare stare
                button_hover(i, b_meniu); /// add hover effect
            }
        }
        else /// nu mai e cursorul pe buton
        {
            if(hover_meniu_and_back[i])
            {
                back_to_normal(i, b_meniu); /// without hover effect
                hover_meniu_and_back[i] = 0; /// actualizare stare
            }
        }
        /// the other buttons
        for(int j = 0; j <= 2; j++)
        {
            if(is_hover(mouse.x, mouse.y, j, b_limbi))
            {
                if(hover_limbi[j] == 0) /// daca nu era cursorul deja pe buton
                {
                    hover_limbi[j] = 1; /// actualizare stare
                    button_hover(j, b_limbi); /// add hover effect
                }
            }
            else /// nu mai e cursorul pe buton
            {
                if(hover_limbi[j])
                {
                    back_to_normal(j, b_limbi); /// without hover effect
                    hover_limbi[j] = 0; /// actualizare stare
                }
            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            /// button back
            if(is_hover(mouse.x, mouse.y, i, b_meniu))
            {
                AddBorder(i, b_meniu);
                delay(350);
                inapoi = 1;
            }
            else
            {
                /// the other buttons
                for(int j = 0; j <= 2; j++)
                {
                    if(is_hover(mouse.x, mouse.y, j, b_limbi))
                    {
                        AddBorder(j, b_limbi);
                        delay(350);
                        switch(j) /// change language (if another is selected)
                        {
                        case 0:
                            if(LimbaSelectata.ro == 0)
                            {
                                LimbaSelectata.ro = 1;
                                LimbaSelectata.engl = LimbaSelectata.fran = 0;
                                refresh = inapoi = 1;
                                txt_menu_ro();
                            }
                            break;
                        case 1:
                            if(LimbaSelectata.engl == 0)
                            {
                                LimbaSelectata.engl = 1;
                                LimbaSelectata.ro = LimbaSelectata.fran = 0;
                                refresh = inapoi = 1;
                                txt_menu_engl();
                            }
                            break;
                        case 2:
                            if(LimbaSelectata.fran == 0)
                            {
                                LimbaSelectata.fran = 1;
                                LimbaSelectata.engl = LimbaSelectata.ro = 0;
                                refresh = inapoi = 1;
                                txt_menu_fran();
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    while(!inapoi);
    if(refresh)
        fereastra_limbi();
}

void fereastra_meniu()
{
    culoare_chenar_meniu = COLOR(Culoare_meniu_chenar.red, Culoare_meniu_chenar.green, Culoare_meniu_chenar.blue);

    ///BACKGROUND
    setbkcolor(BLACK);
    cleardevice();

    int start_butoane;

    /// titlu + selectare text butoane
    if(LimbaSelectata.ro)
        start_butoane = meniu_ro();
    else if(LimbaSelectata.engl)
        start_butoane = meniu_engl();
    else
        start_butoane = meniu_fran();

    generare_butoane_mijloc(start_butoane, b_meniu, 4); /// desenare butoane

    bool Start = 0;
    poz mouse;
    do
    {
        mouse.x = mousex();
        mouse.y = mousey();
        for(int i = 0; i <= nr_butoane_meniu - 1; i++)
        {
            if(is_hover(mouse.x, mouse.y, i, b_meniu))
            {
                if(hover_meniu_and_back[i] == 0) /// daca nu era cursorul deja pe buton
                {
                    hover_meniu_and_back[i] = 1; /// actualizare stare
                    button_hover(i, b_meniu); /// add hover effect
                }
            }
            else /// nu mai e cursorul pe buton
            {
                if(hover_meniu_and_back[i])
                {
                    back_to_normal(i, b_meniu); /// without hover effect
                    hover_meniu_and_back[i] = 0; /// actualizare stare
                }
            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            for(int i = 0; i <= nr_butoane_meniu - 1; i++)
            {
                if(is_hover(mouse.x, mouse.y, i, b_meniu))
                {
                    AddBorder(i, b_meniu);
                    delay(350);
                    switch(i)
                    {
                    case 0:
                        windowInit();
                        titlePrint("NS Diagram Drawer");
                        settingsSection();
                        console_handler();
                        break;
                    case 1:
                        fereastra_instrctiuni();
                        break;
                    case 2:
                        fereastra_limbi();
                        break;
                    case 3:
                        closegraph();
                        return;
                        break;
                    }
                    Start = 1;
                }
            }
        }
    }
    while(!Start);
    if(hover_meniu_and_back[1] || hover_meniu_and_back[2]) /// pt informatii si limba
        fereastra_meniu();
}

int main()
{
    initwindow(window_width, window_height,"NS-diagram drawer");
    LimbaSelectata.ro = 1;
    fereastra_meniu();
    return 0;
}

