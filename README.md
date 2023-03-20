# Сервер для отдачи статики

## Архитектура и ЯП

`С++ thread pool`

## Сборка

nginx запускается на `8000` порту, веб-сервер на `8089`.

### Сборка и запуск сервера

```bash
make docker-build
make docker-run
```

### Запуск nginx

```bash
make build-nginx
make run-nginx
```

### Запуск функциональных тестов

```bash
make func-test
```

Результат:

```
./httptest.py
test_directory_index (__main__.HttpServer)
directory index file exists ... ok
test_document_root_escaping (__main__.HttpServer)
document root escaping forbidden ... ok
test_empty_request (__main__.HttpServer)
Send empty line ... ok
test_file_in_nested_folders (__main__.HttpServer)
file located in nested folders ... ok
test_file_not_found (__main__.HttpServer)
absent file returns 404 ... ok
test_file_type_css (__main__.HttpServer)
Content-Type for .css ... ok
test_file_type_gif (__main__.HttpServer)
Content-Type for .gif ... ok
test_file_type_html (__main__.HttpServer)
Content-Type for .html ... ok
test_file_type_jpeg (__main__.HttpServer)
Content-Type for .jpeg ... ok
test_file_type_jpg (__main__.HttpServer)
Content-Type for .jpg ... ok
test_file_type_js (__main__.HttpServer)
Content-Type for .js ... ok
test_file_type_png (__main__.HttpServer)
Content-Type for .png ... ok
test_file_type_swf (__main__.HttpServer)
Content-Type for .swf ... ok
test_file_urlencoded (__main__.HttpServer)
urlencoded filename ... ok
test_file_with_dot_in_name (__main__.HttpServer)
file with two dots in name ... ok
test_file_with_query_string (__main__.HttpServer)
query string with get params ... ok
test_file_with_slash_after_filename (__main__.HttpServer)
slash after filename ... ok
test_file_with_spaces (__main__.HttpServer)
filename with spaces ... ok
test_head_method (__main__.HttpServer)
head method support ... ok
test_index_not_found (__main__.HttpServer)
directory index file absent ... ok
test_large_file (__main__.HttpServer)
large file downloaded correctly ... ok
test_post_method (__main__.HttpServer)
post method forbidden ... ok
test_request_without_two_newlines (__main__.HttpServer)
Send GET without to newlines ... ok
test_server_header (__main__.HttpServer)
Server header exists ... ok

----------------------------------------------------------------------
Ran 24 tests in 0.030s

OK
```

## Запуск нагрузочного тестирования

[Инструмент](https://github.com/wg/wrk)

Все тесты проведены в Docker c ограничением использования оперативной памяти в 2 GB

1. **nginx сервер - worker_processes 8**

```bash
make perf-nginx
```

Результат:

```bash
wrk -t12 -c400 -d30s 'http://127.0.0.1:8000/httptest/wikipedia_russia.html'
Running 30s test @ http://127.0.0.1:8000/httptest/wikipedia_russia.html
  12 threads and 400 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   124.72ms   90.15ms 538.62ms   67.03%
    Req/Sec   282.36    135.75   737.00     68.75%
  101458 requests in 30.10s, 90.25GB read
Requests/sec:   3370.88
Transfer/sec:      3.00GB
```

2. **static-web-server - thread pool 8**

```bash
make perf-server
```

Результат:

```bash
wrk -t12 -c400 -d30s 'http://127.0.0.1:8089/httptest/wikipedia_russia.html'
Running 30s test @ http://127.0.0.1:8089/httptest/wikipedia_russia.html
  12 threads and 400 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    42.14ms    9.21ms 213.78ms   88.28%
    Req/Sec   344.86    263.79     1.18k    53.17%
  92731 requests in 30.10s, 82.48GB read
Requests/sec:   3081.02
Transfer/sec:      2.74GB
```

3. **Сравнительный анализ**

> 3081.02 / 3370.88 * 100 = 91.4 [%] - дает кастомный веб сервер по сравнению c Nginx
