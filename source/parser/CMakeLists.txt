add_library(parser STATIC)

target_include_directories(parser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(parser PRIVATE project_warnings)
if(TARGET project_sanitizers)
    target_link_libraries(parser PRIVATE project_sanitizers)
endif()

set_target_properties(parser PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# ==========================================
# FLEX CONFIGURATION
# ==========================================

FLEX_TARGET(Lexer 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.l 
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
    COMPILE_FLAGS "--c++"
)

target_sources(parser
    PRIVATE
        ${FLEX_Lexer_OUTPUTS}
)

target_sources(parser
    PRIVATE
        ${FLEX_Lexer_OUTPUTS}
)