////
//// Created by I on 10.03.2023.
////
//
#pragma once
#include <vector>
#include <iostream>
#include "grayscale_filter.h"
#include "filter.h"
#include "image.h"

class CropFilter {
public:
    ~CropFilter() = default;
    void ApplyFilter(int width, int height, Image &image) const {
        std::vector<std::vector<std::vector<double>>> *image_channels = image.GetChannels();
        if (width > image.GetWidth() && height > image.GetHeight()) {
            return;
        }
        image.SetWidth(width);
        image.SetHeight(height);
        for (size_t i = 0; i < (*image_channels).size(); ++i) {
            (*image_channels)[i].resize(width);
            for (size_t x = 0; x < width; ++x) {
                (*image_channels)[i][x].resize(height);
            }
        }
    }
};

class NegFilter final : public ChannelWiseFilters {
public:
    ~NegFilter() override = default;

protected:
    void FilterImpl(Image::Channel &channel) const override {
        for (auto &row : channel) {
            for (auto &el : row) {
                el = 1 - el;
            }
        }
    }
};


class Sharpening : public ConvFilter {
public:
    Sharpening() : ConvFilter({{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}}) {
    }
};

class Threshold : public ChannelWiseFilters {
public:
    explicit Threshold(double threshold) : threshold_(threshold) {
    }

protected:
    void FilterImpl(Image::Channel &channel) const override {
        for (auto &row : channel) {
            for (auto &el : row) {
                if (el > threshold_) {
                    el = 1;
                } else {
                    el = 0;
                }
            }
        }
    }

private:
    double threshold_;
};

class EdgeDetection : public CompositeFilter {
public:
    ~EdgeDetection() override = default;
    explicit EdgeDetection(double threshold) : CompositeFilter({}) {
        AddFilter(std::make_unique<GrayscaleFilter>());
        AddFilter(std::make_unique<ConvFilter>(std::vector<std::vector<float>>{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}));
        AddFilter(std::make_unique<Threshold>(threshold));
    }
};
