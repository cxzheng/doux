#pragma once

#include "doux/doux.h"

#ifdef DOUX_USE_FLOAT64
#  define ASSERT_APPROX_EQ(x, y) ASSERT_DOUBLE_EQ(x, y)
#  define EXPECT_APPROX_EQ(x, y) EXPECT_DOUBLE_EQ(x, y)
#else
#  define ASSERT_APPROX_EQ(x, y) ASSERT_FLOAT_EQ(x, y)
#  define EXPECT_APPROX_EQ(x, y) EXPECT_FLOAT_EQ(x, y)
#endif
