#!/usr/bin/env bash
DIR="/tmp/qt5"
git clone git://code.qt.io/qt/qt5.git ${DIR}
cd ${DIR} || exit 1
git checkout 5.12

perl init-repository -f

./configure -static -opensource -nomake examples -nomake tests -confirm-license

make -j6
sudo make install -j6
