project(core)

add_library(${PROJECT_NAME} OBJECT
  logger.cpp)
add_library(doux::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${SRC_INC_DIR}
)
target_link_libraries(${PROJECT_NAME} 
  PUBLIC
    spdlog::spdlog
    fmt::fmt
)
target_compile_features(${PROJECT_NAME}
  PUBLIC
    cxx_constexpr           cxx_noexcept
    cxx_variable_templates
    cxx_std_20
)
