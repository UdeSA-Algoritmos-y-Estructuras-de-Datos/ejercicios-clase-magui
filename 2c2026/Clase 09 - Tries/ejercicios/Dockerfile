# Imagen mínima con g++, make y valgrind, para poder correr `make valgrind`
# en Mac con Apple Silicon (donde valgrind no está disponible nativamente).
FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends g++ make valgrind && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
