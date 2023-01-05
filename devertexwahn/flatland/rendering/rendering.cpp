/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/rendering.h"

#include "core/logging.h"
#include "flatland/rendering/scene/scene.h"

DE_VERTEXWAHN_BEGIN_NAMESPACE

void render(Integrator2f *integrator, ReferenceCounted<SvgCanvas2f> canvas_, const Scene2f *scene) {
    assert(scene);
    assert(canvas_);

    LOG(INFO) << "Begin rendering.";

    for (auto shape: scene->shapes()) {
        canvas_->add(shape.get());
    }

    auto sensor = scene->sensor();
    assert(sensor);

    Ray2f ray = sensor->generate_ray(Point2f{0.f, 0.f});

    auto sampler = scene->sampler();
    assert(sampler.get());
    integrator->trace(scene, sampler.get(), ray, 1);
}

DE_VERTEXWAHN_END_NAMESPACE
