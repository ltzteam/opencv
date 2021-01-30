// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2020, Stefan Brüns <stefan.bruens@rwth-aachen.de>

#ifndef _GRFMT_OPENJPEG_J2K_H_
#define _GRFMT_OPENJPEG_J2K_H_

#ifdef HAVE_OPENJPEG

#include "grfmt_base.hpp"
#include <openjpeg.h>

namespace cv {
namespace detail_j2k {
struct OpjStreamDeleter
{
    void operator()(opj_stream_t* stream) const
    {
        opj_stream_destroy(stream);
    }
};

struct OpjCodecDeleter
{
    void operator()(opj_codec_t* codec) const
    {
        opj_destroy_codec(codec);
    }
};

struct OpjImageDeleter
{
    void operator()(opj_image_t* image) const
    {
        opj_image_destroy(image);
    }
};

struct OpjMemoryBuffer {
    OPJ_BYTE* pos{nullptr};
    OPJ_BYTE* begin{nullptr};
    OPJ_SIZE_T length{0};

    OpjMemoryBuffer() = default;

    explicit OpjMemoryBuffer(cv::Mat& mat)
        : pos{ mat.ptr() }, begin{ mat.ptr() }, length{ mat.rows * mat.cols * mat.elemSize() }
    {
    }

    OPJ_SIZE_T availableBytes() const CV_NOEXCEPT {
        return begin + length - pos;
    }
};

using StreamPtr = std::unique_ptr<opj_stream_t, detail_j2k::OpjStreamDeleter>;
using CodecPtr = std::unique_ptr<opj_codec_t, detail_j2k::OpjCodecDeleter>;
using ImagePtr = std::unique_ptr<opj_image_t, detail_j2k::OpjImageDeleter>;

} // namespace detail_j2k

class JpegJ2KOpjDecoder CV_FINAL : public BaseImageDecoder
{
public:
    JpegJ2KOpjDecoder();
     ~JpegJ2KOpjDecoder() CV_OVERRIDE = default;

    ImageDecoder newDecoder() const CV_OVERRIDE;
    bool readData( Mat& img ) CV_OVERRIDE;
    bool readHeader() CV_OVERRIDE;

private:
    detail_j2k::StreamPtr stream_{nullptr};
    detail_j2k::CodecPtr codec_{nullptr};
    detail_j2k::ImagePtr image_{nullptr};

    detail_j2k::OpjMemoryBuffer opjBuf_;

    OPJ_UINT32 m_maxPrec = 0;
};

class JpegJ2KOpjEncoder CV_FINAL : public BaseImageEncoder
{
public:
    JpegJ2KOpjEncoder();
    ~JpegJ2KOpjEncoder() CV_OVERRIDE = default;

    bool isFormatSupported( int depth ) const CV_OVERRIDE;
    bool write( const Mat& img, const std::vector<int>& params ) CV_OVERRIDE;
    ImageEncoder newEncoder() const CV_OVERRIDE;
};

} //namespace cv

#endif

#endif/*_GRFMT_OPENJPEG_J2K_H_*/
