all: cxx ansic

cxx:
	@echo ---- Making C++ ----
	g++ -std=c++11 totvar.cpp -o cxx.exe
	./cxx.exe

ansic:
	@echo ---- Making ANSI C ----
	gcc totvar.c -o ansic.exe
	./ansic.exe