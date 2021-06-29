# Find Bison
find_package(BISON 3.5.1 REQUIRED)

# Find Flex
find_package(FLEX 2.6.4 REQUIRED)

BISON_TARGET(QuarkParser
             QuarkGrammar.y
             ${CMAKE_CURRENT_BINARY_DIR}/QuarkParser.cpp)

FLEX_TARGET(QuarkLexer
             QuarkLex.l
             ${CMAKE_CURRENT_BINARY_DIR}/QuarkLexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(QuarkLexer QuarkParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(QuarkFrontend
  Expr.cpp
  Decl.cpp
  Stmt.cpp
  Type.cpp
  SourceModule.cpp
  ${BISON_QuarkParser_OUTPUTS}
  ${FLEX_QuarkLexer_OUTPUTS}
)
