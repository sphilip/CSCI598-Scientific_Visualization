GCC = g++
FLAGS = -O0 -Wall -Werror
LIB = -lm -ggdb
SOURCE = painter.cpp image.cpp
HEADER = image.h RGB.h
OBJECT = $(SOURCE:.cpp=.o)
EXECUTABLE= painterly

all: $(SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECT)
	$(GCC) $(FLAGS) $(OBJECT) $(HEADER) -o $@

.cpp.o:
	$(GCC) $(FLAGS) -c $< -o $@ $(LIB)

clean :
	rm *.o *~ painterly