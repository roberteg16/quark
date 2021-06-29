#include <llvm-12/llvm/Support/Debug.h>
#include <quark/Frontend/LexContext.h>
#include <quark/Frontend/SourceModule.h>

#include "../../build/lib/Frontend/QuarkParser.hpp"
#include <llvm/Support/InitLLVM.h>

#include <fstream>

using namespace quark;

void quark::QuarkParser::error(const std::string &error) {
  llvm::outs() << "Error: " << error << "\n";
}

static quark::SourceModule Parse() {
  quark::LexContext ctx;
  quark::SourceModule sm;

  quark::QuarkParser quarkParser(ctx, sm);
  quarkParser.parse();

  return sm;
}

int main(int argc, const char *argv[]) {
  llvm::InitLLVM llvm(argc, argv);

  llvm::StringRef file = argv[1];
  std::ifstream f(file.data());

  freopen(file.data(), "r", stdin);

  SourceModule sm = Parse();
  sm.dump();

  return 0;
}