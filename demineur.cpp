#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "demineur.h"

using namespace std;


/**
 * @param x Position x max
 * @param y Position y max
 * @param n Nombre de bombes à créer
 * Génère les coordonnées de toutes les bombes dans une liste pour les placer dans la grille
 * On n'est pas assuré que des bombes ne vont pas se super poser
 */
void generer_bombes(vector<Bombe_t> &liste_bombe, int x, int y, int n){

    srand(time(NULL));

    Bombe_t p;
    
    for(int i=0; i<n; i++){

        p.x = rand()%x; 
        p.y = rand()%y;

        liste_bombe.push_back(p);

    }
}//fin generer_bombes


/**
 * Retour le nombre de bombe autour d'une case
 */
int get_nb_bombe(Case_t** grille, int x, int y){
    int n = 0; //nb de bombe

    for(int i=x-1; i<=x+1; i++){
        for(int j=y-1; j<=y+1; j++){
            if(i>=0 && i<GRILLE_H && j>=0 && j<GRILLE_L){ // on vérifie sir une des valeur n'est pas dehors, auquel cas on ne cherche rien dans le tableau
                if(grille[i][j].val == -1){
                    n++;
                }    
            }  
        }
    }

    return n;
} //fin get_nb_bombe



void initialiser_grille(Case_t** grille, vector<Bombe_t> &liste_bombe){
    int nb_bombes = ((GRILLE_H*GRILLE_L)*NB_BOMBE)/100;
    generer_bombes(liste_bombe, GRILLE_H, GRILLE_L, nb_bombes);

    //init grille à zéro
    for(int i=0; i<GRILLE_H; i++){
        for(int j=0; j<GRILLE_L; j++){
            grille[i][j].val = 0;
            grille[i][j].visite = false;
        }
    }

    //init grille bombe
    for (int i=0; i<nb_bombes; i++){
        grille[liste_bombe.back().x][liste_bombe.back().y].val = -1;
        liste_bombe.pop_back();
    }


    //init grille valeurs
    for(int i=0; i<GRILLE_H; i++){
        for(int j=0; j<GRILLE_L; j++){
            if(grille[i][j].val != -1){
                grille[i][j].val = get_nb_bombe(grille, i, j);
            }
            
        }
    }
} //fin initialiser_grille





void afficher_grille(Case_t** grille){

    for(int i=0; i<GRILLE_H; i++){
        for(int j=0; j<GRILLE_L; j++){
            if(grille[i][j].val != -1){
                cout << " " << grille[i][j].val << " ";
            }
            else{
                cout << grille[i][j].val << " ";
            }
            
        }
        cout << endl;
    }
}//fin afficher_grille

