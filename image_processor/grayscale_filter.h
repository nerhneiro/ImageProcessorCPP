//
// Created by I on 20.03.2023.
//
#pragma once
#include "image.h"
#include "filter.h"
class GrayscaleFilter : public Filter {
public:
    ~GrayscaleFilter() override = default;
    void ApplyFilter(Image &image) const final;
};