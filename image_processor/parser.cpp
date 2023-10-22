//
// Created by I on 24.03.2023.
//
#include "parser.h"
ParsingErrors Parser::Parse(int argc, char **argv) {
    if (argc == 1) {
        return ParsingErrors::no_input;
    } else if (argc <= 2) {
        return ParsingErrors::not_enough_arguments;
    }
    filename_in_ = argv[1];
    filename_out_ = argv[2];
    for (size_t i = 3; i < argc; ++i) {
        if (argv[i][0] == '-') {
            filters_.push_back(FilterWithArguments(argv[i]));
        } else if (!filters_.empty()) {
            filters_[filters_.size() - 1].params_.push_back(argv[i]);
        } else {
            return ParsingErrors::wrong_arguments_input;
        }
    }
    return ParsingErrors::no_errors;
}