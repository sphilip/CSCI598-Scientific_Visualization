CC = g++
OPT = -O3
LIBS = -lm
OBJS = proj3.o image.o 

all : proj3

proj3 : $(OBJS)
	$(CC) $(OPT) -o proj3 $(OBJS) $(LIBS) RGB.h

proj3.o : proj3.cpp
	$(CC) $(OPT) -c proj3.cpp 
	
image.o : image.cpp image.h
	$(CC) $(OPT) -c image.cpp


clean: 
	rm *.o proj3

