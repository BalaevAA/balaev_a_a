project(K)
add_executable(K K.cpp task.report.md.in.txt)
target_link_libraries(K ${OpenCV_LIBS})

file(READ K.cpp cpp_source)
# prepare report md file
configure_file(task.report.md.in.txt ${CMAKE_CURRENT_BINARY_DIR}/task.report.md @ONLY)
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/task.report.md" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")