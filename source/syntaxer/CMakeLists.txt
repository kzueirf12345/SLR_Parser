add_library(syntaxer STATIC
    src/Grammar.cpp
    src/ParsingTable.cpp
    src/Syntaxer.cpp
)

target_include_directories(syntaxer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(syntaxer PRIVATE project_warnings)
if(TARGET project_sanitizers)
    target_link_libraries(syntaxer PRIVATE project_sanitizers)
endif()

target_link_libraries(syntaxer PRIVATE 
    lexer
    utils
)

set_target_properties(syntaxer PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)