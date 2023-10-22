//
// Created by I on 21.03.2023.
//
#include "image.h"
#include <iostream>
const int FILE_HEADER_SIZE = 14;
const int INFORMATION_HEADER_SIZE = 40;
unsigned char bmp_pad[3] = {0, 0, 0};

Image::~Image() {
}
void Image::ChangeSize() {
    for (size_t i = 0; i < channels_.size(); ++i) {
        channels_[i].resize(width_);
        for (size_t j = 0; j < channels_[i].size(); ++j) {
            channels_[i][j].clear();
            channels_[i][j].resize(height_);
        }
    }
}
Image::Image(int width, int height) : width_(width), height_(height) {
    for (size_t i = 0; i < 3; ++i) {
        Channel channel;
        channel.resize(width, std::vector<double>(height, 0));
        channels_.push_back(channel);
    }
}
double Image::GetColorR(int x, int y) const {
    return channels_[0][x][y];
}

double Image::GetColorG(int x, int y) const {
    if (channels_.size() == 3) {
        return channels_[1][x][y];
    } else {
        return channels_[0][x][y];
    }
}

double Image::GetColorB(int x, int y) const {
    if (channels_.size() == 3) {
        return channels_[2][x][y];
    } else {
        return channels_[0][x][y];
    }
}

void Image::SetColorR(double value, int x, int y) {
    channels_[0][x][y] = value;
}
void Image::SetColorG(double value, int x, int y) {
    channels_[1][x][y] = value;
}
void Image::SetColorB(double value, int x, int y) {
    channels_[2][x][y] = value;
}
void Image::Read(const char* path) {
    std::ifstream ifstream;
    ifstream.open(path, std::ios::in | std::ios::binary);
    if (!ifstream.is_open()) {
        std::cout << "Error: File could not be opened in Read method" << std::endl;
        return;
    }
    unsigned char file_header[FILE_HEADER_SIZE];
    ifstream.read(reinterpret_cast<char*>(file_header), FILE_HEADER_SIZE);

    if (file_header[0] != 'B' || file_header[1] != 'M') {
        std::cout << "Error: Your file is not in bmp format. Could not read it." << std::endl;
        ifstream.close();
        return;
    }

    unsigned char information_header[INFORMATION_HEADER_SIZE];
    ifstream.read(reinterpret_cast<char*>(information_header), INFORMATION_HEADER_SIZE);

    //    int file_size = file_header[2] + (file_header[3] << 8) + (file_header[4] << 16) + (file_header[5] << 24);
    width_ = information_header[4] + (information_header[5] << 8) + (information_header[6] << 16) +
             (information_header[7] << 24);
    height_ = information_header[8] + (information_header[9] << 8) + (information_header[10] << 16) +
              (information_header[11] << 24);
    ChangeSize();
    const int padding_amount = (4 - (width_ * 3) % 4) % 4;

    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            unsigned char color[3];
            ifstream.read(reinterpret_cast<char*>(color), 3);
            // b g r
            channels_[0][x][y] = static_cast<double>(color[2]) / 255.0;
            channels_[1][x][y] = static_cast<double>(color[1]) / 255.0;
            channels_[2][x][y] = static_cast<double>(color[0]) / 255.0;
        }
        ifstream.ignore(padding_amount);
    }
    ifstream.close();
    std::cout << "File read from " << path << std::endl;
}

// b g r
void Image::Load(const char* path) const {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);
    if (!f.is_open()) {
        std::cout << "Error: File could not be opened in Load method" << std::endl;
        return;
    }

    const int padding_amount = (4 - (width_ * 3) % 4) % 4;
    const int file_size = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + width_ * height_ * 3 + padding_amount * height_;

    unsigned char file_header[FILE_HEADER_SIZE];
    // file type
    file_header[0] = 'B';
    file_header[1] = 'M';
    // file size
    file_header[2] = file_size;
    file_header[3] = file_size >> 8;
    file_header[4] = file_size >> 16;
    file_header[5] = file_size >> 24;
    // Reserved 1
    file_header[6] = 0;
    file_header[7] = 0;
    // Reserved 2
    file_header[8] = 0;
    file_header[9] = 0;
    // Pixel data offset
    file_header[10] = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE;
    file_header[11] = 0;
    file_header[12] = 0;
    file_header[13] = 0;

    unsigned char information_header[INFORMATION_HEADER_SIZE];

    // Header size
    information_header[0] = INFORMATION_HEADER_SIZE;
    information_header[1] = 0;
    information_header[2] = 0;
    information_header[3] = 0;

    // Image width
    information_header[4] = width_;
    information_header[5] = width_ >> 8;
    information_header[6] = width_ >> 16;
    information_header[7] = width_ >> 24;

    // Image height
    information_header[8] = height_;
    information_header[9] = height_ >> 8;
    information_header[10] = height_ >> 16;
    information_header[11] = height_ >> 24;
    //
    information_header[12] = 1;
    information_header[13] = 0;
    // Bits per pixel (RGB)
    information_header[14] = 24;
    information_header[15] = 0;
    // Compression (No compression)
    information_header[16] = 0;
    information_header[17] = 0;
    information_header[18] = 0;
    information_header[19] = 0;
    // Image size (No compression)
    information_header[20] = 0;
    information_header[21] = 0;
    information_header[22] = 0;
    information_header[23] = 0;
    // X Pixels per meter (Not specified)
    information_header[24] = 0;
    information_header[25] = 0;
    information_header[26] = 0;
    information_header[27] = 0;
    // Y Pixels per meter (Not specified)
    information_header[28] = 0;
    information_header[29] = 0;
    information_header[30] = 0;
    information_header[31] = 0;
    // Total colors (Color palette not used)
    information_header[32] = 0;
    information_header[33] = 0;
    information_header[34] = 0;
    information_header[35] = 0;
    // Important colors (Generally ignored)
    information_header[36] = 0;
    information_header[37] = 0;
    information_header[38] = 0;
    information_header[39] = 0;

    f.write(reinterpret_cast<char*>(file_header), FILE_HEADER_SIZE);
    f.write(reinterpret_cast<char*>(information_header),
            INFORMATION_HEADER_SIZE);  // unsigned char pointer to char pointer
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            unsigned char r = static_cast<unsigned char>(GetColorR(static_cast<int>(x), static_cast<int>(y)) * 255.0);
            unsigned char g = static_cast<unsigned char>(GetColorG(static_cast<int>(x), static_cast<int>(y)) * 255.0);
            unsigned char b = static_cast<unsigned char>(GetColorB(static_cast<int>(x), static_cast<int>(y)) * 255.0);

            unsigned char color[] = {b, g, r};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmp_pad), padding_amount);
    }
    f.close();
    std::cout << "File created at " << path << std::endl;
}
void Image::SetHeight(int height) {
    height_ = height;
}
size_t Image::GetWidth() {
    return channels_[0].size();
}
size_t Image::GetHeight() {
    return channels_[0][0].size();
}
std::vector<std::vector<std::vector<double>>>* Image::GetChannels() {
    return &channels_;
}
Color Image::GetColor(int x, int y) const {
    if (channels_.size() == 3) {
        return {channels_[0][x][y], channels_[1][x][y], channels_[2][x][y]};
    }
    return {channels_[0][x][y], channels_[0][x][y], channels_[0][x][y]};
}

void Image::SetColor(const Color& color, int x, int y) {
    channels_[0][x][y] = color.r;
    channels_[1][x][y] = color.g;
    channels_[2][x][y] = color.b;
}
Image::Image() : Image(10, 10){
}
