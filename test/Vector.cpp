#include <malloc.h>

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
    EXPECT_EQ(vec->_objsize, sizeof(size_t));
    ASSERT_TRUE(vec->_dtor == &no_destroy);
    Vector_destroy(vec);
}

// All the test can certainly compacted with multi entrys
class Vector_empty_memory_test : public ::testing::Test
{
    protected:
        Vector_empty_memory_test()
        {
            vec = Vector_create(sizeof(size_t), &no_destroy);
        }

        ~Vector_empty_memory_test()
        {
            Vector_destroy(vec);
        }

        Vector *vec;
};

TEST_F(Vector_empty_memory_test, resize_empty)
{
    static constexpr const size_t size = 5;

    Vector_resize(vec, size);

    EXPECT_EQ(vec->_rsize, size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);
}

TEST_F(Vector_empty_memory_test, resize_double)
{
    static constexpr const size_t size = 5;
    static constexpr const size_t second_size = 5;

    Vector_resize(vec, size);

    ASSERT_EQ(vec->_rsize, size);
    ASSERT_EQ(vec->size, size);
    // ASSERT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);

    Vector_resize(vec, size);

    EXPECT_EQ(vec->_rsize, second_size);
    EXPECT_EQ(vec->size, second_size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * second_size);
}

TEST_F(Vector_empty_memory_test, resize_double_lower)
{
    static constexpr const size_t size = 5;
    static constexpr const size_t second_size = 3;

    Vector_resize(vec, size);

    ASSERT_EQ(vec->_rsize, size);
    ASSERT_EQ(vec->size, size);
    // ASSERT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);

    Vector_resize(vec, size);

    EXPECT_EQ(vec->_rsize, second_size);
    EXPECT_EQ(vec->size, second_size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * second_size);
}

TEST_F(Vector_empty_memory_test, resize_double_higher)
{
    static constexpr const size_t size = 5;
    static constexpr const size_t second_size = 7;

    Vector_resize(vec, size);

    ASSERT_EQ(vec->_rsize, size);
    ASSERT_EQ(vec->size, size);
    // ASSERT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);

    Vector_resize(vec, size);

    EXPECT_EQ(vec->_rsize, second_size);
    EXPECT_EQ(vec->size, second_size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * second_size);
}

TEST_F(Vector_empty_memory_test, reserve_empty)
{
    static constexpr const size_t size = 5;

    Vector_reserve(vec, size);

    EXPECT_EQ(vec->_rsize, size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);
}

TEST_F(Vector_empty_memory_test, reserve_double)
{
    static constexpr const size_t size = 5;
    static constexpr const size_t second_size = 5;

    Vector_reserve(vec, size);

    EXPECT_EQ(vec->_rsize, size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);

    Vector_reserve(vec, second_size);
    EXPECT_EQ(vec->_rsize, second_size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * second_size);
}

TEST_F(Vector_empty_memory_test, reserve_double_low)
{
    static constexpr const size_t size = 5;
    static constexpr const size_t second_size = 3;

    Vector_reserve(vec, size);

    EXPECT_EQ(vec->_rsize, size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);

    Vector_reserve(vec, second_size);
    EXPECT_EQ(vec->_rsize, size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);
}

TEST_F(Vector_empty_memory_test, reserve_double_higher)
{
    static constexpr const size_t size = 5;
    static constexpr const size_t second_size = 7;

    Vector_reserve(vec, size);

    EXPECT_EQ(vec->_rsize, size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);

    Vector_reserve(vec, second_size);
    EXPECT_EQ(vec->_rsize, second_size);
    // EXPECT_EQ(malloc_usable_size(vec->data), sizeof(size_t) * size);
}