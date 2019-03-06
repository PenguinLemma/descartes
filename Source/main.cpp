#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "rand_engine.h"
#include "renderer.h"
#include "two_spheres_scene.h"
#include "random_spheres_scene.h"

int main() {

    using plemma::glancy::RealNum;
    using plemma::glancy::Vec3;
    using plemma::glancy::Image;
    using plemma::glancy::Scene;
    using plemma::glancy::TwoSpheresScene;
    using plemma::glancy::RandomSpheresScene;
    using plemma::glancy::Camera;

    plemma::glancy::my_engine();

    std::cout << "------ Welcome to Glancy ------" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Creating scene with two spheres" << std::endl;
    std::cout << std::endl;

    RandomSpheresScene scene;
    scene.LoadWorld();

    uint16_t nx = 2000;
    uint16_t ny = 1500;
    Image image(nx, ny);

    Vec3 lookfrom(10.0, 1.4, 2.0);
    Vec3 lookat(3.5, 0.6, 0.5);
    RealNum dist_to_focus = (lookfrom - lookat).Norm();
    RealNum aperture = 0.1;
    RealNum aspect = static_cast<RealNum>(nx) / static_cast<RealNum>(ny);

    Camera camera(lookfrom, lookat, Vec3(0.0, 1.0, 0.0), 30.0, aspect, aperture, dist_to_focus);

    uint16_t n_rays_per_pixel = 100;
    uint16_t max_depth = 50;
    auto gamma_correction = [](RealNum x){ return static_cast<RealNum>(std::sqrt(x)); };
    plemma::glancy::Renderer rend(gamma_correction, nx, ny, n_rays_per_pixel, max_depth);

    std::cout << "Please, wait patiently while Glancy is enlightened" << std::endl;
    std::cout << std::endl;

    rend.ProcessScene(scene, camera, image);
    image.Save("../myimage.ppm");

    std::cout << "---- Glancy finished its job ----" << std::endl;
    std::cout << "Results can be seen in 'myimage.ppm', in the root of this repo." << std::endl << std::endl;
}
