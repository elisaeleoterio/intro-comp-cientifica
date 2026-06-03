# Caminho da instalação do LIKWID (Ajuste este caminho se necessário)
LIKWID_HOME = /home/demo/likwid-5.5.1

CC = gcc
# Flags de compilação exigidas:
# -O3: Otimização de alto nível
# -march=native: Otimiza para a arquitetura da máquina atual
# -mavx: Habilita suporte a instruções AVX
# -fopt-info-vec: Fornece informações sobre a vetorização de loops

#-O3   -march=native  -mavx   -fopt-info-vec;
CFLAGS = -std=c11 -g -O3 -march=native -mavx -fopt-info-vec -DLIKWID_PERFMON -I${LIKWID_HOME}/include
# Flags de ligação (Linker)
LDFLAGS = -L${LIKWID_HOME}/lib -Wl,-rpath=${LIKWID_HOME}/lib
LDLIBS = -llikwid -lm #adição da biblioteca do likwid e da math

PROG = broyden
OBJS = broyden.o utils.o matriz.o

.PHONY: all debug clean # não sei oque isso aqui faz

# Regra principal
all: $(PROG)

# Regra para gerar o executável a partir dos objetos
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Regra para compilar o broyden.c
broyden.o: broyden.c utils.h matriz.h
	$(CC) $(CFLAGS) -c broyden.c

# Regra para criar o utils.o
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

# Regra para criar o matriz.o
matriz.o: matriz.c matriz.h
	$(CC) $(CFLAGS) -c matriz.c

# Limpeza de arquivos temporários e executáveis
clean:
	rm -f $(OBJS) $(PROG) *.txt *~ *.bak *.tmp