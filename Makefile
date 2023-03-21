.PHONY: build
build: linter clear
	mkdir build && cd build && cmake .. && cmake --build .

.PHONY: run
run: build
	./build/tp_highload_web_server ./configs/server.cfg

.PHONY: run-docker
run-docker:
	./build/tp_highload_web_server ./configs/server.cfg

.PHONY: vg-check
vg-check: build
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./build/tp_highload_web_server ./configs/server.cfg

.PHONY: clear
clear:
	rm -rf build/
	rm -rf cmake-build-debug/

.PHONY: linter
linter:
	cpplint --extensions=h,c --filter=-legal/copyright,-build/header_guard,-readability/casting,-runtime/printf,-runtime/arrays --linelength=120 project/include/* project/src/* project/main.c

.PHONY: docker-build
docker-build:
	docker build -t hl-web-server .

.PHONY: docker-run
docker-run:
	docker run --memory 2G --rm -d -p 8089:8089 --name hl-server -t hl-web-server

# Tests
.PHONY: func-test
func-test:
	./httptest.py

.PHONY: perf-server
perf-server:
	wrk -t12 -c400 -d30s 'http://127.0.0.1:8089/httptest/wikipedia_russia.html'

.PHONY: build-nginx
build-nginx:
	docker build -t nginx -f nginx.Dockerfile .

.PHONY: run-nginx
run-nginx:
	docker run --memory 2G --rm -d -p 8000:8000 --name nginx -t nginx

.PHONY: perf-nginx
perf-nginx:
	wrk -t12 -c400 -d30s 'http://127.0.0.1:8000/httptest/wikipedia_russia.html'


.PHONY: full
full:
	make docker-build && make docker-run && make perf-server

.PHONY: debug
debug:
	echo 'y' | docker container prune && make docker-build && docker run --memory 2G -d -p 8089:8089 --name hl-server -t hl-web-server && make perf-server
