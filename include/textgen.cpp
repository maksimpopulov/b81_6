#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <random>

class TextGenerator {
public:
    using Prefix = std::deque<std::string>;
    using StateTab = std::map<Prefix, std::vector<std::string>>;

    TextGenerator(int npref = 2, int maxgen = 1000);

    void build(const std::string& filename);

    std::string generate();

    StateTab getStateTab() const { return statetab; }
    void addSuffix(const Prefix& p, const std::string& suffix);
    void setRandomSeed(unsigned seed);

private:
    int NPREF;
    int MAXGEN;
    StateTab statetab;
    std::mt19937 rng;

    std::string randomSuffix(const std::vector<std::string>& suffixes);
};

#endif
