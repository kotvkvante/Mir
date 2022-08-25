# install(
#     DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
#     DESTINATION "source/shaders"
#     FILES_MATCHING PATTERN "*.frag" PATTERN "*.vert"
# )

file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_BINARY_DIR}/source/shaders/
    FILES_MATCHING PATTERN "*.frag" PATTERN "*.vert"
)
