#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "camera.hpp"
#include "dielectric.hpp"
#include "different_dielectrics_scene.hpp"
#include "hitable_list.hpp"
#include "image.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "rand_engine.hpp"
#include "random_spheres_scene.hpp"
#include "renderer.hpp"
#include "sphere.hpp"
#include "two_spheres_scene.hpp"

int main()
{

    using plemma::glancy::Camera;
    using plemma::glancy::DifferentDielectricsScene;
    using plemma::glancy::Image;
    using plemma::glancy::RandomSpheresScene;
    using plemma::glancy::Real;
    using plemma::glancy::RealNum;
    using plemma::glancy::Scene;
    using plemma::glancy::TwoSpheresScene;
    using plemma::glancy::Vec3;

    plemma::glancy::my_engine();

    std::cout << "------ Welcome to Glancy ------" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Creating scene with two spheres" << std::endl;
    std::cout << std::endl;

    RandomSpheresScene scene;
    scene.LoadWorld();

    size_t nx = 2000;
    size_t ny = 1500;
    Image image(nx, ny);

    Vec3 look_from(Real(10), Real(1.4), Real(2));
    Vec3 look_at(Real(3.5), Real(0.6), Real(0.5));
    Vec3 vertical_positive_camera{Real(0), Real(1), Real(0)};
    RealNum vertical_fov_deg{Real(30)};
    RealNum dist_to_focus = (look_from - look_at).Norm();
    RealNum aperture = Real(0.1);
    RealNum aspect = Real(nx) / Real(ny);
    RealNum t0{Real(0)};
    RealNum t1{Real(0.1)};

    Camera camera(look_from,
                  look_at,
                  vertical_positive_camera,
                  vertical_fov_deg,
                  aspect,
                  aperture,
                  dist_to_focus,
                  t0,
                  t1);

    size_t n_rays_per_pixel = 100;
    uint16_t max_depth = 50;
    auto gamma_correction = [](RealNum x) { return Real(std::sqrt(x)); };
    plemma::glancy::Renderer rend(gamma_correction, nx, ny, n_rays_per_pixel, max_depth);

    std::cout << "Please, wait patiently while Glancy is enlightened" << std::endl;
    std::cout << std::endl;

    rend.ProcessScene(scene, camera, image);
    image.Save("../myimage.ppm");

    std::cout << "---- Glancy finished its job ----" << std::endl;
    std::cout << "Results can be seen in 'myimage.ppm', in the root of this repo." << std::endl
              << std::endl;
}
