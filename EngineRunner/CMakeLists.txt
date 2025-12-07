project(EngineRunner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/EngineRunner/src/*.cpp")

add_executable(EngineRunner ${SOURCES})

target_include_directories(EngineRunner PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/EngineCore/include
)

target_compile_definitions(EngineRunner PRIVATE ENGINECORE_EXPORTS)

target_link_libraries(EngineRunner 
    PRIVATE EngineCore
)

add_custom_command(TARGET EngineRunner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:EngineCore>
        $<TARGET_FILE_DIR:EngineRunner>
)