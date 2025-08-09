# Definindo variáveis
CC = gcc
CFLAGS = -Wall -std=c99 -g
LDFLAGS = -lraylib -lm -lpthread -ldl -lGL -lX11 -lstdc++

# Diretórios
SRCDIR = .
OBJDIR = ./obj
BINDIR = ./bin

# Arquivos fontes e objetos
SOURCES = $(SRCDIR)/man.c $(SRCDIR)/inimigos.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXEC = $(BINDIR)/jogo

# Criação dos diretórios se não existirem
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Regra padrão
all: $(EXEC)

# Compila o executável
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Compilação dos arquivos objetos
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Executa o programa
run: $(EXEC)
	./$(EXEC)