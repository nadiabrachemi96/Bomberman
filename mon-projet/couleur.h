#ifndef COULEUR

#define COULEURS
#include <stdio.h>
#define clrscn() printf("\033[H\033[2J")
#define couleur(param) printf("\033[%sm",param);
// couleur :30 =noir ; 31=rouge;32=vert;33=jaune;34=bleu;35=Magenta;36=Cyan;37=blanc
/*5 pour faire clignoter le test 1 active la haute intensité 
7 invesre la selection des couleurs si le systéme est noir sur blonc il devient blonc sur noir
 */
//couleur ("34"); pour bleu 


#endif