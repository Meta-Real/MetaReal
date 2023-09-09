CC = gcc
CFLAGS = -O3 -Wall -std=c99 -pedantic

SDIR = srcs
HDIR = heads
ODIR = objs

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
OUT = metareal

make: $(OUT)
	./$(OUT)

all: clean $(OUT)
	./$(OUT)

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) -lgmp

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(HDIR)

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)

.PHONY: make all clean
