# Makefile - Tut 07 (Árbol binario - ABB)
# Flags estrictos de la materia.

CXX      = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra -Wconversion -Werror
TARGET   = ejercicios
OBJS     = main.o ejercicios.o

# Compila el ejecutable (target por defecto).
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp ejercicios.h
	$(CXX) $(CXXFLAGS) -c main.cpp

ejercicios.o: ejercicios.cpp ejercicios.h
	$(CXX) $(CXXFLAGS) -c ejercicios.cpp

# Compila (si hace falta) y corre las pruebas.
run: $(TARGET)
	./$(TARGET)

# Compila y corre las pruebas bajo Valgrind, para detectar memory leaks y
# accesos inválidos a memoria.
# Nota: en Mac con Apple Silicon, valgrind no está disponible nativamente.
# Usá `make docker-valgrind` en ese caso.
valgrind: $(TARGET)
	valgrind -s --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Igual que `valgrind`, pero corriendo dentro de un container Linux (vía
# Docker). Sirve para Mac con Apple Silicon, donde valgrind no anda nativo.
docker-valgrind:
	docker build -t tut07-valgrind .
	docker run --rm -v "$(CURDIR)":/app -w /app tut07-valgrind bash -c "make clean && make valgrind"

# Borra los archivos generados.
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run valgrind docker-valgrind clean
