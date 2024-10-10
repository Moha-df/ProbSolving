#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL 1
#define SPACE 0
#define ENTRY 2
#define EXIT 3
#define DOOR 4


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
            else if (maze[i][j] == DOOR) printf("-");
        }
        printf("\n");
    }
}

void free_maze(int **maze, int height) {
    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze); 
}

void place_door(int **maze, int x_start, int x_end, int y_start, int y_end, int horizontal) {
    if (horizontal) {
        int door_position;
        if((x_end - x_start - 2) == 0){
            printf("\n\n CACA HAPEN \n\n");
            door_position = x_start + 1; 
        }else{
                door_position = x_start + 1 + rand() % (x_end - x_start - 2); 
        }
        maze[y_end][door_position] = DOOR;  // Place la porte en tant qu'espace
    } 
    else {
                int door_position;
        if((y_end - y_start - 2) == 0){
            printf("\n\n CACA HAPEN \n\n");
            door_position = y_start + 1; 
        }else{
                door_position = y_start + 1 + rand() % (y_end - y_start - 2); 
        }
        maze[door_position][x_end] = DOOR;  // Place la porte en tant qu'espace
    }
}

int generateYwallCoord(int y_end, int y_start){
    int y_wall;
    if((y_end - y_start - 4) == 0){
            y_wall = y_start + 2; 
    }else{
            y_wall = y_start + 2 + rand() % (y_end - y_start - 4); 
    }
    return y_wall;
}
int generateXwallCoord(int x_end, int x_start){
    int x_wall;
    if((x_end - x_start - 4) == 0)
    {
        x_wall = x_start + 2; 
    }else{
        x_wall = x_start + 2 + rand() % (x_end - x_start - 4); 
    }
    return x_wall;
}


int checkYWall(int **maze, int x_start, int x_end, int y_wall){
    if(maze[y_wall][x_start] == DOOR){
        return 0;
    }
    if(maze[y_wall][x_end-1] == DOOR){
        return 0;
    }
    return 1;
}

int checkXWall(int **maze, int y_start, int y_end, int x_wall){
    if(maze[y_start][x_wall] == DOOR){
        return 0;
    }
    if(maze[y_end-1][x_wall] == DOOR){
        return 0;
    }
    return 1;
}


void divide(int **maze, int x_start, int x_end, int y_start, int y_end) {
    int width = x_end - x_start - 1;
    int height = y_end - y_start - 1;
    int state;

    if (width == 3 && height > 3) {
        state = 1; // Ca passe
    } else if (width > 3 && height == 3) {
        state = 1; // Ca passe
    } else if (width <= 3 || height <= 3) {
        state = 0; // Ca - tastrophique
    }else {
        state = 1; // Cas normalement ca passe inchAllah
    }

    switch (state) {
        case 0:
            return;
        case 1:
            break;
        default:
            return;
    }

    int horizontal = (x_end - x_start) < (y_end - y_start); 
    int is_square = (x_end - x_start) == (y_end - y_start);

    if (is_square || horizontal) {
        int y_wall = 0;
        int max_attempts = 50;
        int attempts = 0;

        do {
            y_wall = generateYwallCoord(y_end, y_start);
            //printf("Attempts (horizontal): %d \n", attempts);
            attempts++;
        } while (!checkYWall(maze, x_start, x_end, y_wall) && attempts < max_attempts);

        if (attempts < max_attempts) {
            // Division horizontale réussie
            for (int x = x_start + 1; x < x_end; x++) {
                maze[y_wall][x] = WALL;
            }
            place_door(maze, x_start, x_end, y_start, y_wall, 1);

            divide(maze, x_start, x_end, y_start, y_wall + 1); 
            divide(maze, x_start, x_end, y_wall, y_end);
            return;
        }
    }

    if (is_square || !horizontal) {
        int x_wall = 0;
        int max_attempts = 50;
        int attempts = 0;

        do {
            x_wall = generateXwallCoord(x_end, x_start);
            //printf("Attempts (vertical): %d \n", attempts);
            attempts++;
        } while (!checkXWall(maze, y_start, y_end, x_wall) && attempts < max_attempts);

        if (attempts < max_attempts) {
            // Division verticale réussie
            for (int y = y_start + 1; y < y_end; y++) {
                maze[y][x_wall] = WALL;
            }
            place_door(maze, x_start, x_wall, y_start, y_end, 0);

            divide(maze, x_start, x_wall + 1, y_start, y_end);
            divide(maze, x_wall, x_end, y_start, y_end);
        }
    }
}



int main() {
    srand(time(NULL));
    
    int width = 150, height = 10;
    
    int **maze = generate_maze(width, height);


    divide(maze, 0, width, 0, height);

    generate_entry();



    print_maze(maze, width, height);

    free_maze(maze, height);

    return 0;
}

/*
#### // x0 = 0 ; xmax = 3
#  #
#  #
#  #
####


//y0 = 0; ymax = 4*/