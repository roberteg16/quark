# Find Bison
find_package(BISON 3.5.1 REQUIRED)

# Find Flex
find_package(FLEX 2.6.4 REQUIRED)

BISON_TARGET(QuarkParser
             QuarkGrammar.y
             ${CMAKE_CURRENT_BINARY_DIR}/QuarkParser.cpp)

add_library(QuarkFrontend
  SourceModule.cpp
  Expr.cpp
  Decl.cpp
  Stmt.cpp
  Type.cpp
  ${BISON_QuarkParser_OUTPUTS}
)
