FROM ubuntu:rolling

RUN apt-get update && \
    apt-get install -y \
        g++ \
        gdb \
        build-essential \
        libboost-filesystem-dev \
        libncurses-dev


RUN mkdir /app
COPY . /app

WORKDIR /app
