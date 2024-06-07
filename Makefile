# Definindo o compilador e flags de compilação
CC = gcc
CFLAGS = -Wall -std=c99 -g
LIBS = -lallegro -lallegro_primitives

# Nomes dos arquivos fonte e executável
SOURCES = tp.c automato.c
EXEC = automato

# Regra principal que compila o executável
all: $(EXEC)

$(EXEC): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC) $(LIBS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(EXEC) *.o

# Regra para executar o programa com Valgrind
valgrind: $(EXEC)
	valgrind --leak-check=full -s ./$(EXEC) inicializa.txt

