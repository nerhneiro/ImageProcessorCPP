//
// Created by I on 21.03.2023.
//
#pragma once
#include "color.h"
#include <vector>
#include <fstream>
class Image {
public:
    using Channel = std::vector<std::vector<double>>;
    Image();
    Image(int width, int height);
    ~Image();
    Color GetColor(int x, int y) const;
    void SetColor(const Color &color, int x, int y);
    double GetColorR(int x, int y) const;
    double GetColorG(int x, int y) const;
    double GetColorB(int x, int y) const;
    void SetColorR(double value, int x, int y);
    void SetColorG(double value, int x, int y);
    void SetColorB(double value, int x, int y);
    void Read(const char *path);
    void Load(const char *path) const;
    void ChangeSize();  // меняет размер изображения на текущую ширину и высоту
    void SetWidth(int width) {
        width_ = width;
    }
    void SetHeight(int height);
    size_t GetWidth();
    size_t GetHeight();
    std::vector<std::vector<std::vector<double>>> *GetChannels();

private:
    int width_;
    int height_;
    std::vector<Channel> channels_;
};
