GCC= g++
FLAGS= -O0
LIB= -lm -ggdb
SOURCE= painter.cpp image.cpp
HEADER= image.h RGB.h
OBJECT= $(SOURCE:.cpp=.o)
EXECUTABLE= painterly

all: $(SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECT)
	$(GCC) $(FLAGS) $(OBJECT) $(HEADER) -o $@

.cpp.o:
	$(GCC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean :
	rm *.o painterly