#!/usr/bin/env bash

git clone git://code.qt.io/qt/qt5.git
cd qt5
git checkout 5.12

perl init-repository -f

./configure -static -opensource -nomake examples -nomake tests -confirm-license

make -j8
sudo make install -j8

mv qt5 /tmp/qt5