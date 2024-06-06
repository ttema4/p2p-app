cmake_minimum_required(VERSION 3.4)

project(p2p-app VERSION 0.1 LANGUAGES CXX)

# Включаем автоматическую обработку файлов UI, MOC и RCC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка пути для логов и debug-mode
add_definitions(-DLOG_DIR=\"${CMAKE_SOURCE_DIR}\")
add_definitions(-DDEBUG_MODE)

# Найти необходимые модули Qt
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Устанавливаем иконку для macOS
set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/resourses/icons/icon.icns)
set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")

# Добавляем исполняемый файл
qt_add_executable(p2p-app
    MANUAL_FINALIZATION
    main.cpp resourses.qrc
    mainwindow.h mainwindow.cpp mainwindow.ui
    mypage.h mypage.cpp mypage.ui
    headermenu.h headermenu.cpp
    textpixmapbutton.h textpixmapbutton.cpp
    loginpage.h loginpage.cpp loginpage.ui
    registerpage.h registerpage.cpp registerpage.ui
    settingspage.h settingspage.cpp settingspage.ui
    favouritepage.h favouritepage.cpp favouritepage.ui
    notifypage.h notifypage.cpp notifypage.ui
    chainmonitor.h chainmonitor.cpp chainmonitor.ui
    windowhandler.h windowhandler.cpp windowhandler.ui
    logic_fwd.h datareciever.h datareciever.cpp
    chaintableview.h chaintableview.cpp
    globalcondition.h globalcondition.cpp
    user.h user.cpp
    ${APP_ICON_MACOSX}
)

# Указываем дополнительные директории для включения
target_include_directories(p2p-app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann)

# Линкуем необходимые библиотеки
target_link_libraries(p2p-app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Устанавливаем свойства целевого объекта
set_target_properties(p2p-app PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Указываем инструкции по установке
install(TARGETS p2p-app
    BUNDLE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Завершаем настройку Qt, если используется Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(p2p-app)
endif()

# Добавляем пользовательскую цель для запуска приложения с аргументами
add_custom_target(run
    COMMAND p2p-app --log-file ${CMAKE_CURRENT_SOURCE_DIR}/logs.txt
    DEPENDS p2p-app
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
