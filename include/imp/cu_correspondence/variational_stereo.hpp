#pragma once

#include <cstdint>
#include <memory>

#include <imp/cu_core/cu_image_gpu.cuh>
#include <imp/cu_correspondence/variational_stereo_parameters.hpp>

namespace ze {
namespace cu {

// forward declarations
class StereoCtFWarping;

/**
 * @brief The Stereo class takes a stereo image pair and estimates the disparity map
 */
class VariationalStereo
{
public:
  //! @todo (MWE) first do the implementation with specific type (32fC1) and later generalize
  using ImageGpu32fC1 = ze::cu::ImageGpu32fC1;
  using Parameters = VariationalStereoParameters;

public:
  VariationalStereo(Parameters::Ptr params=nullptr);
  virtual ~VariationalStereo(); //= default;

  virtual void addImage(const ImageGpu32fC1::Ptr& image);
  virtual void solve();

  virtual ImageGpu32fC1::Ptr computePrimalEnergy(size_t level=0);
  virtual ImageGpu32fC1::Ptr getDisparities(size_t level=0);
  virtual ImageGpu32fC1::Ptr getOcclusion(size_t level=0);

  // getters / setters
  virtual inline Parameters::Ptr parameters() {return params_;}

protected:
  Parameters::Ptr params_;  //!< configuration parameters
  std::unique_ptr<StereoCtFWarping> ctf_;  //!< performing a coarse-to-fine warping scheme
};

} // namespace cu
} // namespace ze
