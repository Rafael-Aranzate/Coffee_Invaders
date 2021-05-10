#OBJS specifies which files to compile as part of the project
OBJS = jogo.c

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Jogo

#This is the target that compiles our executable
all : $(OBJS)
	gcc $(OBJS) -w `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o $(OBJ_NAME)

