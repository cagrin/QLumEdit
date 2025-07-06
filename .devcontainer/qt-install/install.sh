QT_VERSION=6.8.3
QT_ARCH="linux desktop"
QT_SPEC=linux_gcc_64

pip install aqtinstall

aqt list-qt $QT_ARCH --arch $QT_VERSION
aqt install-qt $QT_ARCH $QT_VERSION $QT_SPEC