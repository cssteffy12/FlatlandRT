/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h
#define Flatland_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h

#include "flatland/core/namespace.h"
#include "flatland/core/property_set.h"
#include "flatland/math/intersection.h"
#include "flatland/scene/shape/shape.h"

FLATLAND_BEGIN_NAMESPACE

template <typename ScalarType>
class Disk2 : public Shape2<ScalarType> {
public:
    Disk2(const Transform44Type<ScalarType> &transform, const ScalarType radius) : Shape2<ScalarType>(transform), radius_(radius) {
    }

    bool intersect(const Ray2<ScalarType> &ray, MediumEvent2<ScalarType> &me) const override {
        Point2<ScalarType> intersectionPoint;
        Normal2<ScalarType> normal;
        float distance;

        Point2<ScalarType> center{ScalarType{0.0},ScalarType{0.0}};
        center = Shape2<ScalarType>::transform_ * center;
        bool bHit = intersectRayCircle(ray.origin,
                                       ray.direction,
                                       center,
                                       radius_,
                                       intersectionPoint,
                                       distance,
                                       normal);

        me.t = distance;
        me.p = intersectionPoint;
        me.n = normal;
        me.material = Shape2<ScalarType>::material_.get();

        return bHit;
    }

    std::string convertToSvg(const int svgCanvasWidth, const int svgCanvasHeight) const override {
        std::stringstream ss;

        Point2<ScalarType> center{ScalarType{0.0}, ScalarType{0.0}};
        center = Shape2<ScalarType>::getTransform() * center;

        ss << "<circle cx=\"" << center.x() << "\" cy=\"" << svgCanvasHeight - center.y() << "\" r=\""
           << getRadius()
           << "\"";

        ReferenceCounted<Material> material = Shape2<ScalarType>::getMaterial();
        if (material) {
            ss << " ";
            ss << Shape2<ScalarType>::convertMaterialToSvgStyle(material.get());
        }

        ss << ">";
        ss << "</circle>";
        ss << "\n";
        return ss.str();
    }

    ScalarType getRadius() const {
        return radius_;
    }

private:
    ScalarType radius_;
};

using Disk2f = Disk2<float>;

FLATLAND_END_NAMESPACE

#endif // end define Flatland_Disk2f_9deef3ad_50b0_4b94_b279_21c462b565e7_h