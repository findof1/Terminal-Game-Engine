project(EngineGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/EngineGUI/src/*.cpp")

file(GLOB IMGUI_FILES 
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/backends/imgui_impl_glfw.cpp"
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/backends/imgui_impl_opengl3.cpp"
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/imgui.cpp"
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/imgui_draw.cpp"
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/imgui_tables.cpp"
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/imgui_widgets.cpp"
"${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/misc/cpp/imgui_stdlib.cpp"
)

file(GLOB IMGUI_NODES_FILES "${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui_nodes/*.cpp")


add_executable(EngineGUI ${SOURCES} ${IMGUI_FILES} ${IMGUI_NODES_FILES})

target_include_directories(EngineGUI PRIVATE
    ${CMAKE_SOURCE_DIR}/EngineCore/include
    ${CMAKE_SOURCE_DIR}/EngineGUI/include
    ${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui_nodes
    ${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui
    ${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/backends
    ${CMAKE_SOURCE_DIR}/EngineGUI/External/imgui/misc/cpp
    C:/msys64/mingw64/include
    C:/msys64/mingw64/SDL3/include
)

target_link_directories(EngineGUI PRIVATE
    C:/msys64/mingw64/lib
)

target_link_libraries(EngineGUI 
    PRIVATE EngineCore
    opengl32
    glfw3
)

add_custom_command(TARGET EngineGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:EngineCore>
        $<TARGET_FILE_DIR:EngineGUI>
)