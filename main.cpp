#include <graphics.h>
#include <fstream>
#include <winbgim.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#define stil 8
#define x_initial 30
#define y_initial 10
#define distanta 10
#define thickness 4
#define MAX_TEXT 50
#define MAX_STR 256
using namespace std;
char text[MAX_TEXT], anterior[MAX_TEXT], predecesor_anterior[MAX_TEXT], copie[MAX_TEXT];
char mesaj[MAX_STR];
float ratie = 1.5;
int dim = 2;
char info_engl[] = "\tA Nassi-Shneiderman diagram in computer programming is a graphical design representation for structured programming./This type of diagram was developed in 1972 by Isaac Nassi and Ben Shneiderman. These diagrams are also called/structograms, as they show a program's structures./\tFollowing a top-down design, the problem at hand is reduced into smaller and smaller subproblems, until only simple/statements and control flow constructs remain. Nassi–Shneiderman diagrams reflect this top-down decomposition in a/straightforward way, using nested boxes to represent subproblems./\n\tDiagram types:/\t1. Process blocks: it represents the simplest of steps and requires no analysis.When a process/block is encountered, the action inside the block is performed and we move onto the next block./\t2. Branching blocks: there are two types of branching blocks. First (figure on the left) is the simple Yes-No branching/block which offers the program two paths to take depending on whether or not a condition has been fulfilled. Second/(figure on the right) is a multiple branching block. This block usually contains a select case. The block provides/the program with an array of choices./\t3. Testing loops: this block allows the program to loop one or a set of processes as long as a particular condition is/fulfilled. The process blocks covered by each loop are subset with a side-bar extending out from the condition. There are/two main types of testing loops, test first (figure on the left) and test last blocks(figure on the right). The only/difference between the two is the order in which the steps involved are completed.";
char info_ro[] ="\tO diagrama Nassi-Shneiderman este o reprezentare grafica pentru un algoritm in programarea structurata. Acest tip/de diagrama a fost dezvoltat in 1972 de Isaac Nassi si Ben Shneiderman. Aceste diagrame sunt numite si structograme,/intrucat arata structurile unui program./\tDe sus jn jos, problema in cauza este redusa la subprobleme din ce in ce mai mici, pana cand raman doar structuri/de control de baza sau structuri derivate. Diagramele Nassi–Shneiderman reflecta aceasta descompunere de sus in/jos intr-un mod direct, folosind casete imbricate pentru a reprezenta subprobleme./\n\tTipuri de diagrame:/\t1. Blocuri de proces: reprezinta cel mai simplu bloc si nu necesita o analiza amanuntita. Cand se/intalneste un bloc de proces, se realizeaza actiunea din interior si se trece la blocul urmator./\t2. Blocuri de ramificare: exista doua tipuri de blocuri de ramificare. Primul (figura din stanga) este cel de tipul/Da-Nu care ofera programului doua cai de urmat, in functie de indeplinirea sau nu a unei conditii. Al doilea (figura/din dreapta)este un bloc de ramificare multipla. Acest bloc contine de obicei o selectie de caz. Blocul ofera/programului o serie de optiuni./\t3. Blocuri de tip bucla: acest bloc permite programului sa repete un proces sau un set de procese cat timp o/anumita conditie este indeplinita. Blocurile de proces incluse intr-un bloc de tip bucla sunt marginite in partea/stanga de o bara laterala care se extinde din conditie. Exista doua tipuri principale de blocuri, cel cu test initial/(figura din stanga) si cel cu test final (figura din dreapta). Singura diferenta dintre cele doua este ordinea/in care sunt parcursi pasii.";
char info_fran[]="\tUn diagramme Nassi-Shneiderman est une representation graphique d'un algorithme en programmation structuree. Ce type/de diagramme a ete developpe en 1972 par Isaac Nassi et Ben Shneiderman. Ces diagrammes sont aussi appeles/structogrammes,car ils montrent les structures d'un programme./\tDe haut en bas, le probleme est reduit a des sous-problemes de plus en plus petits, jusqu'a ce qu'il ne reste que/des structures simples.Les diagrammes Nassi – Shneiderman refletent cette decomposition de haut en bas d'une maniere/directe, en utilisant des cassette imbriquees pour representer les sous-problemes./\n\tTypes de diagrammes:/\t1. Blocs de processus : ils sont le bloc le plus simple qui ne necessitent pas d'une analyse detaillee./Lorsqu'un bloc de processus est rencontre, l'action est effectuée et on passe au bloc suivant./\t2. Blocs de ramification : Il existe deux types de blocs de ramification. Le premier (figure a gauche) est de type/Oui-Non qui propose au programme deux voies à suivre, selon qu'une condition est remplie ou non. Le second (figure/a droite) est un bloc avec plusieurs branches. Ce bloc contient generalement une selection de cas. Le bloc offre au/programme un certain nombre d'options./\t3. Blocs de type boucle : Ce bloc permet au programme de repeter un processus ou un ensemble de processus tant/qu'une certaine condition est remplie. Les blocs de processus inclus dans un bloc de boucle sont delimites a gauche/par une ligne laterale qui s'etend de la condition. Il existe deux principaux types de blocs, celui avec test initial /(figure a gauche) et celui avec test final (figure a droite). La seule difference entre les deux est l'ordre dans/lequel les etapes sont executees.";
int x_final, y_final,
    window_width=getmaxwidth(),
    window_height=getmaxheight(),
    secWidth, secHeight;
