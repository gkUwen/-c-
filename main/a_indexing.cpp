
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <time.h>

using namespace rt;

void a_indexing() {
    Image img(800, 600);

    clock_t start, end;

    BVH* scene = new BVH();
    scene->add(new Sphere(Point(2.5f,  .5f,  -1), 0.5  , nullptr, nullptr));
    scene->add(new Sphere(Point(2.5f,  -1.f,  -1), 0.5, nullptr, nullptr));
    scene->add(new Sphere(Point(4.5f,  .5f,  -1), 0.5 , nullptr, nullptr));

    loadOBJ(scene, "models/", "cow.obj");
	scene->rebuildIndex();
    World world;
    world.scene = scene;

    PerspectiveCamera cam1(Point(-8.85f, -7.85f, 7.0f), Vector(1.0f,1.0f,-0.6f), Vector(0, 0, 1), pi/8, pi/6);
    PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f)-Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi/8, pi/6);
    RayCastingIntegrator integrator(&world);

    Renderer engine1(&cam1, &integrator);
    start = clock();
    engine1.render(img);
    end = clock();
    std::cout<<"===> Rendered in "<<(double)(end - start) / CLOCKS_PER_SEC<<" seconds"<<std::endl;
    img.writePNG("a3-1.png");

    Renderer engine2(&cam2, &integrator);
    start = clock();
    engine2.render(img);
    end = clock();
    std::cout<<"===> Rendered in "<<(double)(end - start) / CLOCKS_PER_SEC<<" seconds"<<std::endl;
    img.writePNG("a3-2.png");

    // Bonus Points
    BVH* scene1 = new BVH();
    loadOBJ(scene1, "models/", "su.obj");
	scene1->rebuildIndex();
    World world1;
    world1.scene = scene1;

    PerspectiveCamera cam3(Point(-18, -18, -12), Point(0.6996f, 0.7252f, 6.3365f) - Point(-18, -18, -12), Vector(0, 1, -1), pi/3, pi/3);
    RayCastingIntegrator integrator1(&world1);//Point(0.2634f, 0.8489f, 7.4672f)

    Renderer engine3(&cam3, &integrator1);
    start = clock();
    engine3.render(img);
    end = clock();
    std::cout<<"===> Rendered in "<<(double)(end - start) / CLOCKS_PER_SEC<<" seconds"<<std::endl;
    img.writePNG("Aircraft.png");

    BVH* scene2 = new BVH();
    loadOBJ(scene2, "models/", "deer.obj");
	scene2->rebuildIndex();
    World world2;
    world2.scene = scene2;

    PerspectiveCamera cam4(Point(220, 170, 220), Point(0.000000f, 147.890900f, 0) - Point(220, 170, 220), Vector(1, 1, 1), pi/3, pi/3);
    RayCastingIntegrator integrator2(&world2);//Point(0.2634f, 0.8489f, 7.4672f)

    Renderer engine4(&cam4, &integrator2);
    start = clock();
    engine4.render(img);
    end = clock();
    std::cout<<"===> Rendered in "<<(double)(end - start) / CLOCKS_PER_SEC<<" seconds"<<std::endl;
    img.writePNG("Deer.png");
}