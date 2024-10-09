# Nom de l'exécutable
TARGET = programme

# Récupérer tous les fichiers .c dans le répertoire courant
SRC = $(wildcard *.c)

# Compiler avec gcc
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -O2

# Règle par défaut
all: $(TARGET)

# Lien des fichiers objets pour créer l'exécutable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Lancer le programme
run: $(TARGET)
	./$(TARGET)

# Supprimer l'exécutable après l'exécution
clean:
	rm -f $(TARGET)

# Exécuter et nettoyer
exec: run clean

# Lancer le programme avec Valgrind
val: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

.PHONY: all run clean exec val
