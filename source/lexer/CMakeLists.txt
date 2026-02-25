add_library(lexer STATIC
    src/Lexer.cpp
)

target_include_directories(lexer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(lexer PRIVATE project_warnings)
if(TARGET project_sanitizers)
    target_link_libraries(lexer PRIVATE project_sanitizers)
endif()

set_target_properties(lexer PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

#===============================================FLEX================================================

FLEX_TARGET(Lexer 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.l 
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
    COMPILE_FLAGS "--c++"
)

target_sources(lexer
    PRIVATE
        ${FLEX_Lexer_OUTPUTS}
)