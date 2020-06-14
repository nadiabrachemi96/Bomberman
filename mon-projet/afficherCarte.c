#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <stropts.h>
#include <poll.h>
#include <fcntl.h>


#include "couleur.h"




#define BUFFSIZE 6000
#define MAX 6000
#define CAR 456

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXI 9


/************************************************************MENU****************************************************************/
struct actions
{
  char* descriptif;
  void (*fun)();
};
typedef struct actions action_t;


typedef struct menu menu_t;

enum item_d{  
  sous_menu,
  action,
  rien,  
};
typedef enum item_d item_e;


union item{

  menu_t* sous_menu;
  action_t act;
};
typedef union item item_u;


struct item_s
{
  item_e type;
  item_u effet;
};
typedef struct item_s item_t;

struct menu
{
  menu_t* menu_parent;
  char* descriptif;
  item_t ens[MAXI];
};


//----------------------------definition des fonctions--------------------------


//---------------creer le menu-----------------
menu_t* createMenu(const char* text){

  menu_t* menu;
  menu = malloc(sizeof(menu_t));

  char* dest = malloc(strlen(text)+1);
  strcpy(dest,text);

  menu->menu_parent = NULL;
  menu->descriptif = malloc(strlen(text)+1);
  strcpy(menu->descriptif,text);

  for (int i = 0; i < MAXI; i++)
  {
    menu->ens[i].type = rien;
  }

  return menu;

}


//---------------créer une action----------------------------
void addMenuAction(menu_t* m, const char* text, void(*f)()){


  char* dest = malloc(strlen(text)+1);
  strcpy(dest,text);

  for(int i = 0; i < MAXI; i++)
  {

    if((m->ens[i].type)==rien){

      m->ens[i].type = action;
      m->ens[i].effet.act.descriptif = malloc(strlen(dest)+1);
      strcpy(m->ens[i].effet.act.descriptif,dest);
      m->ens[i].effet.act.fun = f;
      return;
    }

  }
  
}


//--------------créer un sous menu------------------
void addSubMenu(menu_t* m, menu_t* sm){


  if(sm->menu_parent != NULL) return;
    
  for (int i = 0; i < MAXI; i++)
  {
      if (m->ens[i].type==rien)
      {
        m->ens[i].type = sous_menu;
        m->ens[i].effet.sous_menu = sm;
        sm->menu_parent = m;
        return;
      }
  }


}


//----------------Supprimer un menu-------------
void deleteMenu(menu_t* m){

  if(m->menu_parent != NULL) return;

    for (int i = 0; i < MAXI; i++)
    {
      if(m->ens[i].type==action) free(m->ens[i].effet.act.descriptif);
      
      if(m->ens[i].type==sous_menu) deleteMenu(m->ens[i].effet.sous_menu);      
    }

    free(m->descriptif);
    free(m);
    

}


//------------------Lancer un menu----------------
void launchMenu(menu_t* m){

//----------afficher menu et items----------------------
  if(m==NULL){
    printf("Menu inexistant\n");
    exit(EXIT_FAILURE);
  }

  printf("%s\n",m->descriptif);

  for (int i = 0; i < 9; i++)
  {
      

        if(m->ens[i].type==action)
          printf("%d - %s\n",i+1,m->ens[i].effet.act.descriptif);

        if(m->ens[i].type==sous_menu)
          printf("%d - %s\n",i+1,m->ens[i].effet.sous_menu->descriptif);
      

  }



//----------------effectuer un choix------------------------------

  printf("Choice ? p pour précédent\n");
  char choice[MAXI];
  scanf("%s",choice);
  
  int choix = atoi(choice);




//-----------analyse et effet du choix--------------

  if((choix >= 1) && (choix <= 9)){

          if(m->ens[choix-1].type == rien) launchMenu(m);


          if(m->ens[choix-1].type == action){

            m->ens[choix-1].effet.act.fun();
            printf("\n");
            
          }
          


          if(m->ens[choix-1].type == sous_menu){

            launchMenu(m->ens[choix-1].effet.sous_menu);
            printf("\n");
            //launchMenu(m);
          }



          

  }

  else if((choix==0) && (strcmp(choice,"p"))==0){

      printf("\n"); 
      if(m->menu_parent==NULL) launchMenu(m);
        else launchMenu(m->menu_parent);
    
  }
    


}



