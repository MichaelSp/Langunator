android {
    qmlcomponents.source = src/imports
    qmlcomponents.target =
    DEPLOYMENTFOLDERS += qmlcomponents

    qmlplugins.files = \
        src/plugins/libandroidplugin_1_1.so \
        src/plugins/libqtcomponentsplugin_1_1.so

    x86 {
        qmlplugins.path = /libs/x86
    } else: armeabi-v7a {
        qmlplugins.path = /libs/armeabi-v7a
    } else {
        qmlplugins.path = /libs/armeabi
    }
    INSTALLS += qmlplugins
}
