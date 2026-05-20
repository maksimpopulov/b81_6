// Copyright 2026 Maksim Populov

#include <algorithm>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#include "textgen.h"


TextGenerator::TextGenerator(int npref, int maxgen)
    :NPREF(npref), MAXGEN(maxgen), rng(std::random_device {}()) {
}

void TextGenerator::setRandomSeed(unsigned seed) {
    rng.seed(seed);
}

void TextGenerator::addSuffix(const Prefix& p, const std::string& suffix) {
    statetab[p].push_back(suffix);
}

void TextGenerator::build(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string word;
    Prefix prefix;

    for (int i = 0; i < NPREF && (file >> word); ++i) {
        prefix.push_back(word);
    }

    if (prefix.size() < static_cast<size_t>(NPREF)) {
        throw std::runtime_error(
            "File too short to build prefix table");
    }

    while (file >> word) {
        statetab[prefix].push_back(word);
        prefix.pop_front();
        prefix.push_back(word);
    }

    statetab[prefix].push_back("");
}

std::string TextGenerator::randomSuffix(
    const std::vector<std::string>& suffixes) {
    if (suffixes.empty()) return "";
    std::uniform_int_distribution<size_t> dist(0, suffixes.size() - 1);
    return suffixes[dist(rng)];
}

std::string TextGenerator::generate() {
    if (statetab.empty()) {
        return "";
    }

    Prefix current = statetab.begin()->first;
    std::vector<std::string> result(current.begin(), current.end());

    for (int i = 0; i < MAXGEN; ++i) {
        StateTab::const_iterator it = statetab.find(current);
        if (it == statetab.end() || it->second.empty()) break;

        std::string next = randomSuffix(it->second);
        if (next.empty()) break;

        result.push_back(next);
        current.pop_front();
        current.push_back(next);
    }

    std::string output;
    for (std::vector<std::string>::const_iterator it = result.begin();
         it != result.end(); ++it) {
        output += *it + " ";
    }
    if (!output.empty()) output.pop_back();
    return output;
}
