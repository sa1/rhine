#include "rhine/IR/GlobalValue.h"
#include "rhine/IR/Type.h"

namespace rhine {
GlobalString::GlobalString(std::string Val, Context *K) :
    User(StringType::get(K), RT_GlobalString), Val(Val) {}

GlobalString::~GlobalString() {}

void *GlobalString::operator new(size_t s) {
  return User::operator new (s);
}

GlobalString *GlobalString::get(std::string Val, Context *K) {
  return new GlobalString(Val, K);
}

bool GlobalString::classof(const Value *V) {
  return V->getValID() == RT_GlobalString;
}

std::string GlobalString::getVal() {
  return Val;
}

void GlobalString::print(std::ostream &Stream) const {
  Stream << "'" << Val << "' ~" << *getType();
}
}