int LMAX = window_width / 2 - 50; /// lungimea este adaptata la dimensiunea ferestrei
enum instructiune_speciala {CLASIC, CONDITIE, ADV, FALS, LOOP_FIRST, IN_LOOP, LOOP_LAST, SWITCH_CASE, CAZ, ROOT, IGNORA_DA, IGNORA_NU, IGNORA_LOOP_LAST, IN_CAZ};
instructiune_speciala cod;
int culoare_chenar_meniu;
struct nod
{
    char linie_text[MAX_TEXT];
    int x, y;
    int lungime;
    instructiune_speciala tip;
    int nr_fii = 0;
    int nr_taburi = -1;
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
    int red = 79;
    int green = 52;
    int blue = 102;
} CuloareSwitch;

struct rgb_case
{
    int red = 147;
    int green = 56;
    int blue = 95;
} CuloareCase;

struct rgb_in_case
{
    int red = 159;
    int green = 107;
    int blue = 153;
} CuloareInCase;

struct rgb_loop
{
    int red = 25;
    int green = 81;
    int blue = 104;
} CuloareLoop;

struct rgb_in_loop
{
    int red = 162;
    int green = 162;
    int blue = 161;
} CuloareInLoop;

struct rgb_conditie
{
    int red = 255;
    int green = 166;
    int blue = 0;
} CuloareConditie;

struct rgb_NU
{
    int red = 255;
    int green = 23;
    int blue = 0;
} CuloareNU;

struct rgb_DA
{
    int red = 77;
    int green = 105;
    int blue = 16;
} CuloareDA;

struct rgb_FALS
{
    int red = 245;
    int green = 82;
    int blue = 63;
} CuloareFALS;

struct rgb_ADV
{
    int red = 88;
    int green = 202;
    int blue = 99;
} CuloareADV;

struct rgb_clasic
{
    int red = 240;
    int green = 173;
    int blue = 120;
} CuloareClasic;

///Universal position struct
struct poz
{
    int x,y;
} console,secPoz;

///Buttons List
struct btn
{
    poz coord;
    char txt[31];
    int width, height, event;
} b[20];

char bttns[20][31];

struct limba
{
    bool ro;
    bool engl;
    bool fran;
} LimbaSelectata;


void clearVP(int x, int y, int width, int height)
{
    setbkcolor(BLACK);
    ///SELECT ViewPort
    setviewport(x, y, x+width, y+height, 1);
    clearviewport();
    ///BACK TO ALL SCREEN
    setviewport(0, 0, window_width, window_height, 0);
}
void printPasteInConsole()
{
    char line[MAX_STR];
    FILE *pst;
    pst = fopen("program.ns", "r");
    if(pst==NULL)
        return;
    poz output;
    output.x=console.x+10;
    output.y=console.y+5;
    settextstyle(10,0,1);
    while(fgets(line,256,pst))
    {
        int i=0;
        while(line[i]==9){
            output.x+=textwidth("    ");
            i++;
        }
        outtextxy(output.x, output.y, line);
        output.x=console.x+10;
        output.y+=18;
    }
}
void printConsoleMessage(char text[])
{
    settextstyle(10,0,3);
    clearVP(console.x+1, console.y-30, secWidth-1, 29);
    outtextxy(console.x+10,console.y-30,text);
    settextstyle(10,0,1);
}
void getLanguageString(char txt[], int code)
{
    if(LimbaSelectata.ro)
    {
        switch (code)
        {
        case 0:
            strcpy(txt,"Nou");
            break;
        case 1:
            strcpy(txt,"Genereaza");
            break;
        case 2:
            strcpy(txt,"Salveaza");
            break;
        case 3:
            strcpy(txt,"Inapoi");
            break;
        case 4:
            strcpy(txt,"Introduceti programul:");
            break;
        case 5:
            strcpy(txt,"Textul a fost lipit cu succes!");
            break;
        case 6:
            strcpy(txt,"Finalizat. Puteti genera diagrama.");
            break;
        case 7:
            strcpy(txt,"Se salveaza diagrama...");
            break;
        case 8:
            strcpy(txt,"Diagrama salvata cu succes.");
            break;
        case 9:
            strcpy(txt,"Nu exista nicio diagrama!");
            break;
        case 10:
            strcpy(txt,"Generare cu succes!");
            break;
        case 11:
            strcpy(txt,"Desenator diagrama N-S");
            break;
        }
    }
    else if (LimbaSelectata.engl)
    {
        switch (code)
        {
        case 0:
            strcpy(txt,"New");
            break;
        case 1:
            strcpy(txt,"Generate");
            break;
        case 2:
            strcpy(txt,"Save");
            break;
        case 3:
            strcpy(txt,"Back");
            break;
        case 4:
            strcpy(txt,"Write the algorithm:");
            break;
        case 5:
            strcpy(txt,"Paste successful!");
            break;
        case 6:
            strcpy(txt,"Done. You can now generate the diagram.");
            break;
        case 7:
            strcpy(txt,"Saving...");
            break;
        case 8:
            strcpy(txt,"Save successful.");
            break;
        case 9:
            strcpy(txt,"There is no diagram to save!");
            break;
        case 10:
            strcpy(txt,"Diagram generated.");
            break;
        case 11:
            strcpy(txt,"N-S Diagram Drawer");
            break;
        }
    }
    else if (LimbaSelectata.fran)
    {
        switch(code)
        {
        case 0:
            strcpy(txt,"Neuf");
            break;
        case 1:
            strcpy(txt,"Generer");
            break;
        case 2:
            strcpy(txt,"Sauver");
            break;
        case 3:
            strcpy(txt,"Retour");
            break;
        case 4:
            strcpy(txt,"Ecrire l'algorithme:");
            break;
        case 5:
            strcpy(txt,"Collage reussi!");
            break;
        case 6:
            strcpy(txt,"Fait. Vous pouvez maintenant generer le diagramme.");
            break;
        case 7:
            strcpy(txt,"Sauver...");
            break;
        case 8:
            strcpy(txt,"Sauver succès.");
            break;
        case 9:
            strcpy(txt,"Il n'y a pas de schema a sauve!");
            break;
        case 10:
            strcpy(txt,"Diagramme genere.");
            break;
        case 11:
            strcpy(txt,"Dessinateur Diagramme N-S");
            break;
        }
    }
}


