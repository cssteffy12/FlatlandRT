/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "include/gmock/gmock.h"

#include <algorithm>

TEST(stl, GivenValueInsideClampRegionAndMinMaxClampValue_WhenClamp_ThenUnmodifiedValue) {
    auto value = 1.f;
    auto min_value = 0.f;
    auto max_value = 10.f;
    auto clamped_value = std::clamp(value, min_value, max_value);

    EXPECT_THAT(clamped_value, 1.f);
}

TEST(stl, GivenValueAboveClampRegionAndMinMaxClampValue_WhenClamp_ThenMaxValue) {
    auto value = 20.f;
    auto min_value = 0.f;
    auto max_value = 10.f;
    auto clamped_value = std::clamp(value, min_value, max_value);

    EXPECT_THAT(clamped_value, max_value);
}

TEST(stl, GivenValueBelowClampRegionAndMinMaxClampValue_WhenClamp_ThenMaxValue) {
    auto value = -20.f;
    auto min_value = 0.f;
    auto max_value = 10.f;
    auto clamped_value = std::clamp(value, min_value, max_value);

    EXPECT_THAT(clamped_value, min_value);
}