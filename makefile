linux:
	clear
	g++ \
	src/main.cpp \
	src/http.cpp \
	-g \
	-o build/http.app

windows:
	clear
	g++ \
	src/main.cpp \
	src/http.cpp \
	-fdiagnostics-color=always \
	-g \
	-static-libstdc++ \
	-fpermissive \
	-static-libgcc \
	-Wl,-Bstatic \
	-lstdc++ \
	-Wl,-Bdynamic \
	-static-libstdc++ \
	-lws2_32 \
	-o build/http.exe

clean:
	rm build/http.app