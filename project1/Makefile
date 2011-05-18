GCC = g++
CFLAGS = -O0

SOURCES = raytrace.cpp image.cpp
HEADERS = image.h RGB.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = volume

all: $(SOURCES) $(EXECUTABLE)

# $(SOURCES):
# 	$(GCC) $(CFLAGS) $(OBJECTS) $(HEADERS) -o $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) $(HEADERS) -o $@

.cpp.o:
	$(GCC) $(CFLAGS) -c $< -o $@

clean :
	rm *.o volume