cmake_minimum_required(VERSION 3.16)
project(ChatClient LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)  # <--- 必须显式为 ON，强制编译 .qrc

file(GLOB PB_FILES "./proto/*.proto")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Protobuf)

qt_standard_project_setup()

qt_add_executable(ChatClient
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwidget.cpp
    mainwidget.h
    model/data.h
    resource.qrc
    sessionfriendarea.h sessionfriendarea.cpp
    debug.h
    messageshowarea.h messageshowarea.cpp
    messageeditarea.cpp
    messageeditarea.h
    selfinfowidget.h selfinfowidget.cpp
    infowidget.h infowidget.cpp
    userinfowidget.h userinfowidget.cpp
    sidebar.h sidebar.cpp
    chatdetailspage.h chatdetailspage.cpp
    choosefriendwidget.h choosefriendwidget.cpp
    floatingscrollarea.h floatingscrollarea.cpp
    toolswidget.h
    toolswidget.cpp
    addfrienddialog.h addfrienddialog.cpp
    historymessagewidget.h historymessagewidget.cpp
    loginwindow.h loginwindow.cpp
    toast.h toast.cpp
    model/datacenter.h model/datacenter.cpp
)

qt_add_protobuf(ChatClient PROTO_FILES ${PB_FILES})

target_link_libraries(ChatClient
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(ChatClient PRIVATE Qt6::Widgets)
target_link_libraries(ChatClient PRIVATE Qt6::Widgets)

include(GNUInstallDirs)

install(TARGETS ChatClient
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ChatClient
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
