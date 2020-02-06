# Docker image for compiler-explorer with loop_fusion

## Build
```sh
cd tools/docker
docker build -t "loop_fusion:latest" .
```

## Run

```sh
docker run -it     \
    --rm           \
    -p 10240:10240 \
    --name compiler_explorer \
    -v $(pwd)/url_storage:/opt/compiler-explorer/lib/storage/data \
    loop_fusion:latest
```


## Docker Compose
After the image is built, you can use the following `docker-compose.yml`
which not only uses the compiler-explorer but also nginx-proxy and 
Let's Encrypt.

```yml
version: '2'

services:

  nginx-proxy:
    container_name: nginx
    image: jwilder/nginx-proxy:0.6.0
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - /var/run/docker.sock:/tmp/docker.sock:ro
      - ./nginx_proxy/certs:/etc/nginx/certs:ro
      - ./nginx_proxy/vhosts:/etc/nginx/vhost.d
      - ./nginx_proxy/html:/usr/share/nginx/html
    labels:
      - "com.github.jrcs.letsencrypt_nginx_proxy_companion.nginx_proxy=true"
    environment:
      - "DEFAULT_HOST=mydomain.de"

  nginx-proxy-companion:
    container_name: nginx_companion
    image: jrcs/letsencrypt-nginx-proxy-companion
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock:ro
      - ./nginx_proxy/certs:/etc/nginx/certs:rw
    volumes_from:
      - nginx-proxy

  compiler_explorer:
    image: loop_fusion:latest
    container_name: compiler_explorer
    expose:
      - "10240"
    environment:
      - "VIRTUAL_HOST=explorer.mydomain.de"
      - "VIRTUAL_PORT=10240"
      - "LETSECNRYPT_EMAIL=info@mydomain.de"
      - "LETSENCRYPT_HOST=explorer.mydomain.de"
    volumes:
      - ./loop_fusion/url_storage:/opt/compiler-explorer/lib/storage/data
```
