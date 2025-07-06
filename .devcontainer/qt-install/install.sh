QT_ARCH="linux desktop"
QT_VERSION=6.2.4
QT_SPEC=gcc_64

pip install aqtinstall

aqt list-qt $QT_ARCH --arch $QT_VERSION
aqt install-qt $QT_ARCH $QT_VERSION $QT_SPEC