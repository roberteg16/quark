add_executable(quark main.cpp)

llvm_map_components_to_libnames(llvm_libs support)

target_link_libraries(quark
    QuarkFrontend
)

target_link_libraries(quark ${llvm_libs})
