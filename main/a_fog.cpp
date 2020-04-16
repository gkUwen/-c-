#include <core/image.h>
#include <core/color.h>
#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/textures/constant.h>
#include <rt/renderer.h>
#include <rt/solids/infiniteplane.h>
#include <rt/integrators/recraytrace.h>
#include <rt/integrators/raytrace.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/glass.h>
#include <rt/materials/fuzzymirror.h>
#include <rt/lights/arealight.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/directional.h>
#include <rt/textures/checkerboard.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/volume.h>

using namespace rt;

namespace {

    void makeBox(Group* scene, const Point& aaa, const Vector& forward, const Vector& left, const Vector& up, CoordMapper* texMapper, Material* material) {
        scene->add(new Quad(aaa, left, forward, texMapper, material));
        scene->add(new Quad(aaa, forward, up, texMapper, material));
        scene->add(new Quad(aaa, up, left, texMapper, material));
        Point bbb = aaa + forward + left + up;
        scene->add(new Quad(bbb, -forward, -left, texMapper, material));
        scene->add(new Quad(bbb, -up, -forward, texMapper, material));
        scene->add(new Quad(bbb, -left, -up, texMapper, material));
    }


    void renderCornellbox(float scale, const char* filename, Camera* cam, Material* sphereMaterial, Material* floorMaterial, int numSamples = 1) {
        Image img(400, 400);
        World world;
        //world.fog = true;
        SimpleGroup* scene = new SimpleGroup();
        world.scene = scene;

        Texture* redtex = new ConstantTexture(RGBColor(.7f, 0.f, 0.f));
        Texture* greentex = new ConstantTexture(RGBColor(0.f, .7f, 0.f));
        Texture* yellowtex = new ConstantTexture(RGBColor(1.0f, 0.0f, 0.f));
        Texture* bluetex = new ConstantTexture(RGBColor(0.f, 0.f, 0.7f));
        Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
        Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));

        Material* grey = new LambertianMaterial(blacktex, whitetex);
        Material* yellowmat = new LambertianMaterial(blacktex, yellowtex);
        Material* leftWallMaterial = new LambertianMaterial(blacktex, redtex);
        Material* rightWallMaterial = new LambertianMaterial(blacktex, greentex);

        //walls
        scene->add(new InfinitePlane(Point(000.f, 000.f, 000.f)*scale, Vector(000.f, 1.f, 0.f)*scale, nullptr, floorMaterial));
        scene->add(new Quad(Point(000.f, 000.f, 000.f)*scale, Vector(000.f, 000.f, 5600.f)*scale, Vector(5500.f, 000.f, 000.f)*scale, nullptr, floorMaterial)); //floor
        scene->add(new Quad(Point(600.f, 600.f, 000.f)*scale, Vector(000.f, 000.f, 560000.f)*scale, Vector(-600.f, 000.f, 000.f)*scale, nullptr, grey)); //ceiling
        scene->add(new Quad(Point(000.f, 000.f, 4000.f)*scale, Vector(000.f, 550.f, 000.f)*scale, Vector(550.f, 000.f, 000.f)*scale, nullptr, grey)); //back wall
        scene->add(new Quad(Point(000.f, 000.f, 000.f)*scale, Vector(000.f, 5500.f, 000.f)*scale, Vector(000.f, 000.f, 560000.f)*scale, nullptr, rightWallMaterial)); //right wall
        scene->add(new Quad(Point(600.f, 600.f, 000.f)*scale, Vector(000.f, -5500.f, 000.f)*scale, Vector(000.f, 000.f, 560000.f)*scale, nullptr, leftWallMaterial)); //left wall

        for (int i = 0; i<5;i++){
            scene->add(new Sphere(Point(450.0f-80.0f*i, 60.0f, 50.0f+1000.f*i)*scale, 60.0f*scale, nullptr, yellowmat));
        }

         world.light.push_back(new PointLight(Point(10.0f * scale, 10.0f*scale, 100.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(20.0f * scale, 400.0f*scale, 2000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(40.0f * scale, 400.0f*scale, 3000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(50.0f * scale, 400.0f*scale, 4000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(50.0f * scale, 400.0f*scale, 5000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(60.0f * scale, 400.0f*scale, 6000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(70.0f * scale, 400.0f*scale, 7000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        // world.light.push_back(new PointLight(Point(80.0f * scale, 400.0f*scale, 8000.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
         //world.light.push_back(new PointLight(Point(100.0f * scale, 400.0f*scale, 0.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));
        //world.light.push_back(new PointLight(Point(300.0f * scale, 400.0f*scale, 0.0f*scale), RGBColor(400000.0f*scale*scale, 400000.0f*scale*scale, 400000.0f*scale*scale)));

         
        //RayTracingIntegrator integrator(&world);
		volume integrator(&world);

        Renderer engine(cam, &integrator);
        if (numSamples > 1)
            engine.setSamples(numSamples);
        engine.render(img);
        img.writePNG(filename);
    }
}

void a_fog() {
    PerspectiveCamera* cam = new PerspectiveCamera(Point(0.278f, 0.273f, -0.800f), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* Checkerboardtex = new CheckerboardTexture(RGBColor::rep(1.0f),RGBColor::rep(0.0f));
    Material* floorMaterial1 = new LambertianMaterial(blacktex, Checkerboardtex);
    Material* sphereMaterial1 = floorMaterial1;

    renderCornellbox(0.001f, "a8-fog-1.png", cam, sphereMaterial1, floorMaterial1, 1);
     
}


