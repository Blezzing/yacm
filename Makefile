main:
	mkdir -p bin
	g++ -std=c++17 src/main.cpp -o bin/yacm -Wall -Wextra -pedantic-errors -lstdc++fs

install:
	cp bin/yacm /usr/bin/yacm

sanitize:
	g++ -std=c++17 src/main.cpp -fsanitize=address -o bin/yacm -Wall -Wextra -pedantic-errors -lstdc++fs
