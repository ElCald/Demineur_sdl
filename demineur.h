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
#define NB_BOMBE    10 //c'est un pourcentage 


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


void generer_bombes(vector<Bombe_t> &liste_bombe, int h, int l, int n, int x, int y);
int get_nb_bombe(Case_t** grille, int x, int y);
int initialiser_grille(Case_t** grille, vector<Bombe_t> &liste_bombe, int x, int y);
void afficher_grille(Case_t** grille);
bool bombe_in_liste(vector<Bombe_t> &liste_bombe, Bombe_t p);
int nb_case_visite(Case_t** grille);


#endif