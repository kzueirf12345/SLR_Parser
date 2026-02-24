add_library(utils STATIC
    src/common.cpp
)

target_include_directories(utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(utils PRIVATE project_warnings)

if(TARGET project_sanitizers)
    target_link_libraries(utils PRIVATE project_sanitizers)
endif()

set_target_properties(utils PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)