# the compiler
CC = gcc

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# the build target executable:
LABVALORX = labValorX
CRIARFILHOS = criarFilhos
EXECEXAMPLE = execExample

all: $(LABVALORX) $(CRIARFILHOS) $(EXECEXAMPLE)

$(LABVALORX): $(LABVALORX).c
	$(CC) $(CFLAGS) -o $(LABVALORX) $(LABVALORX).c

$(CRIARFILHOS): $(CRIARFILHOS).c
	$(CC) $(CFLAGS) -o $(CRIARFILHOS) $(CRIARFILHOS).c

$(EXECEXAMPLE): $(EXECEXAMPLE).c
	$(CC) $(CFLAGS) -o $(EXECEXAMPLE) $(EXECEXAMPLE).c

clean:
	$(RM) $(LABVALORX) $(EXECEXAMPLE) $(CRIARFILHOS) $(EXECEXAMPLE)
