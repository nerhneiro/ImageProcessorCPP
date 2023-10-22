#include "image_processor.h"
#include "grayscale_filter.h"
#include "image.h"
#include "parser.h"

int main(int argc, char** argv) {
    Parser line_parser;
    ParsingErrors result = line_parser.Parse(argc, argv);
    if (result == ParsingErrors::not_enough_arguments) {
        std::cout << "Your input is incorrect: it's too short. It must have at least three arguments." << std::endl;
    } else if (result == ParsingErrors::no_input) {
        std::cout << "Тут будет справка" << std::endl;
    } else if (result == ParsingErrors::wrong_arguments_input) {
        std::cout << "Your input is incorrect" << std::endl;
    }
    Image image;
    image.Read(line_parser.FilenameIn());
    for (FilterWithArguments filter_struct : line_parser.Filters()) {
        if (filter_struct.name_ == "-crop") {
            if (filter_struct.params_.size() != 2) {
                std::cout << "Crop filter requires 2 parameters (width and height). Please, check if your input is correct." << std::endl;
            } else {
                int width = static_cast<int>(*filter_struct.params_[0]);
                int height = static_cast<int>(*filter_struct.params_[1]);
                CropFilter crop;
                crop.ApplyFilter(width, height, image);
            }
        } else if (filter_struct.name_ == "-edge") {
            if (filter_struct.params_.size() != 1) {
                std::cout << "Edge filter requires one parameter for threshhold. Please, check if your input is correct." << std::endl;
            } else {
                double threshold = static_cast<double>(*filter_struct.params_[0]);
                EdgeDetection edge(threshold);
                edge.ApplyFilter(image);
            }
        } else if (filter_struct.name_ == "-gs") {
            if (!filter_struct.params_.empty()) {
                std::cout << "Grayscale filter doesn't require any parameters." << std::endl;
            } else {
                GrayscaleFilter grayscale;
                grayscale.ApplyFilter(image);
            }
        } else if (filter_struct.name_ == "-neg") {
            if (!filter_struct.params_.empty()) {
                std::cout << "Negative filter doesn't require any parameters." << std::endl;
            } else {
                NegFilter neg;
                neg.ApplyFilter(image);
            }
        } else if (filter_struct.name_ == "-sharp") {
            if (!filter_struct.params_.empty()) {
                std::cout << "Sharp filter doesn't require any parameters." << std::endl;
            } else {
                Sharpening sharp;
                sharp.ApplyFilter(image);
            }
        } else if (filter_struct.name_ == "-blur") {
            std::cout << "This filter is not released yet." << std::endl;
        } else {
            std::cout << "There is no filter with name " << filter_struct.name_ << ". Please, enter the input correctly." << std::endl;
        }
    }
    image.Load(line_parser.FilenameOut());
    return 0;
}
