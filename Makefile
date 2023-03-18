.PHONY: build
build: linter clear
	mkdir build && cd build && cmake .. && cmake --build .

.PHONY: run
run: build
	./build/tp_highload_web_server ./server.cfg

.PHONY: vg-check
vg-check: build
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./build/tp_highload_web_server ./server.cfg

.PHONY: clear
clear:
	rm -rf build/
	rm -rf cmake-build-debug/

.PHONY: linter
linter:
	cpplint --extensions=h,c --filter=-legal/copyright,-build/header_guard,-readability/casting --linelength=120 project/include/* project/src/* project/main.c
