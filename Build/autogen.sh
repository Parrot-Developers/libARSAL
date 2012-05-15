#!/bin/sh

mkdir -p m4 config 

git log -v > ChangeLog

autoreconf --force --install -I config -I m4 

