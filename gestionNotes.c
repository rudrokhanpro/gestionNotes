#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Macros
#define NB_NOTES_MAX 50

// Prototypage des fonctions
int menu();
int saisirNotes(float * notes);
void saisirNomFichier(char * nomFichier);
void enregistrerNotes(float * notes, int nbNotes);
void lireNotes(float * notes, int * nbNotes);
void reinitialiserNotes(float * notes);
void afficherNotesEtStatistiques(float * notes, int nbNotes);
void afficherStatistiques(float * notes, int nbNotes);
void afficherNotes(float * notes, int nbNotes);

// Définition des fonctions
int menu() {
  int choix = -1;

  printf("==================================================\n");
  printf("                     ACCEUIL                      \n");
  printf("==================================================\n");
  printf("1. Saisir des notes                               \n");
  printf("2. Enregistrer les notes                          \n");
  printf("3. Ouvrir un fichier de notes                     \n");
  printf("4. Afficher les notes avec les statistiques       \n");
  printf("5. Quitter                                        \n");

  do {
    printf("\n> Entrez votre choix:                            ");
    fflush(stdin);
    scanf("%d", &choix);
  } while (choix < 0 || choix > 5);

  printf("\n");

  return choix;
}

int saisirNotes(float * notes) {
  char newline;
  int nbNotes = 0, i = 0;
  float note = 0;

  printf("--------------------------------------------------\n");
  printf("                 SAISIR DES NOTES                 \n");
  printf("--------------------------------------------------\n");
  printf("Saisissez une note puis tapez <Entré>\n");
  printf("Pour arrêter la saisie, entrez '-1'.\n\n");

  while(note > -1 && i < NB_NOTES_MAX) {
    printf("> n° %d: ", i + 1);
    fflush(stdin);
    scanf("%f%c", &note, &newline);

    if (i == 0 && note < 0) {
      printf("=> Aucune note saisie.\n\n");
      return 0;
    }

    if (note >= 0) {
      notes[i] = note;
      i++;
      nbNotes++;
    };
  }

  printf("\n=> Vous avez saisi %d %s.\n", nbNotes, nbNotes == 1 ? "note" : "notes");

  return nbNotes;
}

void saisirNomFichier(char * nomFichier) {
  char bufferNomFichier[30];

  // saisie du nom de fichier
  printf("> Nom du fichier: ");
  fflush(stdin);
  fgets(bufferNomFichier, 30 - 6 - 1, stdin);

  // traitement du nom de fichier: -\n, +.notes
  size_t len = strlen(bufferNomFichier);
  bufferNomFichier[len - 1] = '\0';
  strcat(bufferNomFichier, ".notes");
  len += 6 + 1;
  
  strcpy(nomFichier, bufferNomFichier);
  printf("\n");
}

void enregistrerNotes(float * notes, int nbNotes) {
  char nomFichier[30];
  int succes = 0;
  FILE *fichierNotes;

  printf("--------------------------------------------------\n");
  printf("              ENREGISTRER LES NOTES               \n");
  printf("--------------------------------------------------\n");
  printf("Entrez un nom de fichier pour y enregistrer les\nnotes. ");
  printf("Un fichier avec l'extension '.notes' sera\nalors créé dans le répertoire actuel.\n\n");

  while (succes == 0) {
    saisirNomFichier(nomFichier);

    printf("=> Création du fichier '%s' en cours...\n", nomFichier);
    fichierNotes = fopen(nomFichier, "a+");
    
    // en cas d'erreur
    if (fichierNotes == NULL) printf("Erreur! Le fichier %s n'a pas pu être créé.\nRééssayez.\n\n", nomFichier);
    else succes = 1;
  }

  printf("=> Le fichier '%s' a bien été créé.\n\n", nomFichier);

  // ecriture du nombre de notes
  fprintf(fichierNotes, "%d\n", nbNotes);

  // écriture des notes
  int i;

  printf("=> Ecriture des notes dans '%s' en cours...\n", nomFichier);
  for (i = 0; i < nbNotes; i++) {
    fprintf(fichierNotes, "%.2f\t",  notes[i]);
  }
  printf("=> Ecriture terminé.\n");

  fclose(fichierNotes);
}

