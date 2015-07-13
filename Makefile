MAINS=inspect input
OBJS=frame.o msg.o io.o
CFLAGS=-std=c11 -pedantic -Wall

all: $(MAINS)

debug: CFLAGS += -DDEBUG
debug: all

clean:
	rm -f $(MAINS) *.o

inspect: inspect.o $(OBJS)
	cc -o $@ $^

input: input.o $(OBJS)
	cc -o $@ $^ -lreadline

%.o: %.c %.h
	cc $(CFLAGS) -c -o $@ $<
