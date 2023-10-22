//
// Created by I on 21.03.2023.
//
#pragma once
#include "image.h"
#include <iostream>
class Filter {
public:
    virtual void ApplyFilter(Image &image) const = 0;
    virtual ~Filter() = default;
};

class ChannelWiseFilters : public Filter {
public:
    ~ChannelWiseFilters() override = default;

    void ApplyFilter(Image &image) const final;

protected:
    virtual void FilterImpl(Image::Channel &channel) const = 0;
};

class ConvFilter : public ChannelWiseFilters {
public:
    ~ConvFilter() override = default;

    explicit ConvFilter(const std::vector<std::vector<float>> &matrix) : matrix_(matrix) {
        padding_ = matrix.size() / 2;
    }

protected:
    void FilterImpl(Image::Channel &channel) const final;

private:
    std::vector<std::vector<float>> matrix_;
    size_t padding_;
};

class CompositeFilter : public Filter {
public:
    ~CompositeFilter() override = default;

    explicit CompositeFilter(std::vector<std::unique_ptr<Filter>> filters) : filters_(std::move(filters)) {
    }

    void ApplyFilter(Image &image) const final;

protected:
    void AddFilter(std::unique_ptr<Filter> filter_ptr);

private:
    std::vector<std::unique_ptr<Filter>> filters_;
};