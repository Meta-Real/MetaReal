CC = gcc
CFLAGS = -O3 -Wall -std=c99 -pedantic

SDIR = srcs
HDIR = heads
ODIR = objs

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
OUT = metareal

make: $(ODIR) $(OUT)
	./$(OUT)

all: clean $(OUT)
	./$(OUT)

$(ODIR):
	@mkdir $(ODIR)
	@mkdir $(ODIR)/error
	@mkdir $(ODIR)/generator
	@mkdir $(ODIR)/lexer
	@mkdir $(ODIR)/optimizer
	@mkdir $(ODIR)/parser

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) -lgmp -lmpfr -lmpc

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(HDIR)

clean: $(ODIR)
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)

.PHONY: make all clean
