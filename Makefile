CC=g++
CFLAGS=-O3 -c -Wall -std=c++11
OBJECTS=main.obj
HEADERS=
INCLUDEDIRS=-Irapidjson/include
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