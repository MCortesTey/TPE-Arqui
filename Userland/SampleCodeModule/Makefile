include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
OBJECTS=$(SOURCES:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) _loader.o
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(OBJECTS) _loader.o
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

clean:
	rm -rf *.o $(MODULE)

.PHONY: all clean