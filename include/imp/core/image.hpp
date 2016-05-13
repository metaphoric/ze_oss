#pragma once

#include <cstdint>

#include <ze/common/types.h>
#include <ze/common/macros.h>
#include <imp/core/image_base.hpp>
#include <imp/core/exception.hpp>
#include <imp/core/pixel.hpp>

namespace ze {

template<typename Pixel>
class Image : public ImageBase
{
public:
  ZE_POINTER_TYPEDEFS(Image);
  using pixel_t = Pixel;

public:
  Image() = delete;
  virtual ~Image() = default;

  /** Returns a pointer to the pixel data.
   * The pointer can be offset to position \a (ox/oy).
   * @param[in] ox Horizontal offset of the pointer array.
   * @param[in] oy Vertical offset of the pointer array.
   * @return Pointer to the pixel array.
   */
  virtual Pixel* data(std::uint32_t ox = 0, std::uint32_t oy = 0) = 0;
  virtual const Pixel* data(std::uint32_t ox = 0, std::uint32_t oy = 0) const = 0;

  /** Get Pixel value at position x,y. */
  Pixel pixel(std::uint32_t x, std::uint32_t y) const
  {
    return *data(x, y);
  }

  /** Get Pixel value at position x,y. */
  Pixel& pixel(std::uint32_t x, std::uint32_t y)
  {
    return *data(x, y);
  }

  /** Get Pixel value at position x,y. */
  Pixel operator()(std::uint32_t x, std::uint32_t y) const
  {
    return *data(x, y);
  }

  /** Get Pixel value at position x,y. */
  Pixel operator()(const Vector2i& px) const
  {
    return *data(px(0), px(1));
  }

  /** Get Pointer to beginning of row \a row (y index).
   * This enables the usage of [y][x] operator.
   */
  Pixel* operator[] (std::uint32_t row)
  {
    return data(0,row);
  }
  const Pixel* operator[] (std::uint32_t row) const
  {
    return data(0,row);
  }


  /**
   * @brief setValue Sets image data to the specified \a value.
   * @param value Value to be set to the whole image data.
   */
  virtual void setValue(const Pixel& value)
  {
    if (this->bytes() == this->pitch()*this->height())
    {
      std::fill(this->data(), this->data()+this->stride()*this->height(), value);
    }
    else
    {
      for (std::uint32_t y=0; y<this->height(); ++y)
      {
        for (std::uint32_t x=0; x<this->width(); ++x)
        {
          this->data()[y*this->stride()+x] = value;
        }
      }
    }
  }

  /**
   * @brief copyTo copies the internal image data to another class instance
   * @param dst Image class that will receive this image's data.
   */
  virtual void copyTo(Image& dst) const
  {
    if (this->width() != dst.width() || this->height() != dst.height())
    {
      throw ze::Exception("Copying failed: Image size differs.", __FILE__, __FUNCTION__, __LINE__);
    }

    // check if dst image is on the gpu and the src image is not so we can
    // use the copyFrom functionality from the dst image as the Image class
    // doesn't know anything about gpu memory (poor thing)
    if (dst.isGpuMemory())
    {
      dst.copyFrom(*this);
    }
    else if (this->bytes() == dst.bytes())
    {
      std::copy(this->data(), this->data()+this->stride()*this->height(), dst.data());
    }
    else
    {
      for (std::uint32_t y=0; y<this->height(); ++y)
      {
        for (std::uint32_t x=0; x<this->width(); ++x)
        {
          dst[y][x] = this->pixel(x,y);
        }
      }
    }
  }

  /**
   * @brief copyFrom copies the image data from another class instance to this image
   * @param from Image class providing the image data.
   */
  virtual void copyFrom(const Image& from)
  {
    if (this->size()!= from.size())
    {
      throw ze::Exception("Copying failed: Image sizes differ.", __FILE__, __FUNCTION__, __LINE__);
    }

    if (from.isGpuMemory())
    {
      from.copyTo(*this);
    }
    else if (this->bytes() == from.bytes())
    {
      std::copy(from.data(), from.data()+from.stride()*from.height(), this->data());
    }
    else
    {
      for (std::uint32_t y=0; y<this->height(); ++y)
      {
        for (std::uint32_t x=0; x<this->width(); ++x)
        {
          (*this)[y][x] = from.pixel(y,x);
        }
      }
    }
  }


protected:
  Image(ze::PixelOrder pixel_order = ze::PixelOrder::undefined)
    : ImageBase(pixel_type<Pixel>::type, sizeof(Pixel), pixel_order)
  {
  }

  Image(std::uint32_t width, std::uint32_t height,
        PixelOrder pixel_order = ze::PixelOrder::undefined)
    : Image(pixel_type<Pixel>::type, sizeof(Pixel), pixel_order, {width, height})
  {
  }

  Image(
      PixelType pixel_type,
      size_t pixel_size,
      PixelOrder pixel_order,
      const Size2u &size)
    : ImageBase(pixel_type<Pixel>::type, sizeof(Pixel), pixel_order, size)
  {
  }

//  Image(const Image& from) = default;
};

//-----------------------------------------------------------------------------
// convenience typedefs
typedef Image<ze::Pixel8uC1> Image8uC1;
typedef Image<ze::Pixel8uC2> Image8uC2;
typedef Image<ze::Pixel8uC3> Image8uC3;
typedef Image<ze::Pixel8uC4> Image8uC4;

typedef Image<ze::Pixel16uC1> Image16uC1;
typedef Image<ze::Pixel16uC2> Image16uC2;
typedef Image<ze::Pixel16uC3> Image16uC3;
typedef Image<ze::Pixel16uC4> Image16uC4;

typedef Image<ze::Pixel32sC1> Image32sC1;
typedef Image<ze::Pixel32sC2> Image32sC2;
typedef Image<ze::Pixel32sC3> Image32sC3;
typedef Image<ze::Pixel32sC4> Image32sC4;

typedef Image<ze::Pixel32fC1> Image32fC1;
typedef Image<ze::Pixel32fC2> Image32fC2;
typedef Image<ze::Pixel32fC3> Image32fC3;
typedef Image<ze::Pixel32fC4> Image32fC4;

// convenience typedefs
template<typename Pixel>
using ImagePtr = typename std::shared_ptr<Image<Pixel>>;

} // namespace ze

