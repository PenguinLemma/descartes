#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace plemma::glancy {

class Image
{
  public:
    Image(size_t width, size_t height) : pixels_(width, std::vector<Vec3>(height)) {}
    void PaintPixel(size_t h_index, size_t v_index, Vec3& color)
    {
        pixels_[h_index][v_index] = color;
    }
    void Save(std::string const& file_path) const;

  private:
    std::vector<std::vector<Vec3> > pixels_{};
};

void Image::Save(std::string const& file_path) const
{
    std::ofstream my_image;
    my_image.open(file_path);
    size_t width = pixels_.size();
    size_t height = pixels_[0].size();
    my_image << "P3\n" << width << " " << height << "\n255\n";
    for (size_t c = height; c > 0U; --c) {
        size_t column = c - 1U;
        for (size_t row = 0U; row < width; ++row) {
            my_image << static_cast<size_t>(pixels_[row][column].R()) << " ";
            my_image << static_cast<size_t>(pixels_[row][column].G()) << " ";
            my_image << static_cast<size_t>(pixels_[row][column].B()) << std::endl;
        }
    }
    my_image.close();
}

}  // namespace plemma::glancy
