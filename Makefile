CC=g++
CFLAGS=-w -O3 -c -Wall
OBJECTS=
HEADERS=
INCLUDEDIRS=
LIBDIRS=
LIBS=
EXECUTABLE=build/langTexGenerator.exe
# -------------------------------------
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(INCLUDEDIRS) $(LIBDIRS) $(LIBS) -o $(EXECUTABLE)

%.obj: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDEDIRS) -o $@ $<

clean:
	rm -rf *.obj $(EXECUTABLE)