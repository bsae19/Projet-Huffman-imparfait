#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*je n'ai pas pu implementer le retour a la ligne pour le fichier a coder en huffman car j'ai le covid depuis plusieur jour cordialement sa e silva bruno 19006948*/


typedef struct noeud{
    int id;
    char val;
    struct noeud* g;
    struct noeud* d;
}Noeud;
typedef Noeud* Arbre;

Arbre construire_arbre(char* tab,int taille,char* carac){
    Arbre n = NULL;
    if (taille > 0) {
        n = malloc(sizeof(Arbre));
        int indice = taille / 2;
        n -> val = tab[indice];
        for(int i=0;i<strlen(carac);i++){
            if (carac[i]==tab[indice]){
                n -> id = i;}}
        n -> g  = construire_arbre(tab, indice,carac);
        n -> d = construire_arbre(tab + indice + 1, taille - indice - 1,carac);}
    return n;  
}
void search_arbre(Arbre a,int ind,char* code){
    if (a->id==ind){
        strcat(code,"\0");}
    if (a->id>ind){
        strcat(code,"0");
        search_arbre(a->g,ind,code);}
    if (a->id<ind){
        strcat(code,"1");
        search_arbre(a->d,ind,code);}
}
void search_dans_arbre(Arbre a,char* co,char *d){
    if (strlen(co)==0){
        *d=a->val;
        return;}
    if (co[0]=='0'){
        search_dans_arbre(a->g,co+1,d);}
    if (co[0]=='1'){
        search_dans_arbre(a->d,co+1,d);}
}

void huff_compresse(FILE* fichier,char *st){
    FILE* f = NULL;
    int bol=1;
    int tl=strlen(st);
    int count=strlen(st);
    while (count>0){
        if (bol==0){
            break;
        }
        if (st[count-1]=='.'){
            bol=0;}
        count-=1;
    }
    char s[tl+5];
    for(int i=0;i<count;i++){
        s[i]=st[i];
    }
    s[count]='\0';
    strcat(s,".huff\0");
    f= fopen(s, "w+");
    if (f == NULL){
        printf("Impossible d'ouvrir le fichier.");
        return;}
    else{
        char carac[50]="";
        char cToStr[2];
        int c=0;
        int check,ln;
        do{
            c= fgetc(fichier);
            check=0;
            ln=strlen(carac);
            for(int i=0;i<ln;i++){
                if (carac[i]==c){
                    check=1;
                }
            }
            if(check==0){
                cToStr[1] = '\0';
                cToStr[0] = c;
                strcat(carac,cToStr);
            }
        }while(c!= EOF);
        ln=strlen(carac);
        carac[ln-1]='\0';
        int taille=strlen(carac);
        Arbre a= construire_arbre(carac,taille,carac);
        fclose(fichier);
        fichier = fopen(st, "r");
        char compt[255]="";
        int bal=0;
        int tal=strlen(st);
        int car=0;
        int t=0;
        while (car<tal){
        if (bal==1){
            compt[t]=st[car];
            t+=1;
        }
        if (st[car+1]=='.'){
            bal=1;}
        car+=1;
    }
    compt[car]='\0';
        fprintf(f, "%s\n", carac);
        fprintf(f, "%s\n", compt);
    do
        {
            int i=0;
            c= fgetc(fichier);
            if(c!=EOF){
            char code[255]="";
            code[0]='0';
            int count=1;
            i=strlen(carac)/2;
            int r=i;
            int ind=0;
                for(int i=0;i<strlen(carac);i++){
                    if (carac[i]==c){ind=i;}
                }
            search_arbre(a,ind,code);
            fprintf(f,"%s ",code);}
        } while (c!= EOF);
    }
    fclose(f);
    return;
}
void huff_decompresse(FILE* fichier,char *st){
    FILE* f = NULL;
    int bol=1;
    int tl=strlen(st);
    int count=strlen(st);
    while (count>0){
        if (bol==0){
            break;}
        if (st[count-1]=='.'){
            bol=0;}
        count-=1;}
    char s[tl+5];
    for(int i=0;i<count;i++){
        s[i]=st[i];}
    s[count]='\0';
    char carac[50]="";
    char form[50]="";
    fscanf(fichier,"%[^\n]",carac);
    fscanf(fichier,"%s\n",&form);
    strcat(s,form);
    f= fopen(s, "w+");
    if (f == NULL){
        printf("Impossible d'ouvrir le fichier.");
        return;}
    else{
        int taille=strlen(carac);
        Arbre a= construire_arbre(carac,taille,carac);
        char co[255]="";
        while (fscanf(fichier,"%s ",&co)!= EOF){
            char d=' ';
            char cod[255]="";
            for(int i=1;i<strlen(co);i++){
                cod[i-1]=co[i];}
            search_dans_arbre(a,cod,&d);
            fprintf(f,"%c",d);}}
    fclose(f);
    return;
}


int main(int argc, char *argv[]){
    if( argc == 2) {
        FILE* fichier = NULL;
        fichier = fopen(argv[1], "r");
        if (fichier == NULL){
            printf("Impossible d'ouvrir le fichier.");
            return 0;}
        else{
            huff_compresse(fichier,argv[1]);}
        fclose(fichier);
        return 1;}
    if( argc == 3){
        if(strcmp(argv[1], "-d") != 0){
            printf("error in entry args.\n");
            return 0;}
        else{
            char compt[255]="";
            int bal=0;
            int tal=strlen(argv[2]);
            char* st=argv[2];
            int car=0;
            int t=0;
            while (car<tal){
                if (bal==1){
                    compt[t]=st[car];
                    t+=1;}
                if (st[car+1]=='.'){
                    bal=1;}
                car+=1;}
            compt[car]='\0';
            if(strcmp(compt, ".huff") != 0){
                printf("error in entry args.\n");
                return 0;}
            else{
                FILE* fichier = NULL;
                fichier = fopen(argv[2], "r");
                if (fichier == NULL){
                    printf("Impossible d'ouvrir le fichier.");
                    return 0;}
                else{
                    huff_decompresse(fichier,argv[2]);}
                fclose(fichier);
                return 1;}}}
    else{
          printf("error in entry args.\n");}
}
