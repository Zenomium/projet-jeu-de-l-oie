#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_CASE 3

void conversion(int pos, int *x, int *y) {
    int spirale[10][10] = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {35, 36, 37, 38, 39, 40, 41, 42, 43, 10},
        {34, 63, 64, 65, 66, 67, 68, 69, 44, 11},
        {33, 62, 83, 84, 85, 86, 87, 70, 45, 12},
        {32, 61, 82, 95, 96, 97, 88, 71, 46, 13},
        {31, 60, 81, 94, 99, 98, 89, 72, 47, 14},
        {30, 59, 80, 93, 92, 91, 90, 73, 48, 15},
        {29, 58, 79, 78, 77, 76, 75, 74, 49, 16},
        {28, 57, 56, 55, 54, 53, 52, 51, 50, 17},
        {27, 26, 25, 24, 23, 22, 21, 20, 19, 18}
    };

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (spirale[i][j] == pos) {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

void collision(int positions[], int attente[], int nbJoueurs, int joueurCourant, int anciennePosition, int nouvellePosition) {
    for (int i = 0; i < nbJoueurs; i++) {
        if (i != joueurCourant && positions[i] == nouvellePosition) {
            positions[i] = anciennePosition;
            printf("Collision entre joueur %d et joueur %d. Ils échangent de place.\n", joueurCourant, i);
        }
    }
}

int avancerJoueur(char plateau[], int positions[], int attente[], int joueurCourant, int nbJoueurs, int des[2], int premierTour) {
    if (attente[joueurCourant] == -1) {
        return -1;
    }
    if (attente[joueurCourant] > 0) {
        attente[joueurCourant]--;
        return -1;
    }
    
    int sommeDes = des[0] + des[1];
    if (premierTour) {
        if ((des[0] == 3 && des[1] == 6) || (des[0] == 6 && des[1] == 3)) {
            sommeDes = 40;
        } else {if ((des[0] == 4 && des[1] == 5) || (des[0] == 5 && des[1] == 4)) {
            sommeDes = 89;
        }
        }
    }
    printf("Le joueur %d avance de %d cases\n", joueurCourant + 1, sommeDes);

    int nouvellePosition = positions[joueurCourant] + sommeDes; 
    if (nouvellePosition == 99) {
        return joueurCourant;
    }
    if (nouvellePosition > 99) {
        nouvellePosition = 99 - (nouvellePosition - 99);
    }

    int x, y;
    conversion(nouvellePosition, &x, &y);
    char casePlateau = plateau[x * 10 + y];

    int autreJoueurDansLeTrou = 0;
    switch (casePlateau) {
        case 'O':
            nouvellePosition += sommeDes;
            printf("Oie : Vous avancez à nouveau de %d\n", sommeDes);
            break;

        case 'T':
            for (int i = 0; i < nbJoueurs; ++i) {
                if (i != joueurCourant && nouvellePosition == positions[i]) {
                    attente[i] = 0;
                    printf("Trou : le joueur %d sort du trou\n", i + 1);
                    autreJoueurDansLeTrou = 1; // Marquer qu'un autre joueur était dans le trou
                }
            }
            if (!autreJoueurDansLeTrou) {
                attente[joueurCourant] = -1;
                printf("Trou : Vous devez attendre qu'un joueur prenne votre place\n");
            }
            break;

        case 'R':
            nouvellePosition = 16;
            printf("Vous prenez le raccourci et vous déplacez en case 16.\n");
            break;

        case 'P':
            for (int i = 0; i < nbJoueurs; i++) {
                if (i != joueurCourant && nouvellePosition == positions[i]) {
                    attente[joueurCourant] = 0;
                    attente[i] = 0;
                    printf("Le joueur %d et le joueur %d s'évadent de prison.\n", joueurCourant + 1, i + 1);
                } else {
                    attente[joueurCourant] = -1;
                    printf("Vous êtes en prison et devez attendre qu'on vous aide à vous évader.\n");
                    return -1;
                }
                break;
            }

        case 'H':
            if (attente[joueurCourant] == 0) {
                attente[joueurCourant] = 2;
                printf("Hôtel : Vous êtes à l'hôtel et passez votre tour deux fois.\n");
            } else {
                attente[joueurCourant]--;
            }
            break;

        case 'L':
            nouvellePosition -= 13;
            printf("Labyrinthe : Vous êtes dans un labyrinthe et reculez de 13 cases.\n");
            break;

        case 'X':
            nouvellePosition = 0;
            printf("Mort : Vous avez rencontré la tête de mort et retournez à la case 0.\n");
            break;

        default:
            printf("Vous êtes sur une case neutre.\n");
            break;
    }
    int anciennePosition = positions[joueurCourant];
    positions[joueurCourant] = nouvellePosition;
    collision(positions, attente, nbJoueurs, joueurCourant, anciennePosition, nouvellePosition);
    return -1;
}

void afficherPlateau(char plateau[10][10], int positions[], int nbJoueurs) {
    int x, y;
    char affichage[10][10][TAILLE_CASE + 1]; // +1 pour le caractère de fin de chaîne

    // Initialisation de l'affichage avec le plateau
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (plateau[i][j] == ' ') {
                strcpy(affichage[i][j], "[ ]"); // '[ ]' pour les cases vides
            } else {
                sprintf(affichage[i][j], "[%c]", plateau[i][j]); // Encadre les caractères spéciaux
            }
        }
    }

    // Ajout des positions des joueurs
    for (int i = 0; i < nbJoueurs; i++) {
        conversion(positions[i], &x, &y);
        sprintf(affichage[x][y], "*%d ", i+1); // Marque les joueurs
    }

    // Affichage du plateau
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%s ", affichage[i][j]);
        }
        printf("\n");
    }

    // Affichage des positions des joueurs
    for (int i = 0; i < nbJoueurs; i++) {
        printf("Joueur %d : case %d\n", i + 1, positions[i]);
    }
}


