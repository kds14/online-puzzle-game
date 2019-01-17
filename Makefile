OUTDIR=./build/
TARGET=./build/trl.exe
SOURCES=./src/*.cpp
CC=cl
FLAGS=/W4 /EHsc
SDLLIB=C:\SDL\lib\win32
SDLINC=C:\SDL\include

all: $(TARGET)

$(TARGET):$(SOURCES)
	mkdir -p build
	$(CC) $(FLAGS) /Fo:$(OUTDIR) /Fe:$@ $**  /I $(SDLINC) /link /LIBPATH:$(SDLLIB) SDL2.lib SDL2main.lib /SUBSYSTEM:CONSOLE

clean:
	rm -r ./build
