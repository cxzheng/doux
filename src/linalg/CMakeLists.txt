project(linalg)

hunter_add_package(Eigen)
find_package(Eigen3 CONFIG REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
add_library(doux::linalg ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  INTERFACE
    ${SRC_INC_DIR}
)

target_link_libraries(${PROJECT_NAME} 
  INTERFACE
    Eigen3::Eigen
)

#target_compile_features(${PROJECT_NAME}
#  PUBLIC
#    cxx_std_20
#)
