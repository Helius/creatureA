cmake_minimum_required(VERSION 3.16)

project(creatureA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Test REQUIRED)

qt_add_executable(appcreatureA
    main.cpp
    creature.cpp
    creature.h
    Direction.h
    worldmap.cpp
    worldmap.h
    WorldGeometry.h
    WorldGeometry.cpp
    lib/di.hpp
    Abilities.h
    abilityimpl.h
    abilityimpl.cpp
    worldprocessor.h
    worldprocessor.cpp
    ICreatureBuilder.h
    CreatureBuilderImpl.h
    CreatureBuilderImpl.cpp


    lib/qmlinjector/iqmlobjectcreator.cpp
    lib/qmlinjector/iqmlobjectcreator.h
    lib/qmlinjector/qmlinjectorbuilder.h
    lib/qmlinjector/qmlinjector.cpp
    lib/qmlinjector/qmlinjector.h

    presentation/WorldMapModel.h
    presentation/WorldMapModel.cpp

    usecase/RenderWorldUseCase.h
    usecase/RenderWorldUseCase.cpp
)

set_property(TARGET appcreatureA PROPERTY CXX_STANDARD 17)

qt_add_qml_module(appcreatureA
    URI creatureA
    VERSION 1.0
    QML_FILES qml/main.qml
)

set_target_properties(appcreatureA PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appcreatureA
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appcreatureA
    PRIVATE Qt6::Quick)


enable_testing(true)

add_executable(test_WorldGeometry tests/test_WorldGeometry.cpp WorldGeometry.cpp)
add_test(NAME test_WorldGeometry COMMAND test_WorldGeometry)
target_link_libraries(test_WorldGeometry Qt::Test)

