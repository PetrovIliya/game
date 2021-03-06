cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(sfml-game)

set(SFML_STATIC_LIBRARIES TRUE)

add_library(tinyXML STATIC
        tinyxml/tinyxml.h
        tinyxml/tinyxml.cpp
        tinyxml/tinyxmlerror.cpp
        tinyxml/tinyxmlparser.cpp
        )

target_compile_definitions(tinyXML
        PRIVATE
        TIXML_USE_STL
        _CRT_SECURE_NO_DEPRECATE
        _CRT_NONSTDC_NO_DEPRECATE
        INTERFACE
        TIXML_USE_STL
        )
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/tinyXML-config-version.cmake
        VERSION 3.8
        COMPATIBILITY AnyNewerVersion
)
install(TARGETS tinyXML EXPORT tinyXML
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(FILES
        tinystr.h
        tinyxml.h
        DESTINATION include)

install(EXPORT tinyXML
        FILE
        tinyXML.cmake
        NAMESPACE
        tinyxml::
        DESTINATION
        lib/cmake/tinyXML
        )
install(
        FILES
        cmake/tinyXML-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/tinyXML-config-version.cmake
        DESTINATION
        lib/cmake/tinyXML
)

export(TARGETS tinyXML
        FILE tinyXML-config.cmake
        NAMESPACE tinyxml::
        )



set(OPENAL_INCLUDE_DIR include)

find_package(Freetype REQUIRED)
find_package(OpenAL REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

add_executable(
                  game game.cpp
                  src/Level/Level.cpp src/Level/Level.h
                  src/Animation/Animation.cpp src/Animation/Animation.hpp
                  src/AnimationManager/AnimationManager.cpp src/AnimationManager/AnimationManager.hpp
                  src/Enemy/Enemy.cpp src/Enemy/Enemy.h
                  src/Player/Player.cpp src/Player/Player.h
                  src/Bullet/Bullet.cpp src/Bullet/Bullet.h
                  src/Clone/Clone.cpp src/Clone/Clone.h
                  src/GameManager/GameManager.cpp src/GameManager/GameManager.h
                  src/Screen/Screen.h
                  src/Menu/Menu.cpp src/Menu/Menu.h
                  src/Game/Game.cpp src/Game/Game.h src/Player/Player.cpp)

target_link_libraries(game ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} tinyXML )