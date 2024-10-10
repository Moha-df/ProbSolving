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


typedef struct Node {
    Point point;
    struct Node **neighbors;
    int neighbor_count;
} Node;

Node* create_node(int x, int y) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->point.x = x;
    node->point.y = y;
    node->neighbors = NULL;
    node->neighbor_count = 0;
    return node;
}

void print_graph(Node **graph) {
    for (int i = 0; graph[i] != NULL; i++) {
        printf("Node at (%d, %d): ", graph[i]->point.x, graph[i]->point.y);
        for (int j = 0; j < graph[i]->neighbor_count; j++) {
            printf("(%d, %d) ", graph[i]->neighbors[j]->point.x, graph[i]->neighbors[j]->point.y);
        }
        printf("\n");
    }
}


Node** generate_entry_and_graph(int **maze, int width, int height) {
    //generate entry, ouais psq on s'en blc jfais tt au mm endroit ggggg
    int entry_x, entry_y;
    int found = 0;

    while (!found) {
        entry_x = 1 + rand() % (width - 1); 
        entry_y = 1 + rand() % (height - 1);
        if (maze[entry_y][entry_x] == SPACE) {
            maze[entry_y][entry_x] = ENTRY;
            found = 1;
        }
    }

    //

    Node **graph = (Node**)malloc(height * width * sizeof(Node*));
    int node_count = 0;

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (maze[y][x] == SPACE || maze[y][x] == ENTRY || maze[y][x] == DOOR) {
                Node* node = create_node(x, y);
                //ajout de tout les voisins du poto
                if (maze[y-1][x] == SPACE || maze[y-1][x] == ENTRY || maze[y-1][x] == DOOR) {
                    node->neighbor_count++;
                    node->neighbors = realloc(node->neighbors, node->neighbor_count * sizeof(Node*));
                    node->neighbors[node->neighbor_count - 1] = create_node(x, y-1);
                }
                if (maze[y+1][x] == SPACE || maze[y+1][x] == ENTRY || maze[y+1][x] == DOOR) {
                    node->neighbor_count++;
                    node->neighbors = realloc(node->neighbors, node->neighbor_count * sizeof(Node*));
                    node->neighbors[node->neighbor_count - 1] = create_node(x, y+1);
                }
                if (maze[y][x-1] == SPACE || maze[y][x-1] == ENTRY || maze[y][x-1] == DOOR) {
                    node->neighbor_count++;
                    node->neighbors = realloc(node->neighbors, node->neighbor_count * sizeof(Node*));
                    node->neighbors[node->neighbor_count - 1] = create_node(x-1, y);
                }
                if (maze[y][x+1] == SPACE || maze[y][x+1] == ENTRY || maze[y][x+1] == DOOR) {
                    node->neighbor_count++;
                    node->neighbors = realloc(node->neighbors, node->neighbor_count * sizeof(Node*));
                    node->neighbors[node->neighbor_count - 1] = create_node(x+1, y);
                }

                graph[node_count++] = node;
            }
        }
    }

    // ca ca sert a rien btw c zehma de loptimisation
    // psq on a allouer trop psq on allou pr les mur aussi alors quon les enregistre pas
    graph = realloc(graph, node_count * sizeof(Node*));

    // on marque la fin
    graph[node_count] = NULL;

    return graph;
}

void free_graph(Node **graph) {
    for (int i = 0; graph[i] != NULL; i++) {
        for (int j = 0; j < graph[i]->neighbor_count; j++) {
            free(graph[i]->neighbors[j]);
        }
        free(graph[i]->neighbors);
        free(graph[i]);
    }
    free(graph);
}


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
    
    int width = 10, height = 6;
    
    int **maze = generate_maze(width, height);


    divide(maze, 0, width, 0, height);

    Node** graph = generate_entry_and_graph(maze, width, height);
    printf("\nGraph:\n");
    print_graph(graph);

    printf("\nMaze:\n");
    print_maze(maze, width, height);

    free_maze(maze, height);
    free_graph(graph);

    return 0;
}

/*
#### // x0 = 0 ; xmax = 3
#  #
#  #
#  #
####


//y0 = 0; ymax = 4*/