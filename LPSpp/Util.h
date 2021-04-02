#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

static std::vector<std::string> tokenize(const std::string str) {
    std::regex no_comments(R"([;]+)");
    std::regex split(R"([\s|\n]+)");

    std::sregex_token_iterator pre{str.begin(), str.end(), no_comments, -1};
    std::vector<std::string> pre_tokenized{pre, {}};

    std::sregex_token_iterator post{pre_tokenized[0].begin(), pre_tokenized[0].end(), split, -1};
    std::vector<std::string> tokenized{post, {}};

    tokenized.erase(
            std::remove_if(tokenized.begin(),
                           tokenized.end(),
                           [](std::string const &s) {
                               return s.size() == 0;
                           }),
            tokenized.end());

    return tokenized;
}

static std::vector<std::vector<std::string>> get_tokens(const std::string &file) {
    std::ifstream f(file);
    if (f.is_open()) {
        std::vector<std::vector<std::string>> tokens;
        std::string line;
        while (getline(f, line)) {
            std::vector<std::string> tokenized_line = tokenize(line);
            tokens.push_back(tokenized_line);
        }
        f.close();
        return tokens;
    } else throw std::runtime_error("Could not open requested file");
}