find_package(LLVM REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_BINARY_DIR}/src/Parse")
include_directories("${LLVM_INCLUDE_DIRS}")

add_library(Toplevel
  "${CMAKE_CURRENT_SOURCE_DIR}/ParseFacade.cpp")
set_target_properties(Toplevel PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LLVM_CXXFLAGS}")
target_link_libraries(Toplevel
  LLVMBitWriter
  Parse
  Transform
  CodeGen)
