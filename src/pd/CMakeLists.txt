project(pd)

add_library(${PROJECT_NAME} OBJECT
  softbody.cpp      constraint.cpp
)
add_library(doux::pd ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${SRC_INC_DIR}
)

target_link_libraries(${PROJECT_NAME} 
  PUBLIC 
    doux::core 
)

target_compile_features(${PROJECT_NAME}
  PUBLIC
    cxx_std_20
)