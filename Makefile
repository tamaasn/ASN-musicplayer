compile:
	clang++ frame.cpp main.cpp `wx-config --libs --cxxflags` -o musicplayer -lSDL2 -lSDL2_mixer
run:
	./musicplayer
clean:
	rm -rf musicplayer
