#ifndef DEMINEUR_H
#define DEMINEUR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

#define GRILLE_L    20
#define GRILLE_H    30
#define NB_BOMBE    12 //c'est un pourcentage 


struct Case_t{
    int val;
    bool visite;
};

struct Bombe_t{
    int x;
    int y;
};

struct Coord_t{
    int x;
    int y;
};


void generer_bombes(vector<Bombe_t> &liste_bombe, int x, int y, int n);
int get_nb_bombe(Case_t** grille, int x, int y);
void initialiser_grille(Case_t** grille, vector<Bombe_t> &liste_bombe);
void afficher_grille(Case_t** grille);


#endif