void f1() {
  printf("vous choisis le mod0 qui contient 2 niveaux\n");
}
 
void f2() {
  printf("vous choisis le mod0 qui contient 2 niveaux\n");
}
 
int cont = 1;
 
void commencer() {
  cont = 0;
  printf("vous avez choisis de commencer");
}



struct termios sauvegarde; 
 int mode_raw(int fd);
 int mode_canonique(int fd);

 int mode_canonique(int fd){
  tcsetattr(fd,TCSAFLUSH,&sauvegarde);
  return 0;
 }

  /************************************* MES STRUCTURES************************************************/

 typedef struct bomb bomberman;
 typedef enum { false , true } bool ;

//structure qui represente la position de mon joueur
 typedef struct 
 {
  int   x;
  int   y;  
 }pos_bombman;

// structure qui représente mon joueur 
  struct bomb{
  char* name;
  char map[10][45];
  char mapCar[10][44];
  int speed;
  int nombre_vie;;
  int porte_bombe;
  int nombre_bombe;
  bomberman* adversaire;
  pos_bombman position;

 };


/***************************MOUVEMENT DES persommages*****************************************
**/

 //on considére que le 0 le mur indestructible le 1 aprés 2 explisions de bombes

//gestion de collision entre deux personnages 

bool gestion_collision(bomberman* ennemie,bomberman* mine){
if((ennemie->position.x-mine->position.x)==0 && (ennemie->position.y-mine->position.x)==0)return true;
else return false;
}

//permet au personnage de tourner a gauche
pos_bombman mouv_left(bomberman* a , char tab [10][45]){
  if (tab[a->position.x][a->position.y-1] == ' ' || tab[a->position.x][a->position.y-1] == '*' ||tab[a->position.x][a->position.y-1] == '+' ||tab[a->position.x][a->position.y-1] == '@' ){
  if (tab[a->position.x][a->position.y-1] != '0'){//pour eviter de traverser un mur indistructible
  if(a->position.y >1 ){
    
    tab[a->position.x][a->position.y] =' ';
    }
  a->position.y= (a->position.y)-1;
  return a->position;
}}
  
  return a->position;

}

//permet au personnage de tourner à droite
pos_bombman mouv_right(bomberman* a,char tab [10][45]){
  if ( tab[a->position.x][a->position.y+1] == ' ' || tab[a->position.x][a->position.y+1] == '*' ||tab[a->position.x][a->position.y+1] == '+' ||tab[a->position.x][a->position.y+1] == '@'){
  if (tab[a->position.x][a->position.y+1] != '0'){
  if(a->position.y < 44 ){
    
    tab[a->position.x][a->position.y] =' ';
    }
  a->position.y= (a->position.y)+1;
return a->position;
}
}
return a->position;

}

//permet au personnage de monter et gérer les collisions avec le mur write(1,buffer,strlen sprintf)
pos_bombman mouv_top(bomberman* a,char tab [10][45]){
  if (tab[a->position.x-1][a->position.y] == ' ' || tab[a->position.x-1][a->position.y] == '*' ||tab[a->position.x-1][a->position.y] == '+' ||tab[a->position.x-1][a->position.y] == '@' ){
  if (tab[a->position.x-1][a->position.y] != '0'){
  if(a->position.x > 0){
    
    tab[a->position.x][a->position.y] =' ';
    }
  a->position.x= (a->position.x)-1;
  
return a->position;
}}
return a->position;
}
void effacer_adversaire(char tab [10][45]){
  for(int i=1;i<9;i++){
    for(int j=1;j<44;j++){
       if(tab[i][j]=='X')tab[i][j]=' ';
    }
  }
}
//permet au personnage de de descnedre
void mouv_low(bomberman* a,char tab [10][45]){
  if (tab[a->position.x+1][a->position.y] == ' ' || tab[a->position.x+1][a->position.y] == '*' ||tab[a->position.x+1][a->position.y] == '+' ||tab[a->position.x+1][a->position.y] == '@'){
  if (tab[a->position.x+1][a->position.y] != '0'){
  if(a->position.x < 10){
    
    tab[a->position.x][a->position.y] =' ';
    }
  a->position.x= (a->position.x)+1;
  

}}
}


