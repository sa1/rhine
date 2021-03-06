//-*- C++ -*-
#ifndef RHINE_LAMBDALIFTING_H
#define RHINE_LAMBDALIFTING_H

#include "rhine/Pass/FunctionPass.hpp"

namespace rhine {
class Function;

class LambdaLifting : public FunctionPass {
public:
  virtual ~LambdaLifting() {}
  void runOnFunction(Function *F) override;
};
}

#endif
