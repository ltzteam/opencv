// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2020, Stefan Brüns <stefan.bruens@rwth-aachen.de>

#ifndef _GRFMT_OPENJPEG_H_
#define _GRFMT_OPENJPEG_H_

#ifdef HAVE_OPENJPEG

#include "grfmt_base.hpp"
#include <openjpeg.h>

namespace cv {
namespace detail {
struct OpjMemoryBuffer {
    OPJ_BYTE* pos;
    OPJ_BYTE* begin;
    OPJ_SIZE_T length;

    OpjMemoryBuffer():pos(NULL), begin(NULL), length(0)
    {
    }

    explicit OpjMemoryBuffer(cv::Mat& mat) : pos(mat.ptr()), begin(mat.ptr()), length(mat.rows * mat.cols * mat.elemSize())
    {
    }

    OPJ_SIZE_T availableBytes() const CV_NOEXCEPT {
        return begin + length - pos;
    }
};
    typedef cv::Ptr<opj_stream_t> StreamPtr;
    typedef cv::Ptr<opj_codec_t> CodecPtr;
    typedef cv::Ptr<opj_image_t> ImagePtr;
} // namespace detail

class Jpeg2KOpjDecoder CV_FINAL : public BaseImageDecoder
{
public:
    Jpeg2KOpjDecoder();
     ~Jpeg2KOpjDecoder() CV_OVERRIDE{};

    ImageDecoder newDecoder() const CV_OVERRIDE;
    bool readData( Mat& img ) CV_OVERRIDE;
    bool readHeader() CV_OVERRIDE;

private:
    detail::StreamPtr stream_;
    detail::CodecPtr codec_;
    detail::ImagePtr image_;

    detail::OpjMemoryBuffer opjBuf_;

    OPJ_UINT32 m_maxPrec;
};

class Jpeg2KOpjEncoder CV_FINAL : public BaseImageEncoder
{
public:
    Jpeg2KOpjEncoder();
    ~Jpeg2KOpjEncoder() CV_OVERRIDE{};

    bool isFormatSupported( int depth ) const CV_OVERRIDE;
    bool write( const Mat& img, const std::vector<int>& params ) CV_OVERRIDE;
    ImageEncoder newEncoder() const CV_OVERRIDE;
};

} //namespace cv

#endif

#endif/*_GRFMT_OPENJPEG_H_*/
