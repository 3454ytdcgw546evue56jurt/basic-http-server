all:
	clear
	g++ src/main.cpp -o build/http.app

clean:
	rm build/http.app