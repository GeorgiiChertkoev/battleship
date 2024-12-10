add_library(game game.cpp game.h)

target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(game PRIVATE field)
target_link_libraries(game PRIVATE strategy)
