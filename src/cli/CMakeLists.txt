add_executable(kate-syntax-highlighter kate-syntax-highlighter.cpp)
target_link_libraries(kate-syntax-highlighter KF5SyntaxHighlighting)

install(TARGETS kate-syntax-highlighter ${INSTALL_TARGETS_DEFAULT_ARGS})
