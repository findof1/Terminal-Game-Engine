project(EngineCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/EngineCore/src/*.cpp")

add_library(EngineCore SHARED ${SOURCES})

target_include_directories(EngineCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(EngineCore PRIVATE ENGINECORE_EXPORTS)