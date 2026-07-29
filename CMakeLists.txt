cmake_minimum_required(VERSION 3.12)
project(MyGame VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- GLAD Generation ---

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(GLAD_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/glad_generated")
set(GLAD_SRC "${GLAD_GENERATED_DIR}/src/gl.c")
set(GLAD_INCLUDE_DIR "${GLAD_GENERATED_DIR}/include")

add_custom_command(
    OUTPUT "${GLAD_SRC}"
    COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/external/glad"
            "${Python3_EXECUTABLE}" -m glad
            --api "gl:core=4.6"
            --out-path "${GLAD_GENERATED_DIR}"
            c
    COMMENT "Generating GLAD 2 source directory..."
    VERBATIM
)

add_custom_target(glad_generate
    DEPENDS "${GLAD_SRC}"
)

add_library(glad STATIC "${GLAD_SRC}")
add_dependencies(glad glad_generate)

target_include_directories(glad PUBLIC "${GLAD_INCLUDE_DIR}")
target_compile_definitions(glad PRIVATE GLAD_API_EXPORT)

# --- External Dependencies ---

add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/entt)

# --- Main Project ---

file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" "src/*.c")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    external/glm
    external/entt/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    glm::glm
    EnTT::EnTT
)

# --- SPIR-V Shader Compile for OpenGL ---

find_program(GLSLC_COMPILER glslc)
if(NOT GLSLC_COMPILER)
    message(FATAL_ERROR "glslc not found! Please install shaderc.")
endif()

set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders")
set(SHADER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(MAKE_DIRECTORY "${SHADER_BIN_DIR}")
file(MAKE_DIRECTORY "${SHADER_BIN_DIR}/cache")

file(GLOB_RECURSE SHADER_SOURCES
    "${SHADER_SRC_DIR}/*.vert"
    "${SHADER_SRC_DIR}/*.frag"
    "${SHADER_SRC_DIR}/*.geom"
)

set(SPV_OUTPUTS "")
foreach(SHADER_FILE ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)

    set(SPV_FILE "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
    list(APPEND SPV_OUTPUTS ${SPV_FILE})

    add_custom_command(
        OUTPUT ${SPV_FILE}
        COMMAND ${GLSLC_COMPILER} ${SHADER_FILE} -o ${SPV_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling GLSL to SPIR-V via glslc: ${SHADER_NAME}"
    )
endforeach()

add_custom_target(CompileShaders DEPENDS ${SPV_OUTPUTS})
add_dependencies(${PROJECT_NAME} CompileShaders)