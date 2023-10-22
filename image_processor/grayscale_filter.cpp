//
// Created by I on 21.03.2023.
//
#include "grayscale_filter.h"

void GrayscaleFilter::ApplyFilter(Image& image) const {
    std::vector<std::vector<std::vector<double>>>* image_channels = image.GetChannels();

    if ((*image_channels).size() == 3) {
        for (size_t x = 0; x < image.GetWidth(); ++x) {
            for (size_t y = 0; y < image.GetHeight(); ++y) {
                (*image_channels)[0][x][y] = 0.299 * (*image_channels)[0][x][y] + 0.587 * (*image_channels)[1][x][y] +
                                             0.114 * (*image_channels)[2][x][y];
            }
        }
    } else {
        std::cout << "Error: The number of image channels " << ((*image_channels).size())
                  << "is incorrect. It should be 1 or 3." << std::endl;
    }
    if ((*image_channels).size() == 3) {
        (*image_channels).pop_back();
        (*image_channels).pop_back();
    }
}
