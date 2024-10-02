#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "demineur.h"


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define SIZE_CASE 16 //pixels


using namespace std;

SDL_Window* fenetre;
SDL_Renderer* renderer;


void ExitWithError(const char* msg){
    SDL_Log("Erreur : %s > %s\n", msg, SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    exit(EXIT_FAILURE);
}


void display_jeu(Case_t** grille, int** grille_flag){

    SDL_Texture* caseTexture_hide;
    SDL_Texture* caseTexture_void;
    SDL_Texture* caseTexture_flag;
    SDL_Texture* caseTexture_bomb;
    SDL_Texture* caseTexture_num[8];

    SDL_Surface* caseSurface_hide = NULL;
    SDL_Surface* caseSurface_void = NULL;
    SDL_Surface* caseSurface_flag = NULL;
    SDL_Surface* caseSurface_bomb = NULL;
    SDL_Surface* caseSurface_num[8];

    

    // Chargement d'une image
    if( (caseSurface_hide = SDL_LoadBMP("misc/images/case_hide.bmp")) == NULL )
        exit(EXIT_FAILURE);
    
    if( (caseSurface_void = SDL_LoadBMP("misc/images/case_void.bmp")) == NULL )
        exit(EXIT_FAILURE);

    if( (caseSurface_flag = SDL_LoadBMP("misc/images/case_flag.bmp")) == NULL )
        exit(EXIT_FAILURE);
    
    if( (caseSurface_bomb = SDL_LoadBMP("misc/images/case_bombe.bmp")) == NULL )
        exit(EXIT_FAILURE);

    if( (caseSurface_num[0] = SDL_LoadBMP("misc/images/case_1.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[1] = SDL_LoadBMP("misc/images/case_2.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[2] = SDL_LoadBMP("misc/images/case_3.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[3] = SDL_LoadBMP("misc/images/case_4.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[4] = SDL_LoadBMP("misc/images/case_5.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[5] = SDL_LoadBMP("misc/images/case_6.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[6] = SDL_LoadBMP("misc/images/case_7.bmp")) == NULL )
        exit(EXIT_FAILURE);
    if( (caseSurface_num[7] = SDL_LoadBMP("misc/images/case_8.bmp")) == NULL )
        exit(EXIT_FAILURE);

    // Chargement de l'image dans une texture pour pouvoir l'afficher
    if( (caseTexture_hide = SDL_CreateTextureFromSurface(renderer, caseSurface_hide)) == NULL ){
        SDL_FreeSurface(caseSurface_hide);
        exit(EXIT_FAILURE);
    }
    if( (caseTexture_void = SDL_CreateTextureFromSurface(renderer, caseSurface_void)) == NULL ){
        SDL_FreeSurface(caseSurface_void);
        exit(EXIT_FAILURE);
    }
    if( (caseTexture_flag = SDL_CreateTextureFromSurface(renderer, caseSurface_flag)) == NULL ){
        SDL_FreeSurface(caseSurface_flag);
        exit(EXIT_FAILURE);
    }
    if( (caseTexture_bomb = SDL_CreateTextureFromSurface(renderer, caseSurface_bomb)) == NULL ){
        SDL_FreeSurface(caseSurface_bomb);
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<8; i++){
        if( (caseTexture_num[i] = SDL_CreateTextureFromSurface(renderer, caseSurface_num[i])) == NULL ){
            SDL_FreeSurface(caseSurface_num[i]);
            exit(EXIT_FAILURE);
        }
    }
    

    // Libération de la mémoire utilisé par l'image car on ne l'utilise plus
    SDL_FreeSurface(caseSurface_hide); 
    SDL_FreeSurface(caseSurface_void);
    SDL_FreeSurface(caseSurface_flag);
    SDL_FreeSurface(caseSurface_bomb);
    for(int i=0; i<8; i++){
        SDL_FreeSurface(caseSurface_num[i]);
    }

    // Rendu du visuel
    SDL_RenderClear(renderer);

    SDL_Rect posCase;

    //boucles de la grille
    for(int i=0; i<GRILLE_H; i++){
        for(int j=0; j<GRILLE_L; j++){

            // Emplacement de l'image dans notre fenêtre
            if(grille[i][j].visite == false){
                if( (SDL_QueryTexture(caseTexture_hide, NULL, NULL, &posCase.w, &posCase.h)) != 0 )
                    exit(EXIT_FAILURE);
            }
            else{
                if(grille[i][j].val == 0){
                    if( (SDL_QueryTexture(caseTexture_void, NULL, NULL, &posCase.w, &posCase.h)) != 0 )
                        exit(EXIT_FAILURE);
                }
                else if(grille[i][j].val == -1){
                    if( (SDL_QueryTexture(caseTexture_bomb, NULL, NULL, &posCase.w, &posCase.h)) != 0 )
                        exit(EXIT_FAILURE);
                }
                else{
                    if( (SDL_QueryTexture(caseTexture_num[grille[i][j].val-1], NULL, NULL, &posCase.w, &posCase.h)) != 0 )
                        exit(EXIT_FAILURE);
                }
            }

            if(grille_flag[i][j] != 0){
                if( (SDL_QueryTexture(caseTexture_flag, NULL, NULL, &posCase.w, &posCase.h)) != 0 )
                    exit(EXIT_FAILURE);
            }
            

            posCase.w = SIZE_CASE;
            posCase.h = SIZE_CASE;
            posCase.x = i*SIZE_CASE;
            posCase.y = j*SIZE_CASE;


            // Chargement de la texture contenant l'image dans le rendu pour l'affichage
            if(grille[i][j].visite == false){
                if( (SDL_RenderCopy(renderer, caseTexture_hide, NULL, &posCase)) != 0 )
                    exit(EXIT_FAILURE);
            }
            else{
                if(grille[i][j].val == 0){
                    if( (SDL_RenderCopy(renderer, caseTexture_void, NULL, &posCase)) != 0 )
                        exit(EXIT_FAILURE);
                }
                else if(grille[i][j].val == -1){
                    if( (SDL_RenderCopy(renderer, caseTexture_bomb, NULL, &posCase)) != 0 )
                        exit(EXIT_FAILURE);
                }
                else{
                    if( (SDL_RenderCopy(renderer, caseTexture_num[grille[i][j].val-1], NULL, &posCase)) != 0 )
                        exit(EXIT_FAILURE);
                }
            }

            if(grille_flag[i][j] != 0){
                if( (SDL_RenderCopy(renderer, caseTexture_flag, NULL, &posCase)) != 0 )
                    exit(EXIT_FAILURE);
            }
        }
    }


    SDL_RenderPresent(renderer); // Actualise à l'écran

} //fin display_jeu


void parcours_largeur_grille(Case_t** grille, int x, int y){
    int orig_x = x/SIZE_CASE;
    int orig_y = y/SIZE_CASE;

    Coord_t case_visite;

    queue<Coord_t> file_case;
    file_case.push({orig_x, orig_y});
    grille[orig_x][orig_y].visite = true;

    while(!file_case.empty()){
        case_visite = file_case.front();
        file_case.pop();

        if(grille[case_visite.x][case_visite.y].val == 0){ //dès qu'on tombe sur un chiffre on s'arrête là  
            //on enfile les voisins non marqués qui ne sont pas des bombes et on les marques

            if(case_visite.x-1 >= 0 && grille[case_visite.x-1][case_visite.y].visite == false && grille[case_visite.x-1][case_visite.y].val != -1 ){
                grille[case_visite.x-1][case_visite.y].visite = true;
                file_case.push({case_visite.x-1, case_visite.y});

                if(case_visite.y-1 >= 0 && grille[case_visite.x-1][case_visite.y-1].visite == false && grille[case_visite.x-1][case_visite.y-1].val != -1){
                    grille[case_visite.x-1][case_visite.y-1].visite = true;
                    file_case.push({case_visite.x-1, case_visite.y-1});
                }

                if(case_visite.y+1 < GRILLE_L && grille[case_visite.x-1][case_visite.y+1].visite == false && grille[case_visite.x-1][case_visite.y+1].val != -1){
                    grille[case_visite.x-1][case_visite.y+1].visite = true;
                    file_case.push({case_visite.x-1, case_visite.y+1});
                }
            }

            if(case_visite.x+1 < GRILLE_H && grille[case_visite.x+1][case_visite.y].visite == false && grille[case_visite.x+1][case_visite.y].val != -1){
                grille[case_visite.x+1][case_visite.y].visite = true;
                file_case.push({case_visite.x+1, case_visite.y});

                if(case_visite.y-1 >= 0 && grille[case_visite.x+1][case_visite.y-1].visite == false && grille[case_visite.x+1][case_visite.y-1].val != -1){
                    grille[case_visite.x+1][case_visite.y-1].visite = true;
                    file_case.push({case_visite.x+1, case_visite.y-1});
                }

                if(case_visite.y+1 < GRILLE_L && grille[case_visite.x+1][case_visite.y+1].visite == false && grille[case_visite.x+1][case_visite.y+1].val != -1){
                    grille[case_visite.x+1][case_visite.y+1].visite = true;
                    file_case.push({case_visite.x+1, case_visite.y+1});
                }
            }

            if(case_visite.y-1 >= 0 && grille[case_visite.x][case_visite.y-1].visite == false && grille[case_visite.x][case_visite.y-1].val != -1){
                grille[case_visite.x][case_visite.y-1].visite = true;
                file_case.push({case_visite.x, case_visite.y-1});
            }

            if(case_visite.y+1 < GRILLE_L && grille[case_visite.x][case_visite.y+1].visite == false && grille[case_visite.x][case_visite.y+1].val != -1){
                grille[case_visite.x][case_visite.y+1].visite = true;
                file_case.push({case_visite.x, case_visite.y+1});
            }
        }

    }

} //fin parcour_largeur_grille


/**
 * Lorsqu'on clique gauche sur une case ça permet de l'a découvrir avec tout l'algo qui vient derrière
 */
void visiter_case(Case_t** grille, int** grille_flag, int x, int y){
    if(x/SIZE_CASE>=0 && x/SIZE_CASE<GRILLE_H && y/SIZE_CASE>=0 && y/SIZE_CASE<GRILLE_L){
        if(grille[x/SIZE_CASE][y/SIZE_CASE].visite == false && grille_flag[x/SIZE_CASE][y/SIZE_CASE] == 0){
            if(grille[x/SIZE_CASE][y/SIZE_CASE].val == -1){
                //cas où c'est une bombe
            }
            else{
                // grille[x/SIZE_CASE][y/SIZE_CASE].visite = true;
                parcours_largeur_grille(grille, x, y);
            }
        } 
    }
}

/**
 * Permet de poser un drapeau sur une grille différente pour protéger la case
 */
void flag_case(int** grille_flag, int x, int y){
    if(x/SIZE_CASE>=0 && x/SIZE_CASE<GRILLE_H && y/SIZE_CASE>=0 && y/SIZE_CASE<GRILLE_L){
        grille_flag[x/SIZE_CASE][y/SIZE_CASE] = !grille_flag[x/SIZE_CASE][y/SIZE_CASE];
    }
}



int main(int argc, char* argv[]){

    //--------------------------- Init interface graphique ---------------------------
    fenetre = NULL;
    renderer = NULL;
    SDL_Surface* perso = NULL;
    SDL_Texture* texture = NULL;

    // Initialisation de la SDL
    if( SDL_Init(SDL_INIT_VIDEO) != 0 )
        ExitWithError("Initialisation SDL");

    // Creation fenetre + rendu
    if( SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &fenetre, &renderer) != 0 )
        ExitWithError("Creation fenetre et rendu echouee");



    //--------------------------- Init jeu ---------------------------

    vector<Bombe_t> liste_bombe;

    Case_t** grille = (Case_t**)malloc(sizeof(Case_t*)*GRILLE_H);
    for(int i=0; i<GRILLE_H; i++){
        grille[i] = (Case_t*)malloc(sizeof(Case_t)*GRILLE_L);
    }

    int** grille_flag = (int**)malloc(sizeof(int*)*GRILLE_H);
    for(int i=0; i<GRILLE_H; i++){
        grille_flag[i] = (int*)malloc(sizeof(int)*GRILLE_L);
        for(int j=0; j<GRILLE_L; j++){
            grille_flag[i][j] = 0;
        }
    }



    initialiser_grille(grille, liste_bombe);

    afficher_grille(grille);

    

    SDL_bool program_launched = SDL_TRUE; // Conditionnelle pour garder la fenêtre ouverte

    while(program_launched){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){

                case SDL_KEYDOWN: // gestion du clavier
                    switch (event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            program_launched = SDL_FALSE; ;
                            break;
                        default:
                            continue;
                    }
                    
                    break;

                case SDL_MOUSEBUTTONDOWN: // clique de la souris
                    // printf("Clique : %d,%d\n", event.button.x, event.button.y); 

                    switch(event.button.button){ // le bouton cliqué sur la souris
                        case SDL_BUTTON_LEFT:
                            visiter_case(grille, grille_flag, event.button.x, event.button.y); break;
                        case SDL_BUTTON_RIGHT:
                            flag_case(grille_flag, event.button.x, event.button.y); break;
                        default:
                            continue;
                    }

                    break;

                case SDL_QUIT: // clique sur la croix de la fenêtre
                    program_launched = SDL_FALSE; break;
                
                default:
                    break;
            }

        }//fin boucle event

        display_jeu(grille, grille_flag);

    }//fin boucle main



    // --------------------------- Fin programme ---------------------------

    cout << "Fin de prog" << endl;

    for(int i=0; i<GRILLE_H; i++){
        free(grille[i]);
        free(grille_flag[i]);
    }
    free(grille);
    free(grille_flag);

    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    SDL_Quit(); 

    return EXIT_SUCCESS;
} //fin main