compile:
	clang++ frame.cpp main.cpp `wx-config --libs --cxxflags` -o program -lSDL2 -lSDL2_mixer
run:
	./program
clean:
	rm -rf program
