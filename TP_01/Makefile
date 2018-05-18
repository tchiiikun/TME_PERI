NAME=lee-fabre
CARD=22

SRC=src
OBJ=obj
BIN=bin
INC=include
LIB=lib


CROSS_COMPILE = bcm2708hardfp-
CFLAGS=-Wall -O2
LDFLAGS=-static -L. -lgpio

PROGS=$(BIN)/hello_world.x $(LIB)/libgpio.a $(BIN)/lab1.x

all: directories $(PROGS)

directories: ${OBJ} ${BIN} ${LIB}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${LIB}:
	mkdir ${LIB}

#hello_world
$(BIN)/hello_world.x: $(SRC)/hello_world.c
	$(CROSS_COMPILE)gcc -o $@ $< -I $(INC) -O2 -static

#exercices sans lib
$(BIN)/lab1_all_exercices.x: $(SRC)/lab1_all_exercices.c
	$(CROSS_COMPILE)gcc -o $@ $< -I $(INC) -O2 -static

#exercice 1 avec libgpio
#createur de lib
$(LIB)/libgpio.a: $(OBJ)/gpio_value.o $(OBJ)/gpio_setup.o
	$(CROSS_COMPILE)ar -rcs $@ $^

$(BIN)/lab1.x: $(SRC)/lab1.c
	$(CROSS_COMPILE)gcc -o $@ $< -I $(INC) -O2 -static $(LIB)/libgpio.a
#regle de compilation
$(OBJ)/%.o: $(SRC)/%.c
	$(CROSS_COMPILE)gcc -o $@ -c $(CFLAGS) $< -I $(INC)

#regle pour upload sur rbpy
upload: $(BIN)/*.x
	scp -P 50$(CARD) $^ pi@132.227.102.36:$(NAME)

clean:
	rm -f $(OBJ)/*.o $(BIN)/*.x $(SRC)/*~
