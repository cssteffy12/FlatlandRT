/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "imaging/io/io_openexr.h"

#include "src/lib/OpenEXR/ImfArray.h"
#include "src/lib/OpenEXR/ImfChannelList.h"
#include "src/lib/OpenEXR/ImfIO.h"
#include "src/lib/OpenEXR/ImfInputFile.h"
#include "src/lib/OpenEXR/ImfOutputFile.h"
#include "src/lib/OpenEXR/ImfRgbaFile.h"
#include "src/lib/OpenEXR/ImfStringAttribute.h"

#include <cfloat>

using namespace Imf;
using namespace Imath;

DE_VERTEXWAHN_BEGIN_NAMESPACE

Image3f load_image_openexr(std::string_view filename) {
    // see https://www.openexr.com/documentation/ReadingAndWritingImageFiles.pdf
    // Heading Reading an Image File
    InputFile file(filename.data());
    const Header &header = file.header();

    Box2i dw = header.dataWindow();
    int width = dw.max.x - dw.min.x + 1;
    int height = dw.max.y - dw.min.y + 1;

    /*
    bool hasRed = false;
    bool hasGreen = false;
    bool hasBlue = false;

    for (ChannelList::ConstIterator it = header.channels().begin(), ite = header.channels().end(); it != ite; it++) {
        if ((strcmp(it.name(), "R") == 0)) { hasRed = true; }
        if ((strcmp(it.name(), "G") == 0)) { hasGreen = true; }
        if ((strcmp(it.name(), "B") == 0)) { hasBlue = true; }
        if (it.channel().type != HALF) {
            //HDR_LOG("Unable to open EXR file \"%s\" (unsupported data type %s)", filename, it.channel().type);
            //return (IEFileCantOpen);
        }
    }
    */

    Imf::Array2D<float> rPixels;
    Imf::Array2D<float> gPixels;
    Imf::Array2D<float> bPixels;

    rPixels.resizeErase(height, width);
    gPixels.resizeErase(height, width);
    bPixels.resizeErase(height, width);

    FrameBuffer frameBuffer;

    frameBuffer.insert("R", // name
                       Slice(FLOAT, // type
                             (char *) (&rPixels[0][0] - // base
                                       dw.min.x -
                                       dw.min.y * width),
                             sizeof(rPixels[0][0]) * 1, // xStride
                             sizeof(rPixels[0][0]) * width,// yStride
                             1, 1, // x/y sampling
                             FLT_MAX)); // fillValue

    frameBuffer.insert("G", // name
                       Slice(FLOAT, // type
                             (char *) (&gPixels[0][0] - // base
                                       dw.min.x -
                                       dw.min.y * width),
                             sizeof(gPixels[0][0]) * 1, // xStride
                             sizeof(gPixels[0][0]) * width,// yStride
                             1, 1, // x/y sampling
                             FLT_MAX)); // fillValue

    frameBuffer.insert("B", // name
                       Slice(FLOAT, // type
                             (char *) (&bPixels[0][0] - // base
                                       dw.min.x -
                                       dw.min.y * width),
                             sizeof(bPixels[0][0]) * 1, // xStride
                             sizeof(bPixels[0][0]) * width,// yStride
                             1, 1, // x/y sampling
                             FLT_MAX)); // fillValue

    file.setFrameBuffer(frameBuffer);
    file.readPixels(dw.min.y, dw.max.y);

    Image3f img{width, height};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color3f color{rPixels[y][x], gPixels[y][x], bPixels[y][x]};
            img.set_pixel(x, y, color);
        }
    }
    return img;
}

void store_open_exr(const std::string_view &filename, const Image3f &image) {
    Header header(image.width(), image.height());
    header.insert("comments", Imf::StringAttribute("Generated by Okapi"));

    //Imf::Compression& ctype = header.compression();
    //assert(ctype == Imf::Compression::ZIP_COMPRESSION);

    ChannelList &channels = header.channels();
    channels.insert("R", Imf::Channel(Imf::FLOAT));
    channels.insert("G", Imf::Channel(Imf::FLOAT));
    channels.insert("B", Imf::Channel(Imf::FLOAT));

    FrameBuffer frameBuffer;
    size_t compStride = sizeof(float),
            pixelStride = 3 * compStride,
            rowStride = pixelStride * image.width();

    char *data = reinterpret_cast<char *>(image.data());
    frameBuffer.insert("R", Imf::Slice(Imf::FLOAT, data, pixelStride, rowStride));
    data += compStride;
    frameBuffer.insert("G", Imf::Slice(Imf::FLOAT, data, pixelStride, rowStride));
    data += compStride;
    frameBuffer.insert("B", Imf::Slice(Imf::FLOAT, data, pixelStride, rowStride));

    OutputFile file(filename.data(), header);
    file.setFrameBuffer(frameBuffer);
    file.writePixels(image.height());
}

void store_open_exr(const std::string_view &filename, ReferenceCounted<Image3f> image) {
    store_open_exr(filename, *image.get());
}

DE_VERTEXWAHN_END_NAMESPACE