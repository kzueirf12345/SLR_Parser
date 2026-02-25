add_library(args STATIC
    src/Args.cpp
    src/IOManager.cpp
)

target_include_directories(args
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(args PRIVATE project_warnings)
if(TARGET project_sanitizers)
    target_link_libraries(args PRIVATE project_sanitizers)
endif()

target_link_libraries(args PRIVATE
    utils
)

set_target_properties(args PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)