void color_middle_top(int x_stanga, int y_sus, int Lsus, int colour)
{
    setfillstyle(SOLID_FILL, colour);
    floodfill(x_stanga + Lsus/2, y_sus + 2, WHITE);
    setbkcolor(colour);
}

void color_switch(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareSwitch.red, CuloareSwitch.green, CuloareSwitch.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_case(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareCase.red, CuloareCase.green, CuloareCase.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_in_case(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareInCase.red, CuloareInCase.green, CuloareInCase.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_loop(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareLoop.red, CuloareLoop.green, CuloareLoop.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_in_loop(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareInLoop.red, CuloareInLoop.green, CuloareInLoop.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);;
}

void color_conditie(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareConditie.red, CuloareConditie.green, CuloareConditie.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_DA(int x_stanga, int y_sus)
{
    int colour = COLOR(CuloareDA.red, CuloareDA.green, CuloareDA.blue);
    setfillstyle(SOLID_FILL, colour);
    floodfill(x_stanga + 1, y_sus + 5, WHITE);
    setbkcolor(colour);
}

void color_NU(int x_dreapta, int y_sus)
{
    int colour = COLOR(CuloareNU.red, CuloareNU.green, CuloareNU.blue);
    setfillstyle(SOLID_FILL, colour);
    floodfill(x_dreapta - 1, y_sus + 5, WHITE);
    setbkcolor(colour);
}

void color_clasic(int x_stanga, int y_sus, int Lsus)
{
    int colour =  COLOR(CuloareClasic.red, CuloareClasic.green, CuloareClasic.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_adv(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareADV.red, CuloareADV.green, CuloareADV.blue);
    color_middle_top(x_stanga, y_sus, Lsus, colour);
}

void color_fals(int x_stanga, int y_sus, int Lsus)
{
    int colour = COLOR(CuloareFALS.red, CuloareFALS.green, CuloareFALS.blue);
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
    p->nr_taburi = i;

    /// FIRST NODE
    if(anterior == NULL)
    {
        /// ADD ROOT
        nod *q = new nod;
        strcpy(q->linie_text, "---");
        q->nr_taburi = -1;
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
            strcpy(until->linie_text, "repeat");

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
        q->nr_taburi = -1;

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
        while(!(anterior->tip == SWITCH_CASE && i == anterior->nr_taburi))
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
        q->nr_taburi = -1;
        q->tip = IGNORA_NU;

        /// GO TO CONDITIE:
        while(anterior->nr_taburi != i - 1)
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
        strcpy(q->linie_text, "repeat");
        q->tip = IGNORA_LOOP_LAST;

        /// LINK "until" TO CHILDREN
        q->nr_fii = 1;
        q->urm[1] = p;
        p->parinte = q;

        if(!strstr(predecesor_anterior, "else"))
        {
            /// START FROM ROOT
            nod *start = radacina->urm[radacina->nr_fii];

            /// GO DOWN TO SIBLING
            while(i - 1 != start->nr_taburi)
                start = start->urm[start->nr_fii];

            /// GO TO PARENT
            start = start->parinte;

            /// LINK "until" TO PARENT
            start->nr_fii++;
            start->urm[start->nr_fii] = q;
            q->parinte = start;

            p->y = get_y_from_previous(q->parinte->urm[q->parinte->nr_fii - 1]);
        }
        else /// else...repreat...(instuctiune)
        {
            /// CREATE NODE "NU"
            nod *nu = new nod;
            strcpy(nu->linie_text, "NU");
            nu->nr_taburi = -1;
            nu->tip = IGNORA_NU;

            /// GO TO CONDITIE
            while(anterior->nr_taburi != i - 2) /// -8 pt ca e intr-un repreat until dintr-un else
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
    else if(anterior->nr_taburi == i)
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
    else if(anterior->tip == LOOP_FIRST && (i - anterior->nr_taburi) == 1)
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
    else if(i < anterior->nr_taburi && cod != FALS)
    {
        /// START FROM ROOT
        nod *start = radacina->urm[radacina->nr_fii];

        /// GO DOWN TO SIBLING
        while(i != start->nr_taburi)
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

            case CAZ:
                p->tip = IN_CAZ;
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
    else if(i < anterior->nr_taburi && cod == FALS)
    {
        /// START FROM ROOT
        nod *start = radacina->urm[radacina->nr_fii];

        /// GO DOWN TO PARENT
        while(i-1 != start->nr_taburi)
            start = start->urm[start->nr_fii];

        /// CREATE NODE "NU"
        nod *q = new nod;
        strcpy(q->linie_text, "NU");
        q->nr_taburi = -1;
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
        p->y = anterior->y + ratie * textheight(anterior->linie_text);

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
    line(x_stanga, y_stanga, x_stanga, y_mijloc);
    line(x_stanga, y_mijloc, x_mijloc, y_mijloc);
    line(x_stanga, y_stanga, x_mijloc, y_mijloc);
}

void DrawTriangle_false(int x_dreapta, int y_sus, int x_mijloc, int y_mijloc)
{
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

bool point_on_the_right_side(int x1, int y1, int x2, int y2, int x, int y)
{
    return (((x - x1)*(y2 - y1) - (y - y1)*(x2 - x1)) > 0);
}

bool width_too_long(char text[], int Lsus)
{
    return (textwidth(text) >= Lsus);
}

void shrink_txt_conditie(char text[], int x1, int y1, int x2, int y2, int x, int y, int Lsus)
{
    int marime = dim;
    while(!point_on_the_right_side(x1, y1, x2, y2, x - 1, y - 1) && marime > 1)
    {
        marime--;
        settextstyle(stil, 0, marime);
    }

    x = x1 + (Lsus - textwidth(text))/2;
    y = y1 + textheight(text);
    if(!point_on_the_right_side(x1, y1, x2, y2, x - 1, y - 1)) /// daca nu merge cu fontul actual
        settextstyle(11, 0, 1);

    x = x1 + (Lsus - textwidth(text))/2;
    y = y1 + textheight(text);
    if(!point_on_the_right_side(x1, y1, x2, y2, x - 1, y - 1)) /// daca nici asa nu a mers
    {
        marime = 5;
        while(!point_on_the_right_side(x1, y1, x2, y2, x - 1, y - 1) && marime > 1)
        {
            marime--;
            settextstyle(2, 0, marime);
            x = x1 + (Lsus - textwidth(text))/2;
            y = y1 + textheight(text);
        }
    }
}

void shrink_txt_yes(char text[], int x1, int y1, int x2, int y2, int x, int y)
{
    int marime = dim;
    while(point_on_the_right_side(x1, y1, x2, y2, x, y) && marime > 1)
    {
        marime--;
        settextstyle(stil, 0, marime);
    }

    x = x1 + textwidth(text);
    y = y2 - textheight(text) - 1;
    if(point_on_the_right_side(x1, y1, x2, y2, x, y)) /// daca nu merge cu fontul actual
        settextstyle(11, 0, 1);

    x = x1 + textwidth(text);
    y = y2 - textheight(text) - 1;
    if(point_on_the_right_side(x1, y1, x2, y2, x, y)) /// daca nici asa nu a mers
    {
        marime = 5;
        while(point_on_the_right_side(x1, y1, x2, y2, x, y) && marime > 1)
        {
            marime--;
            settextstyle(2, 0, marime);
            x = x1 + textwidth(text);
            y = y2 - textheight(text) - 1;
        }
    }
}

void shrink_txt_rectangle(char text[], int Lsus)
{
    int marime = dim;
    while(width_too_long(text, Lsus) && marime > 1)
    {
        marime--;
        settextstyle(stil, 0, marime);
    }

    if(width_too_long(text, Lsus)) /// daca nu merge cu fontul actual
        settextstyle(11, 0, 1);

    if(width_too_long(text, Lsus)) /// daca nici asa nu a mers
    {
        marime = 5;
        while(width_too_long(text, Lsus) && marime > 1)
        {
            marime--;
            settextstyle(2, 0, marime);
        }
    }
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
    settextstyle(stil, 0, dim);
    char text[3];
    strcpy(text, "DA");
    int latime = textheight(text);
    int Lungime = textwidth(text);
    ///coltul dreapta sus al textului
    int x_text = x_stanga + Lungime;
    int y_text = y_mijloc - latime - 1;

    if(point_on_the_right_side(x_stanga, y_stanga, x_mijloc, y_mijloc, x_text, y_text)) /// depasesc linia oblica
        shrink_txt_yes(text, x_stanga, y_stanga, x_mijloc, y_mijloc, x_text, y_text);

    color_DA(x_stanga, y_stanga);
    ///dupa shrink se schimba
    latime = textheight(text);
    Lungime = textwidth(text);
    outtextxy(x_stanga + 2, y_mijloc - latime - 1, text);
}

void DrawConditie_Text(char text[], int x, int y, int Lsus)
{
    int Lungime = textwidth(text);
    int latime = textheight(text);

    color_conditie(x, y, Lsus);

    int x_mijloc = x + Lsus/2;
    int y_mijloc = y + 3*latime;

    if(!point_on_the_right_side(x, y, x_mijloc, y_mijloc, x + (Lsus - Lungime)/2 - 1, y + latime - 1))
            shrink_txt_conditie(text, x, y, x_mijloc, y_mijloc, x + (Lsus - Lungime)/2 - 1, y + latime - 1, Lsus);
    Lungime = textwidth(text);
    outtextxy(x + (Lsus - Lungime)/2, y + 2, text);

    DrawTriangle_true_text(x, y, x_mijloc, y_mijloc);
    DrawTriangle_false_text(x+Lsus, y, x_mijloc, y_mijloc);
}

void DrawRectangle_text(char text[], int x_stanga, int y_sus, int Lsus) /// Lsus = Lungimea dreptunghiului desenat
{
    int Lungime = textwidth(text);
    if(Lungime >= Lsus)
    {
        shrink_txt_rectangle(text, Lsus);
        Lungime = textwidth(text);
    }
    int latime = textheight(text)*ratie; /// dupa shrink, sa fie dimensiunea actuala

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
                r->lungime =r->parinte->parinte->lungime - r->parinte->parinte->urm[1]->urm[1]->lungime;
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

nod* get_main_ramification(nod *start)
{
    nod *first_ramification;
    bool okay = 1;
    while(start->tip != ROOT && okay)
    {
        if(start->tip == CONDITIE)
        {
            first_ramification = start;
            /// verific daca pot lungi in jos chenarul (dedesubt e posibil un spatiu gol)
            if(start->parinte->urm[start->parinte->nr_fii] != start) /// dupa conditie se mai executa o instructiune care are acelasi parinte
                okay = 0;
        }
        else if(start->tip == CAZ)
        {
            first_ramification = start->parinte; /// swithc
            if(first_ramification->parinte->urm[first_ramification->parinte->nr_fii] != first_ramification) /// dupa switch(...) se mai executa o instructiune care are acelasi parinte
                okay = 0;
        }
        start = start->parinte;
    }
    return first_ramification;
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
                {
                    nod *first_ramification = get_main_ramification(r);

                    /// chenar pt true
                    chenar(r->x, r->urm[1]->urm[1]->y, r->x + r->lungime/2, get_y_from_previous(first_ramification));

                    /// chenar pt false
                    chenar(r->x + r->lungime/2, r->urm[1]->urm[1]->y, r->x + r->lungime, get_y_from_previous(first_ramification));
                }
                break;
            case CAZ:

                DrawRectangle(r->linie_text, r->x, r->y, r->lungime);

                {
                    nod *first_ramification = get_main_ramification(r);

                    /// chenar
                    chenar(r->x, r->y, r->x + r->lungime, get_y_from_previous(first_ramification));
                }
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
            settextstyle(stil, 0, dim);
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
        while(text[i] == '\t')
            i++;

        /// SIMPLIFY
        cod = simplify(text, i);

        /// TYPE
        if(strstr(anterior, "else") && cod == CLASIC)
            cod = FALS;
        else if(strstr(anterior, "repeat") && cod == CLASIC)
            cod = IN_LOOP;
        else if(inainte != NULL && i > inainte->nr_taburi && cod == CLASIC) /// SIGUR NU E CLASIC
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

    settextstyle(stil, HORIZ_DIR, dim);
    procesare("program.ns");
    if(get_y_from_previous(radacina) > window_height)
    {
        dim--;
        delete_tree(radacina);
        settextstyle(stil, HORIZ_DIR, dim);
        procesare("program.ns");
    }
    set_x_and_lungime(radacina, 1);
    desen(radacina);
    Add_Color_and_Text(radacina);
    getLanguageString(mesaj,10);
    printConsoleMessage(mesaj);
}

void titlePrint(char title[])
{
    settextstyle(10,0,7); ///Text Style
    int txt_height = textheight(title), ///Title Height
        txt_width = textwidth(title);       ///Title Width

    /// Adjusting font size for smaller screens/window
    if(window_width/2 < txt_width)
    {
        settextstyle(10,0,5);
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


bool isHover(poz mouse, int i, btn b[])  ///hover check for buttons
{
    if (mouse.x>=b[i].coord.x && mouse.x<=b[i].coord.x + b[i].width && mouse.y>=b[i].coord.y && mouse.y<=b[i].coord.y + b[i].height)
        return true;
    return false;
}
bool onlyTabs(char sir[]){
    int i=0;
    while(sir[i]!=NULL){
        if(sir[i]!=9) return 0;
        i++;
    }
    return 1;
}
void spacesToTabs(char sir[]){
    int i,k;
    i=k=0;
    while(sir[i]==9 || sir[i]==' '){
        if(sir[i]==' ') k++;
        if(k%4==0 && k){
            strcpy(sir+i-3, sir+i);
            i-=3;
            sir[i-1]=9;
        }
        i++;
    }
}
void drawFrame(poz frmPoz, int frmWidth, int frmHeight)
{
    rectangle(frmPoz.x, frmPoz.y, frmPoz.x + frmWidth, frmPoz.y + frmHeight);
}
void drawConsole()  ///drawing the console
{
    console.x=secPoz.x;
    console.y=secPoz.y + secHeight/5;
    clearVP(console.x+1, console.y-31, secWidth-1, secHeight); ///clearing viewport of previous console
    rectangle(console.x, console.y, console.x + secWidth, secPoz.y+secHeight);
    line(console.x, console.y-35, console.x+secWidth, console.y-35);
}
void scriere_fisier()
{
    FILE *prgmNS;
    prgmNS = fopen("program.ns", "w");

    char S[MAX_STR]; ///ce se citeste
    poz output;


    char Enter = 13;
    char BackSpace = 8;
    char Escape = 27;
    char s2[2];
    s2[1]='\0';
    char tab = 9;
    char MultimeDeCaractereAcceptabile[MAX_STR]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!`@#$%+-^&*/\\()_=[],;.?<>:;{} ";
    char S_initial[MAX_STR];
    char tasta=0;
    char S_[MAX_STR];
    drawConsole();
    setbkcolor(BLACK);
    settextstyle(10,0,3);
    strcpy(S_initial,S);
    setcolor(WHITE);
    output.x=console.x+10;
    output.y=console.y-30;

    getLanguageString(mesaj, 4); ///new generation text above console
    printConsoleMessage(mesaj);

    output.y+=35;   ///moving output below console line and ready for first print
    settextstyle(10,0,1);
    strcpy(S,"");
    strcpy(S_,S);
    strcat(S_,"_");
    setcolor(BLACK);
    outtextxy(output.x,output.y,S_);
    s2[0]=tasta;
    strcat(S,s2);
    strcpy(S_,S);
    strcat(S_,"_");
    setcolor(WHITE);
    outtextxy(output.x,output.y,S_);

    bool paste = false; ///bool for the ctrl+v event

    do
    {
        tasta=getch();
        if (tasta==0)
        {
            tasta=getch();
        }
        else if (strchr(MultimeDeCaractereAcceptabile, tasta))  ///event for an accepted/printable key
        {
            strcpy(S_,S);
            strcat(S_,"_");
            setcolor(BLACK);
            outtextxy(output.x,output.y,S_);
            s2[0]=tasta;
            strcat(S,s2);
            strcpy(S_,S);
            strcat(S_,"_");
            setcolor(WHITE);
            outtextxy(output.x,output.y,S_);
        }
        if (tasta==BackSpace) ///backspace
            if (!(strcmp(S,"")) && output.y>10+console.y); ///deleting at the start of a line - in progress
            else if (output.x>console.x+11 && !(strcmp(S,"	")))   ///deleting tabs
            {
                setcolor(BLACK);
                outtextxy(output.x,output.y,S_);
                output.x-=textwidth("    ");    ///delete indent if string has only tabs
                setcolor(WHITE);
                outtextxy(output.x,output.y,S_);
            }
            else    ///deleting characters
            {
                setcolor(BLACK);
                outtextxy(output.x,output.y,S_);
                S[strlen(S)-1]='\0';
                strcpy(S_,S);
                strcat(S_,"_");
                setcolor(WHITE);
                outtextxy(output.x,output.y,S_);
            }
        else if (tasta==9) ///tab
        {
            setcolor(BLACK);
            outtextxy(output.x,output.y,S_);
            if(onlyTabs(S))
                output.x+=textwidth("    "); ///add indent if tab is at the beginning
            else
                strcat(S,"    ");
            s2[0]=tab;
            strcat(S,s2);
            strcpy(S_,S);
            strcat(S_,"_");

            setcolor(WHITE);
            outtextxy(output.x,output.y,S_);
        }
        else if (tasta==Enter && GetKeyState(VK_SHIFT) & 0x8000 || tasta==10) ///shift+enter
        {
            setcolor(BLACK);
            outtextxy(output.x,output.y,S_);
            setcolor(WHITE);
            outtextxy(output.x,output.y,S);
            output.x=console.x+10;
            output.y+=18;
            spacesToTabs(S);
            fputs(S, prgmNS);   ///writing current line
            putc('\n', prgmNS); ///new line in file
            strcat(S,"\n");     ///deleting current string and preparing for next line read
            strcpy(S,"\0");
            strcpy(S_,S);
            strcat(S_,"_");
            outtextxy(output.x,output.y,S_);
            tasta=BackSpace;    ///so we dont get out of while loop
        }
        else if(GetKeyState(VK_CONTROL)&0x8000) ///ctrl+v
        {
            OpenClipboard(NULL);
            char* buffer;
            buffer = (char*)GetClipboardData(CF_TEXT);
            CloseClipboard();

            int i=0,k=0;
            while(buffer[i]!=NULL)
            {
                if(buffer[i]==10){
                    putc(10, prgmNS);
                    i++;
                    while(buffer[i]==' '){
                        k++;
                        i++;
                    }
                    if(k && k%4==0) k/=4;
                    for(;k;k--) putc(tab, prgmNS);
                }
                if(buffer[i]!=13)
                    putc(buffer[i], prgmNS);  ///removing CR (character 13)
                i++;
            }
            tasta=Enter;    ///so we jump out of while loop

            ///Print success message
            drawConsole();
            paste = true;
            getLanguageString(mesaj,5); ///paste message
            printConsoleMessage(mesaj);
        }

    }
    while (tasta!=Escape && tasta!=Enter);
    if (tasta == Escape)
        strcpy(S,S_initial);   ///delete last line and end reading
    if (tasta == Enter && !paste)   ///if there wasn't a ctr+v, display a good to go message
    {
        getLanguageString(mesaj,6);
        printConsoleMessage(mesaj);
    }
    setcolor(BLACK);
    outtextxy(output.x,output.y,S_);
    setcolor(WHITE);
    outtextxy(output.x,output.y,S);
    strcat(S," \0");
    fputs(S, prgmNS);
    fclose(prgmNS);
    if(paste)
        printPasteInConsole();   ///pasting ctr+v program in console
}

void drawButton(poz btnPoz, int btnWidth, int btnHeight, char btnText[])
{
    settextstyle(10,0,3);
    poz txtCentered;
    txtCentered = getCenteredTextXY(btnWidth, btnHeight, btnText); ///Centering the text within the button
    setcolor(WHITE);
    rectangle(btnPoz.x, btnPoz.y, btnPoz.x + btnWidth, btnPoz.y+btnHeight);
    setfillstyle(SOLID_FILL,culoare_chenar_meniu);
    setbkcolor(culoare_chenar_meniu);
    floodfill(btnPoz.x+1, btnPoz.y+1,15);
    outtextxy(btnPoz.x + txtCentered.x, btnPoz.y + txtCentered.y, btnText);
}
void clearButton(poz btnPoz, int btnWidth, int btnHeight, char btnText[])   ///clearing the button's content
{
    clearVP(btnPoz.x+1, btnPoz.y+1, btnWidth-2, btnHeight-2);
}
void generate_buttons (btn b[], poz ctr)   ///generating buttons for the settings area
{
    char btntext[MAX_STR];
    int i=0,
        freespace=(secWidth-4*140-2)/8; ///calculating freespace between buttons
    while(i<4)
    {
        b[i].width = 140;
        b[i].height = 50;
        b[i].coord.x = ctr.x + (2*i+1)*freespace + i*b[0].width;    ///buttons automatically centered and adjusted
        b[i].coord.y = ctr.y+ (secHeight/5-b[i].height-35)/2; ///centering vertically

        ///get text for the button based on language
        getLanguageString(btntext,i);
        strcpy(b[i].txt, btntext);

        drawButton(b[i].coord, b[i].width, b[i].height, b[i].txt);
        i++;
    }
}
void settingsSection()   ///the whole right half of the screen
{
    getLanguageString(mesaj,11);
    titlePrint(mesaj);
    secPoz.x = 5 * (window_width / 2) / 100 + window_width/2;    ///Starts 5% indented relative to the right half width
    secPoz.y = 100;                                  ///Starts 100px down from the top
    secWidth = 90 * (window_width / 2) / 100;   /// 90% of the left half's width (centered horizontally)
    secHeight = 85 * window_height / 100;       /// 85% of screen's height

    ///Section's frame
    drawFrame(secPoz, secWidth, secHeight);

    ///Buttons
    generate_buttons(b,secPoz);

    drawConsole();
}

void saveImage()    ///saving image of diagram
{
    if(get_y_from_previous(radacina)-y_initial<20)
    {
        getLanguageString(mesaj,9);
        printConsoleMessage(mesaj);
        return;
    }
    ///Getting local time for image name
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"NS%H%M%S.png",timeinfo); ///giving the image a name based on current time
    char* str(buffer);
    ///saving the image
    writeimagefile(str, x_initial, y_initial, x_initial + LMAX, get_y_from_previous(radacina));
    ///Success message
    setbkcolor(BLACK);
    getLanguageString(mesaj,7);
    printConsoleMessage(mesaj);
    delay(1000);    ///a delay so it looks like it takes time to actually save :D
    getLanguageString(mesaj,8);
    printConsoleMessage(mesaj);
}


void buttonHover(int i, btn b[])    ///event for hovering on button
{
    clearButton(b[i].coord, b[i].width, b[i].height, b[i].txt);
    int culoare= COLOR(Culoare_hover_meniu.red, Culoare_hover_meniu.green, Culoare_hover_meniu.blue);
    setfillstyle(SOLID_FILL,culoare);
    floodfill(b[i].coord.x+1, b[i].coord.y+1,15);
    setbkcolor(culoare);
    setcolor(culoare_chenar_meniu);
    poz txtCentered = getCenteredTextXY(b[i].width, b[i].height, b[i].txt); ///Centering the text within the button
    outtextxy(b[i].coord.x + txtCentered.x, b[i].coord.y + txtCentered.y, b[i].txt);
    setcolor(WHITE);
}

void buttonNormal(int i, btn b[])   ///event for returning button to normal state
{
    clearButton(b[i].coord, b[i].width, b[i].height, b[i].txt);
    drawButton(b[i].coord, b[i].width, b[i].height, b[i].txt);
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
    if(window_width - 10 < start.x + 3 * textwidth(text_exemplu))
        capat.x = window_width - 10;
    else
        capat.x = start.x + 3 * textwidth(text_exemplu);
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
            generate_button_back();
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
            settextstyle(10, 0, marime);
            Draw_switch_case_exemplu(start.y, caz, path); /// blocul switch case
            setbkcolor(BLACK);
            start.y += 3 * textheight(caz) * ratie + 5; /// adaugam dimensiunea blocurilor + 5
            settextstyle(10, 0, marime);
        }
        if(strstr(p, "Diagram types:") || strstr(p, "Types de diagrammes:") || strstr(p, "Tipuri de diagrame:"))
        {
            if(LimbaSelectata.engl)
                diagram_types("Diagram types:", marime, start);
            else if(LimbaSelectata.ro)
                diagram_types("Tipuri de diagrame:", marime, start);
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
            settextstyle(10, 0, marime);
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

void afisare_info_general()
{
    if(LimbaSelectata.engl)
        afisare_info(6, info_engl);
    else if(LimbaSelectata.ro)
        afisare_info(6, info_ro);
    else
        afisare_info(6, info_fran);
}

void fereastra_info()
{
    setbkcolor(BLACK);
    cleardevice();
    generate_button_back();
    afisare_info_general();
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
void windowInit()
{
    ///initwindow(1400, 790);
    ///initwindow(window_width, window_height,"NS-diagram drawer");
    setbkcolor(BLACK);
    cleardevice();
    delay(200);
    line(window_width/2, 0, window_width/2, window_height); ///vertical separation line
    settingsSection(); ///right half of the screen
}
bool hover_check[20];
void event_handler()   ///general event handling
{
    bool inapoi=false;
    do
    {
        poz mouse;
        mouse.x=mousex();
        mouse.y=mousey();
        for(int i = 0; i <= 4 - 1; i++) ///hover animation
        {
            if(isHover(mouse, i, b))
            {
                if(hover_check[i] == 0) /// daca nu era cursorul deja pe buton
                {
                    hover_check[i] = 1; /// actualizare stare
                    buttonHover(i, b); /// add hover effect
                }
            }
            else /// nu mai e cursorul pe buton
            {
                if(hover_check[i])
                {
                    hover_check[i] = 0; /// actualizare stare
                    buttonNormal(i, b); /// without hover effect
                }
            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))   ///click handling
        {
            clearmouseclick(WM_LBUTTONDOWN);
            for(int i=0; i<=3; i++)
            {
                if(isHover(mouse, i, b))
                {
                    switch(i)
                    {
                    case 0:                     ///Console opens for a new program
                        scriere_fisier();
                        break;
                    case 1:                     ///Diagram generates
                        diagram_generation();
                        break;
                    case 2:                     ///Save an image of the diagram
                        saveImage();
                        break;
                    case 3:                     ///Back button
                        inapoi=true;
                    }
                }
            }
        }
        while(kbhit())
            getch(); ///clear keyboard buffer to ignore characters pressed randomly
    }
    while(!inapoi);
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
                        event_handler();
                        break;
                    case 1:
                        fereastra_info();
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
    if(!hover_meniu_and_back[3])
        fereastra_meniu();
}

int main()
{
    initwindow(window_width, window_height,"NS-diagram drawer");
    LimbaSelectata.engl = 1;
    fereastra_meniu();
    return 0;
}