//explosion de ma bambe
void bomb_explosion(bomberman* a,bomberman* b,char tab[10][45]){

couleur("31"); 
//Vérifier si le joueur a encore le droit de jouer (si il a encore une vie )sinon on déclare le joueur gagnant
  if((a->nombre_vie)<=0 ){
     system("clear");
     printf("le gagnant est le joueur 1");
     mode_canonique(STDIN_FILENO); /*si on rend pas le mode canonique il le restera méme si le programme est arrété */
     exit(-1);
  }
  if((b->nombre_vie)<=0 ){
     system("clear");
     printf("le gagnant est le joueur 2");
     mode_canonique(STDIN_FILENO);
     exit(-1);
  }

//Vérifier si le joueur adversaire est présent dans le champ de la bombe si c'est le cas il va perdre une vie 
if ((tab[a->position.x+1][a->position.y]==tab[b->position.x][b->position.y])||(tab[a->position.x-1][a->position.y]==tab[b->position.x][b->position.y])||
    (tab[a->position.x][a->position.y+1]==tab[b->position.x][b->position.y])||(tab[a->position.x][a->position.y-1]==tab[b->position.x][b->position.y])){
      b->nombre_vie--;
    }
 //démolir les murs "le mur 0 indéstructible" et "le mur 1 se démoli aprés 2 bombes "         
 if(a->position.x+1 != '0' && a->position.x-1 !='0' && a->position.x+1 !='1' && a->position.x-1 !='1' && a->position.x>0 && a->position.x<9){
     if(tab[a->position.x+1][a->position.y]=='1') { tab[a->position.x+1][a->position.y]='2'; } else {  if(tab[a->position.x+1][a->position.y]!='0')  tab[a->position.x+1][a->position.y]=' ';}
     if(tab[a->position.x-1][a->position.y]=='1') { tab[a->position.x-1][a->position.y]='2'; } else {  if(tab[a->position.x-1][a->position.y]!='0')  tab[a->position.x-1][a->position.y]=' ';}

 }
if(a->position.y+1 != '0' && a->position.y-1 != '0' && a->position.y>1 && a->position.y<44){
  if(tab[a->position.x][a->position.y+1]=='1') { tab[a->position.x][a->position.y+1]='2'; } else {  if(tab[a->position.x][a->position.y+1]!='0')  tab[a->position.x][a->position.y+1]='@';}
  if(tab[a->position.x][a->position.y-1]=='1') { tab[a->position.x][a->position.y-1]='2'; } else {  if(tab[a->position.x][a->position.y-1]!='0')  tab[a->position.x][a->position.y-1]=' ';}
}

}
//attente ded 1 seconde pour que ma bambe explose
void disparition_bomb(char tab[11][45]){
   
 // sleep(1);
  //mode_raw(STDIN_FILENO);
  for(int i=0;i<10;i++){
  for(int j=0;j<44;j++){
      if(tab[i][j]=='@'){
        tab[i][j]=' ';
      }
    }
  }
   
  }




