add_library(strategy strategy.cpp strategy.h)

target_include_directories(strategy PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(strategy PRIVATE field)
