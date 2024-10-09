#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL 1
#define SPACE 0
#define ENTRY 2
#define EXIT 3

typedef struct {
    int x, y;
} Point;

void init_maze(int **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                maze[i][j] = WALL;
            } else {
                maze[i][j] = SPACE;
            }
        }
    }

    maze[height - 2][width - 2] = EXIT;
}


int** generate_maze(int width, int height) {
    int **maze = (int **)malloc(height * sizeof(int *));
    if (maze == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        maze[i] = (int *)malloc(width * sizeof(int));
        if (maze[i] == NULL) {
            printf("Erreur d'allocation de mémoire\n");
            exit(1);
        }
    }

    init_maze(maze, width, height);

    return maze;
}

void generate_entry(){

    return;
}

// Afficher le labyrinthe
void print_maze(int **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == WALL) printf("#");
            else if (maze[i][j] == SPACE) printf(" ");
            else if (maze[i][j] == ENTRY) printf("E");
            else if (maze[i][j] == EXIT) printf("X");
        }
        printf("\n");
    }
}

void free_maze(int **maze, int height) {
    for (int i = 0; i < height; i++) {
        free(maze[i]);  // Libère chaque ligne
    }
    free(maze);  // Libère le tableau de pointeurs
}

void place_door(int **maze, int x_start, int x_end, int y_start, int y_end, int horizontal) {
    if (x_start >= x_end - 1 || y_start >= y_end - 1) {
        return;
    }
    
    x_start++;
    x_end--;
    y_start++;
    y_end--;


    if (horizontal) {
        // Assurer que la porte n'est pas placée sur le mur du bas
        int door_position = x_start  + rand() % (x_end - x_start); // Position de la porte à l'intérieur
        maze[y_end][door_position] = SPACE; // Placer une porte dans le mur horizontal (mur supérieur)
    } else {
        // Assurer que la porte n'est pas placée sur le mur de droite
        int door_position = y_start + rand() % (y_end - y_start); // Position de la porte à l'intérieur
        maze[door_position][x_end] = SPACE; // Placer une porte dans le mur vertical (mur gauche)
    }
}


// Diviser l'espace pour générer le labyrinthe
void divide(int **maze, int x_start, int x_end, int y_start, int y_end) {
    
    if (x_end - x_start < 2 || y_end - y_start < 2) return;

    int horizontal = (x_end - x_start) < (y_end - y_start); // Diviser dans la direction la plus grande

    if (horizontal) {
        // Placer un mur horizontal
        int y_wall = y_start + 1 + rand() % (y_end - y_start - 1); // Position du mur horizontal
        for (int x = x_start; x < x_end; x++) {
            maze[y_wall][x] = WALL; // Construire le mur horizontal
        }

        // Placer une porte dans ce mur
        place_door(maze, x_start, x_end, y_start, y_wall, 1);

        // Diviser récursivement les deux sous-espaces
        divide(maze, x_start, x_end, y_start, y_wall); // Espace au-dessus
        divide(maze, x_start, x_end, y_wall + 1, y_end); // Espace en dessous
    } else {
        // Placer un mur vertical
        int x_wall = x_start + 1 + rand() % (x_end - x_start - 1); // Position du mur vertical
        for (int y = y_start; y < y_end; y++) {
            maze[y][x_wall] = WALL; // Construire le mur vertical
        }

        // Placer une porte dans ce mur
        place_door(maze, x_start, x_wall, y_start, y_end, 0);

        // Diviser récursivement les deux sous-espaces
        divide(maze, x_start, x_wall, y_start, y_end); // Espace à gauche
        divide(maze, x_wall + 1, x_end, y_start, y_end); // Espace à droite
    }

}



int main() {
    srand(time(NULL));
    
    int width = 21, height = 21;
    
    int **maze = generate_maze(width, height);

    divide(maze, 0, width, 0, height);

    generate_entry();



    print_maze(maze, width, height);

    free_maze(maze, height);;

    return 0;
}
