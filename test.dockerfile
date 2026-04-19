FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y \
    apt-transport-https \
    autoconf \
    automake \
    cmake \
    build-essential \
    ninja-build \
    curl  \
    flex  \
    git  \
    intltool  \
    libarchive-dev  \
    libargon2-dev \
    libcurl4-openssl-dev  \
    libevent-dev  \
    libffi-dev \
    libfuse-dev \
    libglib2.0-dev  \
    libhiredis-dev \
    libjansson-dev \
    libjwt-dev \
    libldap2-dev \
    libmemcached-dev \
    libonig-dev  \
    libpcre3-dev \
    libmysqlclient-dev  \
    libpq-dev \
    librados-dev  \
    libsqlite3-dev  \
    libssl-dev \
    libtool \
    libxml2-dev  \
    libxslt1-dev \
    libz-dev  \
    netbase \
    pkg-config  \
    mariadb-client \
    postgresql-client \
    re2c  \
    sqlite3  \
    unzip  \
    uuid-dev  \
    valac  \
    xz-utils

RUN mkdir /source
WORKDIR /source

RUN git clone https://github.com/justjanne/libevhtp /source/libevhtp
RUN mkdir -p /source/libevhtp/build
WORKDIR /source/libevhtp/build
RUN cmake -DEVHTP_DISABLE_SSL=ON -DEVHTP_BUILD_SHARED=OFF -GNinja ..
RUN ninja
RUN ninja install
RUN ldconfig

RUN git clone https://github.com/justjanne/libsearpc /source/libsearpc
RUN mkdir -p /source/libsearpc/build
WORKDIR /source/libsearpc/build
RUN cmake -GNinja ..
RUN ninja
RUN ninja install
RUN ldconfig

ADD . /source/seafile-server
RUN mkdir -p /source/seafile-server/build
WORKDIR /source/seafile-server/build
RUN cmake -DENABLE_FUSE=OFF -DWITH_MYSQL=ON -DWITH_POSTGRESQL=ON -GNinja ..
RUN ninja
RUN ninja install
RUN ldconfig

FROM ubuntu:24.04


RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    libarchive13t64 \
    libargon2-1 \
    libcurl4t64 \
    libevent-2.1-7t64 \
    libglib2.0-0t64 \
    libhiredis1.1.0 \
    libjansson4 \
    libjwt2 \
    libmysqlclient21 \
    libpq5 \
    libsqlite3-0 \
    git \
    python3 \
    python3-dev \
    python3-pip \
    python3-setuptools \
    python3-ldap \
 && rm -rf /var/lib/apt/lists/*

COPY --from=builder /usr/local/bin /usr/local/bin
COPY --from=builder /usr/local/lib /usr/local/lib
RUN ldconfig

ADD python /tests
ADD ci/requirements.txt /tests/

RUN mkdir -p /run/seafile
RUN chown -R 1000:1000 /run/seafile
RUN mkdir -p /tests/resources/download_dir
RUN chown -R 1000:1000 /tests
RUN mkdir /config && chown 1000:1000 /config
RUN mkdir /data && chown 1000:1000 /data

USER 1000:1000

VOLUME /config
VOLUME /data
VOLUME /tmp

ENTRYPOINT ["seaf-server", "--central-config-dir=/config", "--seafdir=/data", "--debug=all", "--rpc-pipe-path=/run/seafile", "--foreground", "--log", "-"]
