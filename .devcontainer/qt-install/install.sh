QT_VERSION=6.8.3
QT_HOST="linux desktop"
QT_ARCH=linux_gcc_64
QT_SPEC=gcc_64
QT_OUTPUT_DIR=/opt/qt
QT_PATH=$QT_OUTPUT_DIR/$QT_VERSION/$QT_SPEC/bin

pip install aqtinstall

aqt list-qt $QT_HOST --arch $QT_VERSION
aqt install-qt $QT_HOST $QT_VERSION $QT_ARCH --outputdir $QT_OUTPUT_DIR

echo "export PATH=\"$QT_PATH:\$PATH\"" >> /etc/profile.d/qt-path.sh
chmod +x /etc/profile.d/qt-path.sh