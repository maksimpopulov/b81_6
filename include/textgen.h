// Copyright 2026 Maksim Populov
#ifndef INCLUDE_TEXTGEN_H_
#define INCLUDE_TEXTGEN_H_

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <random>

class TextGenerator {
 public:
  using Prefix = std::deque<std::string>;
  using StateTab = std::map<Prefix, std::vector<std::string> >;

  explicit TextGenerator(int npref = 2, int maxgen = 1000);

  void build(const std::string& filename);
  std::string generate();

  StateTab getStateTab() const { return statetab; }
  void addSuffix(const Prefix& p, const std::string& suffix);
  void setRandomSeed(unsigned seed);
  void clear() { statetab.clear(); }

 private:
  int NPREF;
  int MAXGEN;
  StateTab statetab;
  std::mt19937 rng;

  std::string randomSuffix(const std::vector<std::string>& suffixes);
};

#endif  // INCLUDE_TEXTGEN_H_
