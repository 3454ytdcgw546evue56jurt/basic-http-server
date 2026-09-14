all:
	clear
	g++ src/main.cpp src/http.c -o build/http.app

clean:
	rm build/http.app