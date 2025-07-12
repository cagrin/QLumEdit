ARCH="$(uname -m)"
if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ]; then
    QT_HOST=linux_arm64
    QT_ARCH=linux_gcc_arm64
    QT_SPEC=gcc_arm64
elif [ "$ARCH" = "x86_64" ] || [ "$ARCH" = "amd64" ]; then
    QT_HOST=linux
    QT_ARCH=linux_gcc_64
    QT_SPEC=gcc_64
else
    echo "Unsupported architecture: $ARCH" >&2
    exit 1
fi
QT_VERSION=6.8.3
QT_OUTPUT_DIR=/opt/qt
QT_PATH=$QT_OUTPUT_DIR/$QT_VERSION/$QT_SPEC/bin

apt-get update && \
apt-get install -y pipx libgl1-mesa-dev libxkbcommon-x11-0 libxcb-cursor0 libxcb-keysyms1 libxcb-icccm4 && \
export PATH="$PATH:/root/.local/bin" && \
pipx install aqtinstall && \
aqt list-qt $QT_HOST desktop --arch $QT_VERSION && \
aqt install-qt $QT_HOST desktop $QT_VERSION $QT_ARCH --outputdir $QT_OUTPUT_DIR && \
echo "export PATH=\"$QT_PATH:\$PATH\"" >> /etc/profile.d/qt-path.sh && \
chmod +x /etc/profile.d/qt-path.sh && \
apt-get clean && rm -rf /var/lib/apt/lists/*