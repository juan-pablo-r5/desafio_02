cmake_minimum_required(VERSION 3.16)

project(spotify LANGUAGES CXX)

# --- Configuración base de Qt ---
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# --- Configuración para SFML ---
# ⚠️ IMPORTANTE: cambia esta ruta según dónde instalaste SFML
# Ejemplo: C:/SFML/lib/cmake/SFML (en Windows)
# o deja la línea comentada si estás en Linux con libsfml-dev
set(SFML_DIR "C:\\Users\\Manuu\\Downloads\\SFML-3.0.2-windows-gcc-14.2.0-mingw-64-bit\\SFML-3.0.2\\lib\\cmake\\SFML")

# Buscar SFML (solo audio y system)
find_package(SFML 3 COMPONENTS Audio System REQUIRED)


# --- Archivos de tu proyecto ---
add_executable(spotify
  main.cpp
  Album.cpp
  Album.h
)

# --- Enlazar Qt + SFML ---
target_link_libraries(spotify PRIVATE Qt${QT_VERSION_MAJOR}::Core winmm)


# --- Instalación (opcional) ---
include(GNUInstallDirs)
install(TARGETS spotify
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
