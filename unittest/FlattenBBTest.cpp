#include "rhine/Util/TestUtil.h"
#include "gtest/gtest.h"

#include "rhine/IR/Module.h"
#include "rhine/Transform/LambdaLifting.h"
#include "rhine/Transform/FlattenBB.h"

using namespace rhine;

TEST(FlattenBB, NumberOfBBs)
{
  std::string SourcePrg =
    "def main [] {\n"
    "  if(false) 3; else 4;\n"
    "}";
  auto Pf = ParseFacade(SourcePrg);
  FlattenBB Flatten;
  auto Mod = Pf.parseToIR(ParseSource::STRING, { &Flatten });
  auto MainF = Mod->front();
  auto NumberOfBBs = std::distance(MainF->begin(), MainF->end());
  ASSERT_EQ(NumberOfBBs, 4);
}

TEST(FlattenBB, PredSucc)
{
  std::string SourcePrg =
    "def main [] {\n"
    "  if(false) 3; else 4;\n"
    "}";
  auto Pf = ParseFacade(SourcePrg);
  FlattenBB Flatten;
  auto Mod = Pf.parseToIR(ParseSource::STRING, { &Flatten });
  auto MainF = Mod->front();
  std::vector<int> NumPreds = {0, 1, 1, 2}, NumSuccs = {2, 1, 1, 0};
  auto NumPredsIt = NumPreds.begin();
  auto NumSuccsIt = NumSuccs.begin();
  for (auto BB : *MainF) {
    ASSERT_EQ(std::distance(BB->pred_begin(), BB->pred_end()), *NumPredsIt++);
    ASSERT_EQ(std::distance(BB->succ_begin(), BB->succ_end()), *NumSuccsIt++);
  }
}

TEST(FlattenBB, SetParent)
{
  std::string SourcePrg =
    "def main [] {\n"
    "  ret 4;\n"
    "}";
  auto Pf = ParseFacade(SourcePrg);
  FlattenBB Flatten;
  auto Mod = Pf.parseToIR(ParseSource::STRING, { &Flatten });
  auto MainF = Mod->front();
  auto EntryBlock = MainF->getEntryBlock();
  ASSERT_EQ(EntryBlock->getParent(), MainF);
}

TEST(FlattenBB, SetIfParent)
{
  std::string SourcePrg =
    "def main [] {\n"
    "  if(false) 3; else 4;\n"
    "}";
  auto Pf = ParseFacade(SourcePrg);
  FlattenBB Flatten;
  auto Mod = Pf.parseToIR(ParseSource::STRING, { &Flatten });
  auto MainF = Mod->front();
  for (auto BB : *MainF) {
    ASSERT_EQ(BB->getParent(), MainF);
  }
}

TEST(FlattenBB, SetLambdaParent)
{
  std::string SourcePrg =
    "def foo [] Bfunc = \\x ~Int -> ret x;";
  auto Pf = ParseFacade(SourcePrg);
  LambdaLifting LambLift;
  FlattenBB Flatten;
  auto Mod = Pf.parseToIR(ParseSource::STRING, { &LambLift, &Flatten });
  for (auto F : *Mod)
    for (auto BB : *F)
      ASSERT_EQ(BB->getParent(), F);
}