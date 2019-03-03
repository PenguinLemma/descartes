#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <limits>
#include <algorithm>
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "rand_engine.h"

namespace plemma
{
namespace glancy
{

Vec3 GetColor(const Ray& r, HitableList& world, int depth)
{
    HitRecord rec;
    // We increase the minimum parameter to avoid finding out the original intersection again
    RealNum min_param = 0.001f;
    if (world.Hit(r, min_param, std::numeric_limits<RealNum>::max(), rec))
    {
        Ray scattered_ray;
        Vec3 attenuation;
        if (depth < 50 && rec.mat->Scatter(r, rec, attenuation, scattered_ray))
        {
            return attenuation*GetColor(scattered_ray, world, depth + 1);
        }
        else
        {
            return Vec3(0.0, 0.0, 0.0);
        }
    }
    else
    {
        Vec3 unit_direction = UnitVector(r.Direction());
        RealNum t = 0.5f*(unit_direction.Y() + 1.0f);
        return (1.0f-t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
    }
}

std::vector<std::unique_ptr<Hitable> > CreateSimpleScene()
{
    std::vector<std::unique_ptr<Hitable> > spheres;
    spheres.push_back(std::make_unique<Sphere>(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                                        std::make_shared<Lambertian>(Vec3(0.5f, 0.5f, 0.5f))));
    spheres.push_back(std::make_unique<Sphere>(Vec3(0.0f, 1.0f, 0.0f), 1.0f,
                                        std::make_shared<Dielectric>(1.5f)));
    return spheres;
}


std::vector<std::unique_ptr<Hitable> > CreateRandomScene()
{
    std::vector<std::unique_ptr<Hitable> > spheres;
    int n = 500;
    int number_of_spheres = 0;
    std::uniform_real_distribution<RealNum> dist(0.0, 1.0);

    spheres.push_back(std::make_unique<Sphere>(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                                std::make_shared<Lambertian>(Vec3(0.5f, 0.5f, 0.5f))));

    spheres.push_back(std::make_unique<Sphere>(Vec3(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(1.5f)));

    spheres.push_back(std::make_unique<Sphere>(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Lambertian>(Vec3(0.4f, 0.2f, 0.1f))));

    spheres.push_back(std::make_unique<Sphere>(Vec3(4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Metal>(Vec3(0.7f, 0.6f, 0.5f), 0.0f)));
    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            RealNum choose_mat = dist(my_engine());
            Vec3 center(static_cast<RealNum>(a) + 0.9f * dist(my_engine()), 0.2f, static_cast<RealNum>(b) + 0.9f * dist(my_engine()));
            if ((center - Vec3(4.0f, 0.2f, 0.0f)).SquaredNorm() > 0.9f * 0.9f)
            {
                if (choose_mat < 0.65f)
                {
                    spheres.push_back(std::make_unique<Sphere>(center, 0.2f,
                        std::make_shared<Lambertian>(Vec3(dist(my_engine()) * dist(my_engine()), dist(my_engine()) * dist(my_engine()), dist(my_engine()) * dist(my_engine())))));
                }
                else if (choose_mat < 0.85f)
                {
                    spheres.push_back(std::make_unique<Sphere>(center, 0.2f,
                        std::make_shared<Metal>(Vec3(0.5f*(1.0f + dist(my_engine())), 0.5f*(1.0f + dist(my_engine())), 0.5f*(1.0f + dist(my_engine()))), 0.5f*dist(my_engine()))));
                }
                else
                {
                    spheres.push_back(std::make_unique<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
                }
            }
        }
    }
    return spheres;
}

} // namespace glancy

} // namespace plemma

int main() {

    using plemma::glancy::RealNum;
    using plemma::glancy::Vec3;
    using plemma::glancy::Image;
    uint16_t nx = 2000;
    uint16_t ny = 1500;
    uint16_t n_rays_per_pixel = 100;


    Vec3 lookfrom(10.0f, 1.4f, 2.0f);
    Vec3 lookat(3.5f, 0.6f, 0.5f);
    RealNum dist_to_focus = (lookfrom - lookat).Norm();
    RealNum aperture = 0.1f;
    RealNum aspect = static_cast<RealNum>(nx) / static_cast<RealNum>(ny);

    plemma::glancy::Camera cam(lookfrom, lookat, Vec3(0.0f, 1.0f, 0.0f), 30.0f, aspect, aperture, dist_to_focus);

    std::cout << "------ Welcome to Glancy ------" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Creating random spheres distribution" << std::endl;
    std::cout << std::endl;

    plemma::glancy::HitableList world(plemma::glancy::CreateRandomScene());

    std::cout << "Please, wait patiently while Glancy is enlightened" << std::endl;
    std::cout << std::endl;

    std::cout << "0% processing completed." << std::endl;

    Image image(nx, ny);

    int percentage_completed = 0;
    int prev_percentage_written = 0;
    const int initial_depth = 0;
    RealNum width = static_cast<RealNum>(nx);
    RealNum height = static_cast<RealNum>(ny);
    plemma::glancy::my_engine();
    std::uniform_real_distribution<RealNum> dist(0.0, 1.0);
    for (uint16_t i = 0U; i < nx; ++i)
    {
        for (uint16_t j = 0U; j < ny; ++j)
        {
            Vec3 color(0.0, 0.0, 0.0);
            for (uint16_t s = 0U; s < n_rays_per_pixel; ++s)
            {
                RealNum u = (static_cast<RealNum>(i) + dist(plemma::glancy::my_engine())) / width;
                RealNum v = (static_cast<RealNum>(j) + dist(plemma::glancy::my_engine())) / height;

                plemma::glancy::Ray r = cam.GetRay(u,v);

                color += GetColor(r, world, initial_depth);
            }

            color /= static_cast<RealNum>(n_rays_per_pixel);
            // The sqrts below are for gamma correction -> read about it
            auto gamma_correction = [](RealNum x){ return static_cast<RealNum>(std::sqrt(x)); };
            std::transform(color.Begin(), color.End(), color.Begin(), gamma_correction);
            color *= static_cast<RealNum>(255.9999);
            image.PaintPixel(i, j, color);
        }

        percentage_completed = static_cast<int>(100.0 * (static_cast<RealNum>(i + 1) / width));
        if (static_cast<int>(percentage_completed) >= prev_percentage_written + 5)
        {
            int to_write = percentage_completed - (percentage_completed % 5);
            std::cout << to_write << "% processing completed." << std::endl;
            prev_percentage_written = to_write;
        }
    }

    image.Save("../myimage.ppm");

    std::cout << "100% processing completed." << std::endl;
    std::cout << std::endl;
    std::cout << "---- Glancy.exe FINISHED ITS JOB ----" << std::endl;
    std::cout << "Results can be seen in 'myimage.ppm', in the root of this repo." << std::endl << std::endl;
}
