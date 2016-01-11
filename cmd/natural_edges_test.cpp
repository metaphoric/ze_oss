#include <assert.h>
#include <cstdint>
#include <iostream>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <imp/core/roi.hpp>
#include <imp/core/image_raw.hpp>
#include <imp/bridge/opencv/image_cv.hpp>
#include <imp/cu_core/cu_utils.hpp>
#include <imp/cu_core/cu_image_gpu.cuh>
#include <imp/cu_imgproc/edge_detectors.cuh>
#include <imp/bridge/opencv/cu_cv_bridge.hpp>


int main(int argc, char** argv)
{
  try
  {
    if (argc < 2)
    {
      std::cout << "usage: natural_edges_test input_image_filename";
      return EXIT_FAILURE;
    }
    std::string in_filename(argv[1]);

    std::shared_ptr<ze::cu::ImageGpu32fC1> im;
    ze::cu::cvBridgeLoad(im, in_filename, ze::PixelOrder::gray);

    std::unique_ptr<ze::cu::ImageGpu32fC1> edges(
          new ze::cu::ImageGpu32fC1(*im));
    IMP_CUDA_CHECK();

    ze::cu::naturalEdges(*edges, *im, 1.f, 10.f, 0.7f);

    ze::cu::cvBridgeShow("image", *im);
    ze::cu::cvBridgeShow("edges", *edges, true);

    cv::waitKey();
  }
  catch (std::exception& e)
  {
    std::cout << "[exception] " << e.what() << std::endl;
    assert(false);
  }

  return EXIT_SUCCESS;

}
