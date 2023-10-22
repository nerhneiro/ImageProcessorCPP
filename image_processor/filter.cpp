//
// Created by I on 21.03.2023.
//
#include "filter.h"

void CompositeFilter::ApplyFilter(Image& image) const {
    for (const auto& filter : filters_) {
        filter->ApplyFilter(image);
    }
}
void CompositeFilter::AddFilter(std::unique_ptr<Filter> filter_ptr) {
    filters_.push_back(std::move(filter_ptr));
}
void ChannelWiseFilters::ApplyFilter(Image& image) const {
    std::vector<std::vector<std::vector<double>>>* image_channels = image.GetChannels();
    for (auto& channel : *image_channels) {
        FilterImpl(channel);
    }
}
void ConvFilter::FilterImpl(Image::Channel& channel) const {
    const std::vector<int> around_x = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    const std::vector<int> around_y = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    int width = channel.size();
    int height = channel[0].size();
    Image::Channel channel_new;
    for (size_t i = 0; i < width; ++i) {
        channel_new.push_back(std::vector<double>(height, 0));
    }
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            double pixel = 0;
            for (size_t i = 0; i < around_x.size(); ++i) {
                pixel += channel[std::min(std::max(0, static_cast<int>(x) + around_x[i]), width - 1)]
                                [std::min(std::max(0, static_cast<int>(y) + around_y[i]), height - 1)] *
                         matrix_[i / matrix_.size()][i % matrix_.size()];
            }
            channel_new[x][y] = std::min(1.0, std::max(0.0, pixel));
        }
    }
    channel = channel_new;
}
