#include <cstring>
// #include <malloc.h>

#include <gtest/gtest.h>

extern "C" {
    #include "CContainer/Vector.h"
}

static void no_destroy(Type _value)
{
    std::ignore = _value;
}

TEST(Vector_test, create)
{
    Vector *vec = Vector_create(sizeof(size_t), &no_destroy);

    ASSERT_TRUE(vec != NULL);
    EXPECT_EQ(vec->size, 0);
    EXPECT_EQ(vec->_rsize, 0);
    EXPECT_TRUE(vec->data == NULL);
    ASSERT_EQ(vec->_objsize, sizeof(size_t));
    ASSERT_TRUE(vec->_dtor == &no_destroy);
    std::free(vec);
}

// All the test can certainly compacted with multi entrys
class Vector_empty_test : public ::testing::TestWithParam<std::tuple<size_t, size_t>>
{
    protected:
        Vector_empty_test()
        {
            vec = Vector_create(sizeof(size_t), &no_destroy);
        }

        ~Vector_empty_test()
        {
            if (vec->data != NULL)
                std::free(vec->data);
            std::free(vec);
        }

        Vector *vec;
};

TEST_P(Vector_empty_test, resize)
{
    const size_t first_size = std::get<0>(GetParam());
    const size_t second_size = std::get<1>(GetParam());

    Vector_resize(vec, first_size);

    ASSERT_EQ(vec->_rsize, first_size);
    ASSERT_EQ(vec->size, first_size);
    EXPECT_TRUE(vec->data != NULL);

    Vector_resize(vec, second_size);

    if (first_size >= second_size) {
        ASSERT_EQ(vec->_rsize, first_size);
        ASSERT_EQ(vec->size, second_size);
    } else {
        ASSERT_EQ(vec->_rsize, second_size);
        ASSERT_EQ(vec->size, second_size);
    }
    EXPECT_TRUE(vec->data != NULL);
}

TEST_P(Vector_empty_test, reserve)
{
    const size_t first_size = std::get<0>(GetParam());
    const size_t second_size = std::get<1>(GetParam());

    Vector_reserve(vec, first_size);

    ASSERT_EQ(vec->_rsize, first_size);
    EXPECT_EQ(vec->size, 0);
    EXPECT_TRUE(vec->data != NULL);

    Vector_reserve(vec, second_size);

    if (first_size >= second_size)
        ASSERT_EQ(vec->_rsize, first_size);
    else
        ASSERT_EQ(vec->_rsize, second_size);
    EXPECT_TRUE(vec->data != NULL);
    EXPECT_EQ(vec->size, 0);
}

INSTANTIATE_TEST_SUITE_P(Vector, Vector_empty_test,
    ::testing::Combine(
        testing::Values(5, 8, 10),
        testing::Values(3, 8, 15)
    )
);