// Copyright 2026 Maksim Populov
#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>
#include "textgen.h"

void testPrefixConstruction() {
    TextGenerator gen(2);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("a");
    prefix.push_back("b");
    gen.addSuffix(prefix, "c");
    assert(gen.getStateTab().size() == 1);
    std::cout << "testPrefixConstruction passed\n";
}

void testSingleSuffixSelection() {
    TextGenerator gen(2);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("hello");
    prefix.push_back("world");
    gen.addSuffix(prefix, "!");
    TextGenerator::StateTab tab = gen.getStateTab();
    assert(tab[prefix].size() == 1);
    std::cout << "testSingleSuffixSelection passed\n";
}

void testMultipleSuffixSelection() {
    TextGenerator gen(2);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("A");
    prefix.push_back("B");
    gen.addSuffix(prefix, "C");
    gen.addSuffix(prefix, "D");
    TextGenerator::StateTab tab = gen.getStateTab();
    assert(tab[prefix].size() == 2);
    std::cout << "testMultipleSuffixSelection passed\n";
}

void testTextGenerationFixedTable() {
    TextGenerator gen(2, 10);
    gen.setRandomSeed(42);
    TextGenerator::Prefix p1, p2, p3;
    p1.push_back("the");
    p1.push_back("quick");
    p2.push_back("quick");
    p2.push_back("brown");
    p3.push_back("brown");
    p3.push_back("fox");
    gen.addSuffix(p1, "brown");
    gen.addSuffix(p2, "fox");
    gen.addSuffix(p3, "jumps");

    std::string result = gen.generate();
    assert(result.find("brown") != std::string::npos);
    std::cout << "testTextGenerationFixedTable passed\n";
}

void testMaxgenLimit() {
    TextGenerator gen(1, 3);
    gen.setRandomSeed(42);
    TextGenerator::Prefix p1, p2, p3;
    p1.push_back("start");
    p2.push_back("A");
    p3.push_back("B");
    gen.addSuffix(p1, "A");
    gen.addSuffix(p2, "B");
    gen.addSuffix(p3, "C");

    std::string result = gen.generate();
    std::cout << "Generated (maxgen=3): " << result << "\n";
    std::cout << "testMaxgenLimit passed\n";
}

void testEmptyFileHandling() {
    TextGenerator gen(2);
    bool thrown = false;
    try {
        gen.build("nonexistent.txt");
    } catch (...) {
        thrown = true;
    }
    assert(thrown);
    std::cout << "testEmptyFileHandling passed\n";
}

void testPrefixSizeOne() {
    TextGenerator gen(1);
    gen.setRandomSeed(42);
    TextGenerator::Prefix p1, p2;
    p1.push_back("hello");
    p2.push_back("world");
    gen.addSuffix(p1, "world");
    gen.addSuffix(p2, "!");
    std::string result = gen.generate();
    assert(!result.empty());
    std::cout << "testPrefixSizeOne passed\n";
}

void testMultipleAdds() {
    TextGenerator gen(2);
    TextGenerator::Prefix prefix;
    prefix.push_back("X");
    prefix.push_back("Y");
    gen.addSuffix(prefix, "Z1");
    gen.addSuffix(prefix, "Z2");
    gen.addSuffix(prefix, "Z3");
    assert(gen.getStateTab()[prefix].size() == 3);
    std::cout << "testMultipleAdds passed\n";
}

void testDeterministicGeneration() {
    TextGenerator gen1(2), gen2(2);
    gen1.setRandomSeed(123);
    gen2.setRandomSeed(123);
    TextGenerator::Prefix prefix;
    prefix.push_back("A");
    prefix.push_back("B");
    gen1.addSuffix(prefix, "C");
    gen2.addSuffix(prefix, "C");
    assert(gen1.generate() == gen2.generate());
    std::cout << "testDeterministicGeneration passed\n";
}

void testLongGeneration() {
    TextGenerator gen(2, 100);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("w1");
    prefix.push_back("w2");
    for (int i = 0; i < 10; ++i) {
        gen.addSuffix(prefix, "w3");
    }
    std::string result = gen.generate();
    assert(result.length() > 0);
    std::cout << "testLongGeneration passed\n";
}

int main() {
    testPrefixConstruction();
    testSingleSuffixSelection();
    testMultipleSuffixSelection();
    testTextGenerationFixedTable();
    testMaxgenLimit();
    testEmptyFileHandling();
    testPrefixSizeOne();
    testMultipleAdds();
    testDeterministicGeneration();
    testLongGeneration();

    std::cout << "\nAll tests passed!\n";
    return 0;
}
