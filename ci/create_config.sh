#!/usr/bin/env bash
fileserver=${1:-c}
db=${2:-mysql}

cd $(dirname $0)

cat template/ccnet.$db.conf > config/ccnet.conf
cat template/fileserver.$fileserver.conf template/seafile.$db.conf > config/seafile.conf