//Afficher mon perssomage
void afficher_personage(char tab[10][45],bomberman *bomb){
  
  //affichage de mes personnages dans ma carte
  tab[bomb->position.x][bomb->position.y]='#';
  tab[bomb->adversaire->position.x][bomb->adversaire->position.y]='X';
 

} 
bool est_entier(char c){
  bool entier=false;
  
    if(c=='0' || c=='1' || c=='2' || c=='3' ||c=='4' || c=='5'
     || c=='6'|| c=='7' ||c=='8' || c=='9' )
      entier=true;
return entier;
}
/***************************************Gestion des powerups******************************************/
void afficher_powerups(char tab[10][45],char caracrteristiques[10][44]){
  for(int i=1;i<10;i++){
     for(int j=1;j<44;j++){
     if(caracrteristiques[i][j]=='*' || caracrteristiques[i][j]=='+' || caracrteristiques[i][j]=='@' )tab[i][j]=caracrteristiques[i][j];
  }
}
}
void prendre_powerups(char tab[][44],bomberman* bomb,char c ){
if (c=='z' || c =='A'){
  if (tab[bomb->position.x-1][bomb->position.y]=='*'){ bomb->speed++; tab[bomb->position.x-1][bomb->position.y]=' '; }
  if (tab[bomb->position.x-1][bomb->position.y]=='+'){ bomb->porte_bombe++; tab[bomb->position.x-1][bomb->position.y]=' '; }
  if (tab[bomb->position.x-1][bomb->position.y]=='@'){ bomb->nombre_bombe++; tab[bomb->position.x-1][bomb->position.y]=' '; }
}else{
if (c=='s' || c =='B'){
  if (tab[bomb->position.x+1][bomb->position.y]=='*'){ bomb->speed++; tab[bomb->position.x+1][bomb->position.y]=' '; }
  if (tab[bomb->position.x+1][bomb->position.y]=='+'){ bomb->porte_bombe++;  tab[bomb->position.x+1][bomb->position.y]=' '; }
  if (tab[bomb->position.x+1][bomb->position.y]=='@'){ bomb->nombre_bombe++; tab[bomb->position.x+1][bomb->position.y]=' '; }  

}else{
if (c=='d' || c =='C'){
  if (tab[bomb->position.x][bomb->position.y+1]=='*'){ bomb->speed++;  tab[bomb->position.x][bomb->position.y+1]=' ';}
  if (tab[bomb->position.x][bomb->position.y+1]=='+'){ bomb->porte_bombe++; tab[bomb->position.x][bomb->position.y+1]=' ';}
  if (tab[bomb->position.x][bomb->position.y+1]=='@'){ bomb->nombre_bombe++;  tab[bomb->position.x][bomb->position.y+1]=' ';}
}else{
  if (tab[bomb->position.x][bomb->position.y-1]=='*'){ bomb->speed++; tab[bomb->position.x][bomb->position.y-1]=' '; }
  if (tab[bomb->position.x][bomb->position.y-1]=='+'){ bomb->porte_bombe++; tab[bomb->position.x][bomb->position.y-1]=' '; }
  if (tab[bomb->position.x][bomb->position.y-1]=='@'){ bomb->nombre_bombe++; tab[bomb->position.x][bomb->position.y-1]=' '; }
 }
 }
 }
}



void cacher_powerups(char tab[10][45]){
   //affichage des powerups dans ma carte
 
  for(int i=0;i<10;i++){
  for(int j=0;j<44;j++){
    if(tab[i][j]=='*' || tab[i][j]=='+' || tab[i][j]=='@')tab[i][j]=' ';
}} 
}




/********************************TELECHARGEMENT DES DONNES*********************************/

