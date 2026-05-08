PROG = operacoes
CPU = 7
CC = gcc -std=c11 -g
OBJS = matriz.o utils.o
CFLAGS =  -O3 -mavx2 -march=native
LFLAGS = -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

all: $(PROG)

debug: CFLAGS += -DDEBUG
debug: $(PROG)

$(PROG):  $(PROG).o

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@rm -f *~ *.bak *.tmp *.o $(PROG) *.txt

