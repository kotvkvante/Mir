option(WITH_GLAD "Use glad" ON)
option(WITH_STB "Use Stb" ON)

target_sources(Mir PRIVATE
    main.c
)

add_subdirectory(ai)

add_subdirectory(extern)

add_subdirectory(graphics)

add_subdirectory(gui)

add_subdirectory(kernel)

add_subdirectory(math)

add_subdirectory(mir)

add_subdirectory(player)

add_subdirectory(shaders)

add_subdirectory(utils)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

target_include_directories(Mir PUBLIC
    ${PROJECT_BINARY_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)

target_link_libraries(Mir PUBLIC
    ${EXTRA_LIBS}
    ${FREETYPE_LIBRARIES}
    m
    glad
    OpenGL::GL
    glfw
)
