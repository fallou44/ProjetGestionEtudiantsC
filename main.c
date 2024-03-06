#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>


#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ROLE_LENGTH 20
#define MAX_CLASSES 10
#define MAX_APPRENANTS 50


typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char role[MAX_ROLE_LENGTH];
} User;

typedef struct {
    int idClasse;
    char nomClasse[50];
} Classe;

typedef struct {
    int idApprenant;
    char nomApprenant[50];
} Apprenant;

typedef struct {
    int idClasse;
    int idApprenant;
    int presence; // 1 pour présent, 0 pour absent
    char date[20]; // Format : JJ/MM/AAAA HH:MM:SS
} Presence;

int lireInfosAuthentification(char *username, char *password, char *role, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    char storedUsername[MAX_USERNAME_LENGTH], storedPassword[MAX_PASSWORD_LENGTH], storedRole[MAX_ROLE_LENGTH];
    while (fscanf(file, "%s %s %s", storedUsername, storedPassword, storedRole) == 3) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            strcpy(role, storedRole);
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
    
    
}

void marquerPresenceApprenant() {
    int idApprenant;
    int presence;

    // Demander à l'apprenant de saisir son ID
    printf("Saisir votre ID d'apprenant : ");
    scanf("%d", &idApprenant);

    // Demander à l'apprenant de marquer sa présence
    printf("Marquer votre présence (1 pour présent, 0 pour absent) : ");
    scanf("%d", &presence);

    // Enregistrer la présence dans le fichier
    FILE *file = fopen("presence.txt", "a");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de présence.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];
    sprintf(date, "%02d/%02d/%d %02d:%02d:%02d", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);

    fprintf(file, "%d %d %d %s\n", idApprenant, presence, 1, date); // Assurez-vous que le troisième argument correspond à la présence

    fclose(file);

    printf("Votre présence a été enregistrée avec succès.\n");
}

void marquerPresences(Classe classes[], int nbClasses, Apprenant apprenants[], int nbApprenants) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];
    sprintf(date, "%02d/%02d/%d %02d:%02d:%02d", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);

    FILE *file = fopen("presence.txt", "a");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de présence.\n");
        return;
    }

    int idClasse, idApprenant, presence;
    
    // Demander à l'admin de saisir les informations pour marquer la présence
    printf("Saisir l'ID de la classe : ");
    scanf("%d", &idClasse);
    
    // Afficher les apprenants de la classe sélectionnée
 
    
    printf("Saisir l'ID de l'apprenant : ");
    scanf("%d", &idApprenant);
    
    printf("Marquer la présence (1 pour présent, 0 pour absent) : ");
    scanf("%d", &presence);

    // Enregistrer la présence dans le fichier
    fprintf(file, "%d %d %d %s\n", idClasse, idApprenant, presence, date);

    fclose(file);
}

char* getNomApprenant(int idApprenant) {
    FILE *file = fopen("apprenants.txt", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier des apprenants.\n");
        return "Inconnu";
    }

    static char nomApprenant[50]; // Variable statique pour stocker le nom de l'apprenant

    int id;
    char nom[50];

    while (fscanf(file, "%d %s", &id, nom) == 2) {
        if (id == idApprenant) {
            strcpy(nomApprenant, nom);
            fclose(file);
            return nomApprenant;
        }
    }

    fclose(file);
    return "Inconnu";
}

void afficherApprenantsPresents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de présence.\n");
        return;
    }

    int idClasse, idApprenant, presence;
    char date[20], heure[20];

    printf("Apprenants présents aujourd'hui :\n");
    while (fscanf(file, "%d %d %d %s %s", &idClasse, &idApprenant, &presence, date, heure) == 5) {
        if (presence == 1) {
            char* nomApprenant = getNomApprenant(idApprenant);
            printf("Classe %d - Apprenant %d (%s) : présent à %s %s\n", idClasse, idApprenant, nomApprenant, date, heure);
        }
    }

    fclose(file);
}



int main() {
   Classe classes[MAX_CLASSES];
    Apprenant apprenants[MAX_APPRENANTS];
    int nbClasses = 0;
    int nbApprenants = 0;



    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char role[MAX_ROLE_LENGTH];
    int authentificationReussie = 0;
    int choix;

    do {
        printf("Connexion :\n");
        
        do {
            printf("Entrer votre username : ");
            fgets(username, MAX_USERNAME_LENGTH, stdin);
            username[strcspn(username, "\n")] = 0; // Supprimer le retour à la ligne
        } while (strcmp(username, "") == 0); // Demander à nouveau si le login est vide
        
        printf("Entrer votre mot de passe : ");
        system("stty -echo");
        fgets(password, MAX_PASSWORD_LENGTH, stdin);
        password[strcspn(password, "\n")] = 0; // Supprimer le retour à la ligne
	system("stty echo");
        authentificationReussie = lireInfosAuthentification(username, password, role, "authentification.txt");

        if (!authentificationReussie) {
            printf("Nom d'utilisateur ou mot de passe incorrect. Veuillez réessayer.\n");
        }
    } while (!authentificationReussie);

      if (strcmp(role, "admin") == 0) {
        printf("\nConnexion réussie en tant qu'admin.\n");

        printf("Voici le menu Admin :\n");
        printf("1. Gestion des étudiants\n");
        printf("2. Génération de fichiers\n");
        printf("3. Marque les presences\n");
        printf("4. Envoyer un meassage\n");
        printf("5. Liste presences\n");
        printf("6. Déconnexion\n");
        printf("Choisissez une option : ");
        scanf("%d", &choix);

            switch (choix) {
                case 1:
                    // gestion des étudiants
                    break;
                case 2:
                    // la génération de fichiers
                    break;
                case 3:
                    marquerPresences(classes, nbClasses, apprenants, nbApprenants);
                    break;
                case 4:
                    //  envoyer un message
                    break;
                 case 5:
                    afficherApprenantsPresents("presence.txt");
                    break;
              case 6:
    printf("Déconnexion...\n");
    authentificationReussie = 0; // Réinitialiser l'authentification
    break;


                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
            }
    } else if (strcmp(role, "apprenant") == 0) {
        printf("\nConnexion réussie en tant qu'apprenant.\n");

        printf("Menu Apprenant :\n");
        printf("1. Marquer ma présence\n");
        printf("2. Nombre de minutes d'absence\n");
        printf("3. Mes Messages (0)\n");
        printf("4. Déconnexion\n");
        printf("Choisissez une option : ");
        scanf("%d", &choix);

            switch (choix) {
                case 1:
                     marquerPresenceApprenant();
                    break;
                case 2:
                    // afficher le nombre de minutes d'absence
                    break;
                case 3:
                    // afficher les messages
                    break;
                case 4:
                    printf("Déconnexion...\n");
                    exit(0);
                    break;
                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
            }
    }

    return 0;
}

