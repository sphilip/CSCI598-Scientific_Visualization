GCC = g++
CFLAGS = -O0 -Wall
LIBS = -lm -ggdb
SOURCES = raytrace.cpp image.cpp
HEADERS = image.h RGB.h Vector.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = volume

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) $(HEADERS) -o $@

.cpp.o:
	$(GCC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean :
	rm *.o *~ volume