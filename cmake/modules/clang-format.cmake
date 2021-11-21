# A CMake script to find all source files and setup clang-format targets for them

get_filename_component(_clangcheckpath ${CMAKE_CURRENT_LIST_FILE} PATH)

foreach(DIR ${CLANG_FORMAT_DIRECTORIES})
    list(APPEND INC_DIR_ARGS "--include=${DIR}")
endforeach()

# call the script to check changed files in git
add_custom_target(format
    COMMENT "Clang format source files"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${_clangcheckpath}/../scripts/run-clang-format.py 
    --file-ext \"${CLANG_FORMAT_FILE_EXTENSIONS}\"
    ${INC_DIR_ARGS}
    --clang-format-bin ${CLANG_FORMAT_EXECUTABLE}
)