void lireNotes(float * notes, int * nbNotes) {
  printf("--------------------------------------------------\n");
  printf("            OUVRIR UN FICHIER DE NOTES            \n");
  printf("--------------------------------------------------\n");
  printf("Saisissez le nom ou l'emplacement du fichier à\nouvrir sans l'extension.\n\n");

  int succes = 0, i;
  char nomFichier[30];
  FILE  * fichierNotes;

  while (succes == 0) {
    saisirNomFichier(nomFichier);

    printf("=> Ouverte du fichier '%s' en cours...\n", nomFichier); 
    fichierNotes = fopen(nomFichier, "r");

    // en cas d'erreur
    if (fichierNotes == NULL) printf("Erreur! Le fichier %s n'a pas pu être ouvert.\nRééssayez.\n\n", nomFichier);
    else succes = 1;
  }
  
  printf("=> Le fichier '%s' a correctement été ouvert.\n\n", nomFichier);

  // lecture du nombre de notes
  fscanf(fichierNotes, "%d\n", nbNotes);

  // lecture des notes
  printf("=> Importation des notes en cours...\n");
  reinitialiserNotes(notes);
  for (i = 0; i < *nbNotes; i++) fscanf(fichierNotes, "%f\t", &notes[i]);

  printf("=> Importation terminée.\n\n");
}

void reinitialiserNotes(float * notes) {
  int i;

  // notes = { 0, 0, 0, ..., 0 }
  for (i = 0; i < NB_NOTES_MAX; i++) notes[i] = 0;
}

void afficherNotesEtStatistiques(float * notes, int nbNotes) {
  printf("--------------------------------------------------\n");
  printf("     AFFICHER LES NOTES ET LEUR STATISTIQUES      \n");
  printf("--------------------------------------------------\n");
  printf("Voici vos notes et leurs statistiques.\n\n");

  afficherStatistiques(notes, nbNotes);

  printf("\n");

  afficherNotes(notes, nbNotes);

  printf("\n");

}

void afficherStatistiques(float * notes, int nbNotes) {
  int i;
  float moyenne = 0, somme = 0;
  float min, max;

  min = max = notes[0];

  for (i = 1; i < nbNotes; i++) {
    // min
    if (notes[i] < min) min = notes[i];

    // max
    if (notes[i] > max) max = notes[i];
  
    // pour la moyenne
    somme += notes[i];
  }

  moyenne = somme / nbNotes;


  printf("              STATISTIQUE     VALEUR              \n");
  printf("              -----------     ------              \n");
  printf("              minimum         %2.2f\n", min);
  printf("              minimum         %2.2f\n", max);
  printf("              minimum         %2.2f\n", moyenne);
  printf("              cardinal        %d\n", nbNotes);
}

void afficherNotes(float * notes, int nbNotes) {
  int i;
  
  printf("              LISTE DES NOTES\n");
  printf("              ---------------\n");

  for (i = 0; i < nbNotes; i++) {
    printf("              %d.%s%.2f\n", i + 1, (i + 1) < 9 ? "  " : " ", notes[i]);
  }
}

// Fonction main
int main() {
  char newline, continuer;
  int choix;
  int nbNotes = 0;
  int notesSaisies = 0; // faux
  float notes[NB_NOTES_MAX];
  
  while ((choix = menu()) < 5) {
    scanf("%c", &newline);

    // 1. Saisir des notes
    if (choix == 1) {
      nbNotes = saisirNotes(notes);
      
      if (nbNotes > 0) notesSaisies = 1; // vrai
    }

    // 2. Enregistrer les notes
    else if (choix == 2) {

      // Si notes pas encore saisies
      if (notesSaisies == 0) {
        nbNotes = saisirNotes(notes);

        if (nbNotes > 0) notesSaisies = 1; // vrai
      }

      if (nbNotes > 0) enregistrerNotes(notes, nbNotes);
    }

    // 3. Ouvrir un fichier de notes
    else if (choix == 3) {
      lireNotes(notes, &nbNotes);
      notesSaisies = 1;
    }

    // 4. Afficher les notes avec les statistiques
    else if (choix == 4) {
      // Si notes pas encore saisies
      if (notesSaisies == 0) {
        nbNotes = saisirNotes(notes);

        if (nbNotes > 0) notesSaisies = 1; // vrai
      }

      if (nbNotes > 0) afficherNotesEtStatistiques(notes, nbNotes);
      else printf("Pas de notes à afficher.\n");
    }

    printf("\n> Entrer une touche pour contrinuer...");
    fflush(stdin);
    scanf("%c", &continuer);
    printf("\n");

    system("clear");
  }

  printf("Au revoir!\n");

  return 0;
}