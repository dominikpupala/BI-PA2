FrameLimiter.o: src/FrameLimiter.cpp src/FrameLimiter.h
Game.o: src/Game.cpp src/Game.h src/GameObjects.h src/MapLoader.h \
 src/ScoreCounter.h src/RenderManager.h src/Utility.h src/TextureLoader.h \
 src/InputHandler.h src/FrameLimiter.h src/HighscoreLoader.h
GameObjects.o: src/GameObjects.cpp src/GameObjects.h src/MapLoader.h \
 src/ScoreCounter.h src/RenderManager.h src/Utility.h src/TextureLoader.h
HighscoreLoader.o: src/HighscoreLoader.cpp src/HighscoreLoader.h
InputHandler.o: src/InputHandler.cpp src/InputHandler.h
Main.o: src/Main.cpp src/Game.h src/GameObjects.h src/MapLoader.h \
 src/ScoreCounter.h src/RenderManager.h src/Utility.h src/TextureLoader.h \
 src/InputHandler.h src/FrameLimiter.h src/HighscoreLoader.h
MapLoader.o: src/MapLoader.cpp src/MapLoader.h
RenderManager.o: src/RenderManager.cpp src/RenderManager.h src/Utility.h
ScoreCounter.o: src/ScoreCounter.cpp src/ScoreCounter.h
TextureLoader.o: src/TextureLoader.cpp src/TextureLoader.h
Utility.o: src/Utility.cpp src/Utility.h
