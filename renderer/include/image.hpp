#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

namespace plemma
{
namespace glancy
{

class Image
{
public:
    Image(size_t width, size_t height) : pixels_(width, std::vector<Vec3> (height)) {}
    void PaintPixel(size_t h_index, size_t v_index, Vec3& color) { pixels_[h_index][v_index] = color; }
    void Save(const std::string& file_path) const;
private:
    std::vector<std::vector<Vec3> > pixels_;
};

void Image::Save(const std::string& file_path) const
{
    std::ofstream myimage;
    myimage.open(file_path);
    size_t width = pixels_.size();
    size_t height = pixels_[0].size();
    myimage << "P3\n" << width << " " << height << "\n255\n";
    for (size_t c = height; c > 0U; --c)
    {
        size_t column = c - 1U;
        for (size_t row = 0U; row < width; ++row)
        {
            myimage << static_cast<size_t>(pixels_[row][column].R()) << " ";
            myimage << static_cast<size_t>(pixels_[row][column].G()) << " ";
            myimage << static_cast<size_t>(pixels_[row][column].B()) << std::endl;
        }
    }

    myimage.close();
}

} // namespace glancy

} // namespace plemma
