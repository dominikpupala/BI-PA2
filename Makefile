CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -std=c++14
LIBS=-lSDL2 -lSDL2_image

all: compile doc

compile: pupaldom

pupaldom: src/Main.o src/Game.o src/GameObjects.o src/InputHandler.o src/FrameLimiter.o src/HighscoreLoader.o src/MapLoader.o src/ScoreCounter.o src/RenderManager.o src/TextureLoader.o src/Utility.o
	$(LD) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

doc: index

index:
	doxygen Doxyfile

clean:
	rm -rf src/*.o pupaldom

run: compile
	./pupaldom examples/maps/Map4.txt MakePlayer

deps:
	$(CXX) -MM src/*cpp > Makefile.d

-include Makefile.d