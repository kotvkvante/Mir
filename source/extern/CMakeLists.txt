if(WITH_GLAD)
    add_subdirectory(glad)

    target_link_libraries(Mir PUBLIC glad)
endif()

if(WITH_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory(glfw)
    
    target_link_libraries(Mir PUBLIC glfw)
endif()

if(WITH_STB)
    add_subdirectory(stb)

endif()
