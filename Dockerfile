FROM ubuntu:24.04

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

RUN apt-get install -y python3 python3-dev python3-pip python3-setuptools python3-ldap
RUN pip3 install --break-system-packages \
    pytz \
    jinja2 \
    Django==5.2.* \
    django-statici18n==2.3.* \
    django_webpack_loader==1.7.* \
    django_picklefield==3.1 \
    django_formtools==2.4 django_simple_captcha==0.6.* djangosaml2==1.11.* \
    djangorestframework==3.14.* python-dateutil==2.8.* pyjwt==2.10.* \
    pycryptodome==3.23.* python-cas==1.6.* pysaml2==7.5.* requests==2.28.* requests_oauthlib==1.3.* future==1.0.* gunicorn==20.1.* \
    mysqlclient==2.2.* qrcode==7.3.* pillow==11.3.* pillow-heif==1.0.* chardet==5.1.* cffi==1.17.1 captcha==0.7.* \
    psycopg2-binary \
    openpyxl==3.0.* Markdown==3.4.* bleach==5.0.* python-ldap==3.4.* sqlalchemy==2.0.* redis mock pytest \
    pymysql==1.1.* configparser pylibmc django-pylibmc nose exam splinter \
    pytest-django psd-tools lxml cairosvg==2.8.*

RUN mkdir /source
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
RUN cmake -DENABLE_FUSE=OFF -DWITH_POSTGRESQL=ON -GNinja ..
RUN ninja
RUN ninja install
RUN ldconfig

RUN mkdir -p /run/seafile
RUN chown -R 1000:1000 /run/seafile
RUN chown -R 1000:1000 /source/seafile-server/python/tests/resources
RUN mkdir /config && chown 1000:1000 /config
RUN mkdir /data && chown 1000:1000 /data

USER 1000:1000

VOLUME /config
VOLUME /data
VOLUME /tmp

ENTRYPOINT ["seaf-server", "--central-config-dir=/config", "--seafdir=/data", "--debug=all", "--rpc-pipe-path=/run/seafile", "--foreground", "--log", "-"]
