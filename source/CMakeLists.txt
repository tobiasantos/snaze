cmake_minimum_required(VERSION 3.5)
project(Snaze VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall")

# Lista de arquivos de origem
# Configurar o diretório onde estão os arquivos-fonte do projeto
set(SOURCES
    level.cpp
    main.cpp
    player.cpp
    simulation.cpp
    snake.cpp
)

# Adiciona um executável
add_executable(snaze ${SOURCES})