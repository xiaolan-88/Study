#Get all .c files of the current file
SRC = $(wildcard *.c)

# name.c -> name.o
OBJ= $(patsubst %.c,%.o,$(SRC))

all:$(OBJ)

$(OBJ): %.o:%.c
	$(CC) $(INCLUDE_DIR) -c $^ -o $(OBJ_DIR)/$@
