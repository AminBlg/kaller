# SPDX-FileCopyrightText: 2026 AminBlg
# SPDX-License-Identifier: GPL-3.0-or-later

add_compile_options(-Wall -Wextra)

add_library(kallercore STATIC
    core/telephony/agdevice.cpp
    core/telephony/agmodel.cpp
    core/telephony/call.cpp
    core/telephony/callmodel.cpp
    core/telephony/pendingendtracker.cpp
    core/logging.cpp
    core/telephony/bluezdevices.cpp
    core/telephony/telephonyclient.cpp
    core/contacts/contactsmodel.cpp
    core/contacts/pbapclient.cpp
    core/recents/recentsmodel.cpp
    core/notifications/incomingcallnotifier.cpp
    core/notifications/eventnotifier.cpp
    core/settings/settings.cpp
    core/settings/migration.cpp
)
target_include_directories(kallercore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kallercore PUBLIC
    Qt6::Core Qt6::DBus
    KF6::CoreAddons KF6::I18n KF6::Contacts KF6::Notifications KF6::ConfigCore
)

add_executable(kaller
    main.cpp
    app/trayicon.cpp
)

qt_add_resources(kaller "qml"
    PREFIX "/qml"
    BASE app/qml
    FILES
        app/qml/main.qml
        app/qml/Keypad.qml
)
target_link_libraries(kaller PRIVATE
    kallercore
    Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::Widgets
    KF6::I18nQml KF6::DBusAddons KF6::StatusNotifierItem
)

install(TARGETS kaller ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
