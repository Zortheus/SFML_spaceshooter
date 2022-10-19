all: compileandrun

compileandrun:
	clear && clang++ -std=c++11 -stdlib=libc++ src/*.cpp -Iinclude -Llib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o main && ./main && clear