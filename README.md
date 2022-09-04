# Nginx Unit

---

## Dependency

```
$ sudo apt-get install libssl-dev libpcre2-dev
$ wget https://unit.nginx.org/download/unit-1.24.0.tar.gz #https://unit.nginx.org/download/
$ tar xzf unit-1.24.0.tar.gz
$ cd unit-1.24.0
$ ./configure --prefix=/home/ubuntu/unit_bin_1.24.0 --openssl
$ make libunit-install # only dependency
```

## Build

```
$ make
$ ./start.sh
$ curl 127.0.0.1:8400
{"code":200,"msg":"ok"}
```

## Reference

[Nginx blog](https://www.nginx.com/blog/nginx-unit-adds-assembly-language-support)

[issues](https://github.com/nginx/unit/issues/491)

[nxt_unit_app_test.c](https://github.com/nginx/unit/blob/master/src/test/nxt_unit_app_test.c)

[nxt_unit.h](https://github.com/nginx/unit/blob/master/src/nxt_unit.h)

[Source Code download](https://unit.nginx.org/download/)