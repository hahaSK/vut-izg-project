#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>
#include <memory>

#include <BasicCamera/OrbitCamera.h>
#include <BasicCamera/PerspectiveCamera.h>
#include <student/phongMethod.hpp>
#include <student/timer.hpp>
#include <tests/performanceTest.hpp>

#define ___ std::cerr << __FILE__ << "/" << __LINE__ << std::endl

void runPerformanceTest(size_t framesPerMeasurement) {
  uint32_t width = 500;
  uint32_t height = 500;
  auto method = std::make_shared<PhongMethod>();
  method->gpu.createFramebuffer(width,height);

  auto perspectiveCamera = basicCamera::PerspectiveCamera();
  auto orbitCamera       = basicCamera::OrbitCamera      ();


  orbitCamera.addDistance(1.f);
  perspectiveCamera.setNear(0.1f);
  auto const aspect = static_cast<float>(width) / static_cast<float>(height);
  perspectiveCamera.setAspect(aspect);

  auto const light  = glm::vec3(10.f,10.f,10.f);
  auto const view   = orbitCamera      .getView      ();
  auto const proj   = perspectiveCamera.getProjection();
  auto const camera = glm::vec3(glm::inverse(view)*glm::vec4(0.f,0.f,0.f,1.f));


  Timer<float>timer;
  timer.reset();
  for (size_t i   = 0; i < framesPerMeasurement; ++i){
    method->onDraw(proj,view,light,camera);
  }
  auto const time = timer.elapsedFromStart() / static_cast<float>(framesPerMeasurement);

  std::cout << "Seconds per frame: " << std::scientific << std::setprecision(10)
            << time << std::endl;

}
