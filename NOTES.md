# How to make universal build on MacOS in Qt

## Modify Build Step

Check all ABIs:
* x86-darwin-generic-mach_o-64bit
* arm-darwin-generic-mach_o-64bit

## Add Custom Process Step

Command: ```%{Qt:QT_HOST_PREFIX}/bin/macdeployqt```

Arguments: ```%{ActiveProject:BuildConfig:Path}/%{ActiveProject:Name}.app -qmldir=%{ActiveProject:NativePath}```

