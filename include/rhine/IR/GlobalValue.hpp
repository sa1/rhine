#ifndef GLOBALVALUE_H
#define GLOBALVALUE_H

#include "rhine/IR/User.hpp"
#include "rhine/Diagnostic/Diagnostic.hpp"

namespace rhine {
class GlobalString : public User {
  std::string Val;
public:
  GlobalString(std::string Val, Context *K);
  virtual ~GlobalString();
  void *operator new(size_t s);
  static GlobalString *get(std::string Val, Context *K);
  static bool classof(const Value *V);
  std::string getVal();
  virtual llvm::Value *toLL(llvm::Module *M) override;
protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
};
}

#endif
