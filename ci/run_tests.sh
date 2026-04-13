#!/usr/bin/env bash

docker compose -f docker-compose.yml -f docker-compose.pgsql.yml down -v
docker compose -f docker-compose.yml -f docker-compose.pgsql.yml build
docker compose -f docker-compose.yml -f docker-compose.pgsql.yml up -d

docker compose -f docker-compose.yml -f docker-compose.pgsql.yml exec seafile /bin/bash -c "cd /source/seafile-server/python && pip3 install --break-system-packages -r ../ci/requirements.txt"
docker compose -f docker-compose.yml -f docker-compose.pgsql.yml exec seafile /bin/bash -c "cd /source/seafile-server/python && python3 -m pytest"
