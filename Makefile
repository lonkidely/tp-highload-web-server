.PHONY: build
build: clear
	mkdir build && cd build && cmake .. && cmake --build .

.PHONY: run
run: build
	./build/tp_highload_web_server ./server.cfg

.PHONY: clear
clear:
	rm -rf build/
	rm -rf cmake-build-debug/
