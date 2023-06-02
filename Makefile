CC = gcc

OBJS = engine.c graphics.c

LINKER_FLAGS = -lSDL2

OBJ_NAME = exc

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
