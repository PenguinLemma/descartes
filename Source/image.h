#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

namespace plemma
{
namespace descartes
{

class Image
{
public:
    Image(uint16_t width, uint16_t height) : pixels_(width, std::vector<Vec3> (height)) {}
    void PaintPixel(uint16_t h_index, uint16_t v_index, Vec3& color) { pixels_[h_index][v_index] = color; }
    void Save(const std::string& file_path) const;
private:
    std::vector<std::vector<Vec3> > pixels_;
};

void Image::Save(const std::string& file_path) const
{
    std::ofstream myimage;
    myimage.open(file_path);
    uint16_t width = pixels_.size();
    uint16_t height = pixels_[0].size();
    myimage << "P3\n" << width << " " << height << "\n255\n";
    for (uint16_t c = height; c > 0U; --c)
    {
        uint16_t column = c - 1U;
        for (uint16_t row = 0; row < width; ++row)
        {
            myimage << static_cast<int>(pixels_[row][column].R()) << " ";
            myimage << static_cast<int>(pixels_[row][column].G()) << " ";
            myimage << static_cast<int>(pixels_[row][column].B()) << std::endl;
        }
    }

    myimage.close();
}

} // namespace descartes

} // namespace plemma
