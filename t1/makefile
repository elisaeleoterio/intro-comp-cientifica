# Nome do executável final
PROG = broyden

# Arquivo fonte único
SRC = broyden.c
OBJS = $(SRC:.c=.o)

# Caminho da instalação do LIKWID (Ajuste este caminho se necessário)
LIKWID_HOME = /home/ubuntu/likwid-5.5.1

CC = gcc

# Flags de compilação exigidas:
# -O3: Otimização de alto nível
# -march=native: Otimiza para a arquitetura da máquina atual
# -mavx: Habilita suporte a instruções AVX (exigido no trabalho)
# -fopt-info-vec: Fornece informações sobre a vetorização de loops
CFLAGS = -std=c11 -g -O3 -march=native -mavx -fopt-info-vec -DLIKWID_PERFMON -I${LIKWID_HOME}/include

# Flags de ligação (Linker)
LDFLAGS = -L${LIKWID_HOME}/lib
LDLIBS = -llikwid -lm

.PHONY: all debug clean

# Regra principal
all: $(PROG)

# Regra para gerar o executável a partir dos objetos
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Regra para compilar o código-fonte em objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para depuração (desativa otimizações)
debug: CFLAGS += -DDEBUG -O0
debug: all

# Limpeza de arquivos temporários e executáveis
clean:
	rm -f $(OBJS) $(PROG) *.txt *~ *.bak *.tmp