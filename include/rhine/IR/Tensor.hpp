#ifndef RHINE_TENSOR_H
#define RHINE_TENSOR_H

#include <cstdlib>
#include <vector>

#include "rhine/IR/Value.hpp"

/// Forward declaration
namespace llvm {
  class Value;
}

namespace rhine {
class Tensor : public Value {
  std::vector<size_t> Dimensions;
  std::vector<Value *> Elements;

public:
  /// A tensor is a finite, non-zero list of entries. Dims = { M, N, P }, and
  /// Elts = { ... MxNxP elements ... } for a MxNxP tensor, stored in row-major
  /// order.
  Tensor(std::vector<size_t> Dims, std::vector<Value *> Elts);

  /// Destructor is a no-op
  virtual ~Tensor();

  /// Invokes sanity checks on Dims and Elts before calling constructor
  static Tensor *get(std::vector<size_t> Dims, std::vector<Value *> Elts);

  /// RT_Tensor
  static bool classof(const Value *V);

  /// Intricate malloc + gep operations
  virtual llvm::Value *toLL(llvm::Module *M) override;

  /// Printed like Lua tables, { ... }
  virtual void print(DiagnosticPrinter &Stream) const override;
};
}

#endif