//TELECHARGER UN PERSONNAGE A PARTIR D'UN FICHIER ET DE La metre dans une structure
//bomberman
bomberman* load_bomberman(char* name){
bomberman* ret=malloc(sizeof(bomberman));
  ret->name=name;
  int fd=open(name, O_RDONLY);
  char buffer[BUFFSIZE];
  
  int nread=0;
  
//lecture du fichier et recuperer tout dans un buffer
  if((nread=read(fd,buffer,BUFFSIZE))<0){
    perror("read");
  }
char copie[nread];

 //recuperatioin de la carte

for(int i=0;i<CAR-5;i++){
    copie[i]=buffer[i+5];
  }
  
  //ma carte a deux dimesntions
  
  int k=0;
  for(int i=0;i<11;i++){
    for(int j=0;j<45;j++){
      ret->map[i][j]=copie[k];
      k++;
    }
  }
  

//recuperation des caracrteristiques 
  char caract[MAX];
  for(int i=0;i<nread-CAR;i++){
    caract[i]=buffer[i+CAR];
     

  }
   
  int m=0;
  for(int i=0;i<10;i++){
    for(int j=0;j<44;j++){
      ret->mapCar[i][j]=caract[m];
     m++;
    }
  }

ret->nombre_vie=3;
ret->speed=0;
ret->porte_bombe=0;
ret->nombre_bombe=0;
close(fd);
return ret;
}






/**********************************MODE CANONIQUE ET NON CANONIQUE***************************************/

 int mode_raw(int fd){
  //creer une structure termios
  struct termios raw;

  //Recuperer la structure
   //de notre terminal
  if(tcgetattr( fd ,&raw)!=0) return -1;
  //la copier dans une nouvelle struct
  memcpy(&sauvegarde ,&raw,sizeof(struct termios));
  //utiliser la fonction cfmakeraw qui nous permet de passer en mode raw -non canonique-aprés avoir changer les par
  //De la structure termios
  cfmakeraw(&raw);
  //vmin minimum number of characters for noncanonical read(MIN).
  raw.c_cc[VMIN]  =  0;
  //mettre notre nouvelle structure termios non canonique a sa place 
  if(tcsetattr(fd,TCSANOW, &raw) != 0){
    return -1;
  }
  // -1 gestion des erreurs avec return -1;
  return 0;
 }
                       

 /*---AFFICHAGE DE BOMBERMAN---*/
void  affichage(){
  //le nom du jeux dans le terminal
  couleur("43");   
  printf("%s","                                     bomberman                                ");
  couleur("0");
}


