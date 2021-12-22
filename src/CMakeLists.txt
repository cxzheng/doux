hunter_add_package(Eigen)
find_package(Eigen3 CONFIG REQUIRED)

add_subdirectory(core)
add_subdirectory(linalg)
add_subdirectory(pd)
