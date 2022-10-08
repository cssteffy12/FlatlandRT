/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Imaging_io_png_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
#define De_Vertexwahn_Imaging_io_png_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h

#include "core/reference_counted.h"
#include "imaging/image.h"

#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

ReferenceCounted<Image3f> load_image_png(std::string_view filename);

bool store_png(const char *filename, const Image4b &image);

bool store_png(const char *filename, const Image3f &image);

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Imaging_io_png_a17ff2bc_adf7_432f_a1ec_3dbd77af587d_h
