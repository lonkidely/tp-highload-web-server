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
	cpplint --extensions=h,c --filter=-legal/copyright,-build/header_guard,-readability/casting,-runtime/printf --linelength=120 project/include/* project/src/* project/main.c

.PHONY: docker-build
docker-build:
	docker build -t hl-web-server .

.PHONY: docker-run
docker-run:
	docker run --memory 2G --rm -d -p 80:80 hl-web-server

# Tests
.PHONY: func-test
func-test:
	./httptest.py

.PHONY: bench-static-web-server
bench-static-web-server:
	wrk -t12 -c400 -d30s 'http://127.0.0.1:80/tests/splash.css'

.PHONY: build-nginx
build-nginx:
	docker build -t nginx -f nginx.Dockerfile .

.PHONY: run-nginx
run-nginx:
	docker run --memory 2G --rm -d -p 8000:8000 --name nginx -t nginx

.PHONY: bench-nginx
bench-nginx:
	wrk -t12 -c400 -d30s 'http://127.0.0.1:8000/tests/splash.css'
