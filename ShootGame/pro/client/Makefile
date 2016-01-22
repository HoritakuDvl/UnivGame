MAKE = make
CC = gcc
CFLAGS = -O2 -Wall
OBJS = main.o client.o client_Event.o client_PlayerSelect.o client_PlayerData.o client_PlayerShotPattern.o client_EnemyData.o client_EnemyPattern.o client_EnemyShotPattern.o client_ItemData.o client_System.o client_GameTitle.o
EXE = client

all: $(EXE)
.c.o:	
	$(CC) -c $< $(CFLAGS)
$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) -lm -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

clean:
	rm -f $(EXE) $(OBJS) *~ *.o *.core
