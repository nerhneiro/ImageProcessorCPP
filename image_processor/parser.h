//
// Created by I on 24.03.2023.
//
#pragma once
#include <iostream>
#include <vector>

struct FilterWithArguments {
    explicit FilterWithArguments(std::string_view name) : name_(name) {
    }
    std::string_view name_;
    std::vector<char*> params_;
};
enum ParsingErrors { no_input, not_enough_arguments, wrong_arguments_input, no_errors };

class Parser {
public:
    ParsingErrors Parse(int argc, char** argv);
    const char* const FilenameIn() const {
        return filename_in_;
    }
    char* FilenameOut() const {
        return filename_out_;
    }
    std::vector<FilterWithArguments> Filters() {
        return filters_;
    }

private:
    std::vector<FilterWithArguments> filters_;
    char* filename_in_;
    char* filename_out_;
};
