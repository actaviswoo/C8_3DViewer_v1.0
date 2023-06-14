#!/bin/bash

cd ../../
docker rmi -f ereva/valgrind/3dviewer
docker build . -t ereva/valgrind/3dviewer -f src/Valgrind/Dockerfile
docker run --rm ereva/valgrind/3dviewer

