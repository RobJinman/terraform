DODGE_LOCATION = ../Dodge

NAME = terraform
CC = g++
CFLAGS = -std=c++0x -Wall `sdl-config --cflags` -DLINUX -DGLEW -g -DDEBUG
INCL = -I$(DODGE_LOCATION)/Dodge/include
LIBS = -L$(DODGE_LOCATION)/Dodge/lib -L/usr/lib -lDodge -lX11 -lGL -lGLEW -lpnglite -lBox2D -lz `sdl-config --libs` -lSDLmain -lSDL -lSDL_mixer
SRC = src
OBJS = $(SRC)/Application.o \
	$(SRC)/CParallaxSprite.o \
	$(SRC)/CPhysicalEntity.o \
	$(SRC)/CPhysicalSprite.o \
	$(SRC)/CSprite.o \
	$(SRC)/Item.o \
	$(SRC)/main.o \
	$(SRC)/Player.o \
	$(SRC)/Soil.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCL) $< -o $@

clean:
	rm -f $(SRC)/*.o
