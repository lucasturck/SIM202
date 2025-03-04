# Nom de l'exécutable
TARGET = main

# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Wall -Wextra -g

# Fichiers sources et objets
SRC = main.cpp reseau/reseau.cpp couches/couches.cpp cifar10/image_process.cpp
OBJ = $(SRC:.cpp=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Règle pour compiler les fichiers objets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyer les fichiers générés
clean:
	rm -f $(OBJ) $(TARGET)