int main(){
 menu_t* m;
 
  m = createMenu("Main menu");
  
  addMenuAction(m,"Mod0",f1);
  addMenuAction(m,"Mod1",f2);

 
  addMenuAction(m,"Commencer",commencer);
 
  while(cont) launchMenu(m);


 
  
  //récuperer ma map et mes caractéristisues 
    bomberman* mine=load_bomberman("mod0/niveaux/1");
    mine->adversaire=load_bomberman("mod0/niveaux/1");

    
    //choisir une position de depart pour mes joueurs
    mine->position.x=2;
    mine->position.y=2;
    mine->adversaire->position.x=4;
    mine->adversaire->position.y=4;
   
   afficher_personage(mine->map,mine);
   
   system("clear");
   mode_raw(STDIN_FILENO);
   //poll
   struct pollfd fds[2];

   fds[0].fd=STDOUT_FILENO;
   fds[1].fd=STDIN_FILENO;
   fds[0].events=POLLOUT;
   fds[1].events=POLLIN;

   
   char c;
  
   bool un =true;
  
   while(un){
   poll(fds,2,2000000);
   if(fds[1].revents & POLLIN){

  
    if(read(STDIN_FILENO ,&c ,1)==1){
      if (c=='a'){
       break;
        
      }
      if(c=='z' ){
        affichage(mine,mine->adversaire);
        mouv_top(mine,mine->map);
        afficher_personage(mine->map,mine);
        prendre_powerups(mine->mapCar,mine,c);
        mode_canonique(STDIN_FILENO);
        system("clear");
        fflush(stdout);
        write(STDOUT_FILENO,mine->map,451);
        mode_raw(STDIN_FILENO);
      }else{
        if (c=='s'){
                affichage(mine,mine->adversaire);
                mouv_low(mine,mine->map);
                afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine,c);
                mode_canonique(STDIN_FILENO);
                system("clear");
                fflush(stdout);
                write(STDOUT_FILENO,mine->map,451);
                mode_raw(STDIN_FILENO);

        }else{
          if(c=='q'){
                affichage(mine,mine->adversaire);
                mouv_left(mine,mine->map);
                afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine,c);
                mode_canonique(STDIN_FILENO);
                system("clear");
                fflush(stdout);
                write(STDOUT_FILENO,mine->map,451);
                mode_raw(STDIN_FILENO);
           }else{
            if(c=='d'){
                affichage(mine,mine->adversaire);
                mouv_right(mine,mine->map);
                afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine,c);
                mode_canonique(STDIN_FILENO);
                system("clear");
                fflush(stdout);
                write(STDOUT_FILENO,mine->map,451);
                mode_raw(STDIN_FILENO);

          }else{
            if(c==' '){
                couleur("31");
                affichage(mine,mine->adversaire);
                bomb_explosion(mine,mine->adversaire,mine->map);
                afficher_personage(mine->map,mine);
                mode_canonique(STDIN_FILENO);
                system("clear");
                fflush(stdout);
                afficher_powerups(mine->map,mine->mapCar);
                write(STDOUT_FILENO,mine->map,451);
                disparition_bomb(mine->map);
                cacher_powerups(mine->map);
                couleur("0");
                mode_raw(STDIN_FILENO);
            }else{
            if(c=='D'){
                affichage(mine,mine->adversaire);
   		    	    mouv_left(mine->adversaire,mine->map);
   			        afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine->adversaire,c);
   			        mode_canonique(STDIN_FILENO);
   			        system("clear");
   			        fflush(stdout);
   			        write(STDOUT_FILENO,mine->map,451);
   			        mode_raw(STDIN_FILENO);
            }else{
           	if(c=='C'){
                affichage(mine,mine->adversaire);
   		         	mouv_right(mine->adversaire,mine->map);
   			        afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine->adversaire,c);
   			        mode_canonique(STDIN_FILENO);
   			        system("clear");
   			        fflush(stdout);
   			        write(STDOUT_FILENO,mine->map,451);
   			        mode_raw(STDIN_FILENO);
            }else{
            	if(c=='A'){
                affichage(mine,mine->adversaire);
   		    	    mouv_top(mine->adversaire,mine->map);
   			        afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine->adversaire,c);
   			        mode_canonique(STDIN_FILENO);
   			        system("clear");
   			        fflush(stdout);
   			        write(STDOUT_FILENO,mine->map,451);
   			        mode_raw(STDIN_FILENO);
            }else{
            	if(c=='B'){
                affichage(mine,mine->adversaire);
   		    	    mouv_low(mine->adversaire,mine->map);
   			        afficher_personage(mine->map,mine);
                prendre_powerups(mine->mapCar,mine->adversaire,c);
   			        mode_canonique(STDIN_FILENO);
   			        system("clear");
   			        fflush(stdout);
   			        write(STDOUT_FILENO,mine->map,451);
   			        mode_raw(STDIN_FILENO);
            }else{
            	 if (c=='b'){
                affichage(mine,mine->adversaire);
                bomb_explosion(mine->adversaire,mine,mine->map);
                printf("%d",mine->nombre_vie);
                afficher_personage(mine->map,mine);
                mode_canonique(STDIN_FILENO);
                system("clear");
                fflush(stdout);
                afficher_powerups(mine->map,mine->mapCar);
                write(STDOUT_FILENO,mine->map,451);
                disparition_bomb(mine->map);
                cacher_powerups(mine->map);
                mode_raw(STDIN_FILENO);
}
            	}
            }

            }
        }
   		    }
   		}
   	}

   
   }}
}
}
} 


  /*manipulation de l'adversaire*/

mode_canonique(STDIN_FILENO);

   	return 0;
}