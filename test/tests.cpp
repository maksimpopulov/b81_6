// Copyright 2026 Maksim Populov

#include <gtest/gtest.h>
#include <fstream>
#include <random>
#include <string>
#include <deque>
#include <set>
#include <sstream>
#include "textgen.h"

static void writeTempFile(const std::string& content, 
                         const std::string& filename = "temp_test.txt") {
    std::ofstream f(filename);
    f << content;
    f.close();
}

TEST(TextGen, PrefixConstruction) {
    TextGenerator gen(2);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("a");
    prefix.push_back("b");
    gen.addSuffix(prefix, "c");
    EXPECT_EQ(gen.getStateTab().size(), 1);
}

TEST(TextGen, SingleSuffixSelection) {
    TextGenerator gen(2);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("hello");
    prefix.push_back("world");
    gen.addSuffix(prefix, "!");
    TextGenerator::StateTab tab = gen.getStateTab();
    EXPECT_EQ(tab[prefix].size(), 1);
}

TEST(TextGen, MultipleSuffixSelection) {
    TextGenerator gen(2);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("A");
    prefix.push_back("B");
    gen.addSuffix(prefix, "C");
    gen.addSuffix(prefix, "D");
    TextGenerator::StateTab tab = gen.getStateTab();
    EXPECT_EQ(tab[prefix].size(), 2);
}

TEST(TextGen, TextGenerationFixedTable) {
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
    EXPECT_NE(result.find("brown"), std::string::npos);
}

TEST(TextGen, MaxgenLimit) {
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
    // Test passes if no exception is thrown
    SUCCEED();
}

TEST(TextGen, EmptyFileHandling) {
    TextGenerator gen(2);
    EXPECT_THROW(gen.build("nonexistent.txt"), std::runtime_error);
}

TEST(TextGen, PrefixSizeOne) {
    TextGenerator gen(1);
    gen.setRandomSeed(42);
    TextGenerator::Prefix p1, p2;
    p1.push_back("hello");
    p2.push_back("world");
    gen.addSuffix(p1, "world");
    gen.addSuffix(p2, "!");
    std::string result = gen.generate();
    EXPECT_FALSE(result.empty());
}

TEST(TextGen, MultipleAdds) {
    TextGenerator gen(2);
    TextGenerator::Prefix prefix;
    prefix.push_back("X");
    prefix.push_back("Y");
    gen.addSuffix(prefix, "Z1");
    gen.addSuffix(prefix, "Z2");
    gen.addSuffix(prefix, "Z3");
    EXPECT_EQ(gen.getStateTab()[prefix].size(), 3);
}

TEST(TextGen, DeterministicGeneration) {
    TextGenerator gen1(2), gen2(2);
    gen1.setRandomSeed(123);
    gen2.setRandomSeed(123);
    TextGenerator::Prefix prefix;
    prefix.push_back("A");
    prefix.push_back("B");
    gen1.addSuffix(prefix, "C");
    gen2.addSuffix(prefix, "C");
    EXPECT_EQ(gen1.generate(), gen2.generate());
}

TEST(TextGen, LongGeneration) {
    TextGenerator gen(2, 100);
    gen.setRandomSeed(42);
    TextGenerator::Prefix prefix;
    prefix.push_back("w1");
    prefix.push_back("w2");
    for (int i = 0; i < 10; ++i) {
        gen.addSuffix(prefix, "w3");
    }
    std::string result = gen.generate();
    EXPECT_GT(result.length(), 0);
}

TEST(TextGen, CorrectNumberOfWords) {
    TextGenerator gen(3, 10);
    writeTempFile("a b c d e f");
    gen.build("temp_test.txt");
    std::string result = gen.generate();
    std::stringstream ss(result);
    std::string w;
    int wordCount = 0;
    while (ss >> w) ++wordCount;
    EXPECT_GE(wordCount, 3);
    std::remove("temp_test.txt");
}

TEST(TextGen, SingleMapping) {
    TextGenerator gen(2, 10);
    std::deque<std::string> prefix = {"hello", "world"};
    gen.addSuffix(prefix, "test");
    auto suffixes = gen.getStateTab()[prefix];
    ASSERT_EQ(suffixes.size(), 1);
    EXPECT_EQ(suffixes[0], "test");
}

TEST(TextGen, MultipleSuffixesRandom) {
    TextGenerator gen(2, 10);
    std::deque<std::string> prefix = {"color", "is"};
    gen.addSuffix(prefix, "red");
    gen.addSuffix(prefix, "green");
    gen.addSuffix(prefix, "blue");
    std::set<std::string> seen;
    for (int i = 0; i < 50; ++i) {
        std::string result = gen.generate();
        // Just verify generation works with multiple suffixes
        EXPECT_FALSE(result.empty());
    }
}

TEST(TextGen, FixedLengthFromManualTable) {
    TextGenerator gen(2, 5);
    std::deque<std::string> p1 = {"I", "like"};
    std::deque<std::string> p2 = {"like", "to"};
    std::deque<std::string> p3 = {"to", "eat"};
    std::deque<std::string> p4 = {"eat", "apples"};
    gen.addSuffix(p1, "to");
    gen.addSuffix(p2, "eat");
    gen.addSuffix(p3, "apples");
    gen.addSuffix(p4, ".");
    std::string result = gen.generate();
    std::stringstream ss(result);
    int wordCount = 0;
    std::string word;
    while (ss >> word) ++wordCount;
    EXPECT_GE(wordCount, 5);
    EXPECT_LE(wordCount, 7);
}

TEST(TextGen, TextShorterThanPrefix) {
    TextGenerator gen(3, 10);
    writeTempFile("only two words");
    EXPECT_THROW(gen.build("temp_test.txt"), std::runtime_error);
    std::remove("temp_test.txt");
}

TEST(TextGen, NoInfiniteLoop) {
    TextGenerator gen(1, 100);
    std::deque<std::string> a = {"a"};
    std::deque<std::string> b = {"b"};
    gen.addSuffix(a, "b");
    gen.addSuffix(b, "a");
    std::string result = gen.generate();
    std::stringstream ss(result);
    int cnt = 0;
    std::string w;
    while (ss >> w) ++cnt;
    EXPECT_LE(cnt, 1 + 100);
}

TEST(TextGen, DistributionOfSuffixes) {
    TextGenerator gen(2, 100);
    std::deque<std::string> prefix = {"A", "B"};
    gen.addSuffix(prefix, "X");
    gen.addSuffix(prefix, "Y");
    int countX = 0, countY = 0;
    const int trials = 200;
    for (int i = 0; i < trials; ++i) {
        std::string result = gen.generate();
        if (result.find("X") != std::string::npos) ++countX;
        else if (result.find("Y") != std::string::npos) ++countY;
    }
    EXPECT_GT(countX, 10);
    EXPECT_GT(countY, 10);
}

TEST(TextGen, MaxWordLimit) {
    TextGenerator gen(2, 5);
    writeTempFile("one two three four five six seven eight nine ten");
    gen.build("temp_test.txt");
    std::string result = gen.generate();
    std::stringstream ss(result);
    int wordCount = 0;
    std::string w;
    while (ss >> w) ++wordCount;
    EXPECT_LE(wordCount, 2 + 5);
    std::remove("temp_test.txt");
}

TEST(TextGen, ClearTableResets) {
    TextGenerator gen(2, 10);
    std::deque<std::string> prefix = {"test", "prefix"};
    gen.addSuffix(prefix, "suffix");
    EXPECT_EQ(gen.getStateTab().size(), 1);
    gen.clear();
    EXPECT_EQ(gen.getStateTab().size(), 0);
}
