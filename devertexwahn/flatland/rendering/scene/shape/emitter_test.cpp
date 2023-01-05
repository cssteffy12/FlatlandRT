/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/shape/emitter.h"

#include "gmock/gmock.h"

using namespace de_vertexwahn;

class DummyEmitter : public Emitter<2, float> {
public:
    DummyEmitter() : Emitter<2, float>() {}

    virtual ColorType<3, float> evaluate() const {
        return ColorType<3, float>(1.f);
    }
};

TEST(Emitter, to_string) {
    DummyEmitter emitter;
    EXPECT_THAT(emitter.to_string(), "Emitter");
    EXPECT_THAT(emitter.evaluate(), Color3f(1.f));
};
