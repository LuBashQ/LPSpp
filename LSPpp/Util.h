#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

static std::vector<std::string> tokenize(const std::string str, const std::regex re)
{
	std::sregex_token_iterator it{ str.begin(), str.end(), re, -1 };
	std::vector<std::string> tokenized{ it, {} };

	tokenized.erase(
		std::remove_if(tokenized.begin(),
			tokenized.end(),
			[](std::string const& s) {
				return s.size() == 0;
			}),
		tokenized.end());

	return tokenized;
}

static std::vector<std::vector<std::string>> get_tokens(const std::string& file)
{
	std::ifstream f(file);
	if (f.is_open()) {
		std::vector<std::vector<std::string>> tokens;
		std::string line;
		std::regex re(R"([\s|\n]+)");
		while (getline(f, line))
		{
			std::vector<std::string> tokenized_line = tokenize(line, re);
			tokens.push_back(tokenized_line);
		}
		f.close();
		return tokens;
	}
	else throw std::runtime_error("Could not open requested file");
}