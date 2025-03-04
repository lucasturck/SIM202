# Nom de l'exécutable
TARGET = main

# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Wfatal-errors -Wall -Wextra -g

# Fichiers sources et objets
SRC = main.cpp couches/couche.cpp classe_reseau/reseau.cpp cifar_10_image/image_processing.cpp
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
	rm -f $(OBJ)
realclean:
	rm -f $(OBJ) $(TARGET)