/*----------------------------------------------------------------------------------------------
  --------------------------------  Fonction principale  ---------------------------------------
  ----------------------------------------------------------------------------------------------*/

int main() {
    int nb_joueurs = 0;
    while (nb_joueurs < 2 || nb_joueurs > 4) {
        printf("Saisissez le nombre de joueurs :\n");
        scanf("%d", &nb_joueurs);
    }
    char plateau[10][10] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'R', 'O'},
        {' ', 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', 'O', ' ', 'L', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', 'O', ' '},
        {'H', ' ', ' ', 'X', 'V', ' ', ' ', 'O', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'O', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
        {' ', ' ', ' ', ' ', 'O', 'T', ' ', ' ', ' ', ' '},
        {'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'O'}
    };

    int positions[nb_joueurs];
    for (int i = 0; i < nb_joueurs; i++) {
        positions[i] = 0;
    }
    int x, y;
    int attente[nb_joueurs];
    for (int i = 0; i < nb_joueurs; i++) {
        attente[i] = 0;
    }
    int des[2] = {0, 0};
    int gagnant = -1;
    int joueurCourant = 0;
    int premierTour = 1;
    srand(time(NULL));    

    while (gagnant == -1) {
        afficherPlateau(plateau, positions, nb_joueurs);
        printf("\nTour du joueur %d :\n", joueurCourant + 1);
        char reponse = 'c';
        printf("Joueur %d, Souhaitez-vous continuer et lancer vos dés (c) ou déclarer forfait (f) ?\n", joueurCourant + 1);
        scanf(" %c", &reponse);
        if (reponse == 'f') {
            printf("La partie a été stoppée.\n");
            return (0);
        }
        des[0] = rand() % 6 + 1;
        des[1] = rand() % 6 + 1;
        printf("Vous avez fait un %d et un %d.\n", des[0], des[1]);
        gagnant = avancerJoueur(plateau[0], positions, attente, joueurCourant, nb_joueurs, des, premierTour);
        conversion(positions[joueurCourant], &x, &y);
        joueurCourant = (joueurCourant + 1) % nb_joueurs;
        premierTour = 0;
    }

    printf("Le joueur %d a gagné la partie!\n", gagnant + 1);

    return 0;
}