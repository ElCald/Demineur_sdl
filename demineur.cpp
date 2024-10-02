#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "demineur.h"

using namespace std;


/**
 * @return True si une bombe se trouve déjà au coordonnées choisi dans la liste
 */
bool bombe_in_liste(vector<Bombe_t> &liste_bombe, Bombe_t p){

    for(int i=0; i<liste_bombe.size(); i++){
        if(liste_bombe[i].x == p.x || liste_bombe[i].y == p.y){
            return true;
        }
    }

    return false;

} //fin bombe_in_liste


/**
 * @param h Position x max
 * @param l Position y max
 * @param n Nombre de bombes à créer
 * @param x Clique initial du joueur en X
 * @param y Clique initial du joueur en Y
 * Génère les coordonnées de toutes les bombes dans une liste pour les placer dans la grille
 */
void generer_bombes(vector<Bombe_t> &liste_bombe, int h, int l, int n, int x, int y){

    srand(time(NULL));

    Bombe_t p;
    
    for(int i=0; i<n; i++){

        do{
            p.x = rand()%h; 
            p.y = rand()%l;
        }while(p.x == x && p.y == y && bombe_in_liste(liste_bombe, p));
        
        liste_bombe.push_back(p);
    }
}//fin generer_bombes


/**
 * @return Le nombre de bombe autour d'une case
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


/**
 * @return Le nombre de bombes placé sur la grille
 */
int initialiser_grille(Case_t** grille, vector<Bombe_t> &liste_bombe, int x, int y){
    int nb_bombes = ((GRILLE_H*GRILLE_L)*NB_BOMBE)/100;
    generer_bombes(liste_bombe, GRILLE_H, GRILLE_L, nb_bombes, x, y);

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

    liste_bombe.clear();

    return nb_bombes;
} //fin initialiser_grille




/**
 * Affichage console de la grille
 */
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


/**
 * @return Le nombre de cases visités dans la grille
 */
int nb_case_visite(Case_t** grille){
    int n = 0;

    for(int i=0; i<GRILLE_H; i++){
        for(int j=0; j<GRILLE_L; j++){
            if(grille[i][j].visite == true){
                n++;
            }
        }
    }

    return n;
} //fin nb_case_visite
