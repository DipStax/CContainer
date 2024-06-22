#include <cstring>
#include <iostream>
#include <gtest/gtest.h>

#include "Test.hpp"

extern "C" {
    #include "CContainer/Vector.h"
    #include "CContainer/Utils.h"
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

class Vector_empty_test : public ::testing::TestWithParam<std::tuple<size_t, size_t>>
{
    protected:
        void SetUp() override
        {
            vec = Vector_create(sizeof(size_t), &no_destroy);
        }

        void TearDown() override
        {
            std::free(vec->data);
            std::free(vec);
        }

        Vector *vec = NULL;
};

TEST_F(Vector_empty_test, at_oor)
{
    ASSERT_EXIT({ Vector_at(vec, 0); exit(0); },
        EXIT_WITH_SEGV,
        ".*");
}

TEST_F(Vector_empty_test, erase_oor)
{
    ASSERT_EXIT({ Vector_erase(vec, 0); exit(0); },
        EXIT_WITH_SEGV,
        ".*");
}

TEST_P(Vector_empty_test, resize)
{
    const size_t first_size = std::get<0>(GetParam());
    const size_t second_size = std::get<1>(GetParam());

    Vector_resize(vec, first_size);

    ASSERT_EQ(vec->_rsize, first_size);
    ASSERT_EQ(vec->size, first_size);
    EXPECT_TRUE(vec->data != NULL);

    for (size_t it = 0; it < first_size; it++)
        ASSERT_EQ(VEC_AT_VAL(vec, size_t, it), 0);

    Vector_resize(vec, second_size);

    size_t iteration = 0;
    ASSERT_EQ(vec->size, second_size);
    iteration = max(second_size, first_size);
    ASSERT_EQ(vec->_rsize, iteration);
    // for (size_t it = 0; it < iteration; it++)
    //     ASSERT_EQ(VEC_AT_VAL(vec, size_t, it), 0);
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

    size_t iteration = 0;

    EXPECT_EQ(vec->size, 0);
    EXPECT_TRUE(vec->data != NULL);
    iteration = max(first_size, second_size);
    ASSERT_EQ(vec->_rsize, iteration);
}

INSTANTIATE_TEST_SUITE_P(Vector, Vector_empty_test,
    ::testing::Combine(
        testing::Values(5, 8, 10),
        testing::Values(3, 8, 15)
    )
);

class Vector_stock_test : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            vec = Vector_create(sizeof(size_t), &no_destroy);

            value1 = ALLOC_HEAP(size_t, literal_value1);
            value2 = ALLOC_HEAP(size_t, literal_value2);
        }

        void TearDown() override
        {
            std::free(vec->data);
            std::free(vec);
            std::free(value1);
            std::free(value2);
        }

        Vector *vec;

        Type value1;
        Type value2;

        static constexpr size_t literal_value1 = 128;
        static constexpr size_t literal_value2 = 256;
};


TEST_F(Vector_stock_test, append)
{
    Vector_append(vec, value1);

    EXPECT_EQ(vec->size, 1);
    ASSERT_EQ(vec->_rsize, 1);
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value1);

    Vector_append(vec, value2);

    EXPECT_EQ(vec->size, 2);
    ASSERT_EQ(vec->_rsize, 2);
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value1);
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 1), literal_value2);
}

TEST_F(Vector_stock_test, prepend)
{
    Vector_append(vec, value1);

    EXPECT_EQ(vec->size, 1);
    ASSERT_EQ(vec->_rsize, 1);
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value1);

    Vector_append(vec, value2);

    EXPECT_EQ(vec->size, 2);
    ASSERT_EQ(vec->_rsize, 2);
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value1);
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 1), literal_value2);
}

class Vector_access_test : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            vec = Vector_create(sizeof(size_t), &no_destroy);

            value1 = ALLOC_HEAP(size_t, literal_value1);
            value2 = ALLOC_HEAP(size_t, literal_value2);

            Vector_append(vec, value1);
            Vector_append(vec, value2);
        }

        void TearDown() override
        {
            std::free(vec->data);
            std::free(vec);
            std::free(value1);
            std::free(value2);
        }

        Vector *vec;

        Type value1;
        Type value2;

        static constexpr size_t literal_value1 = 128;
        static constexpr size_t literal_value2 = 256;
};

TEST_F(Vector_access_test, at)
{
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value1);

    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 1), literal_value2);
}

class Vector_filled_test : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            vec = Vector_create(sizeof(size_t), &no_destroy);

            std::cout << "Vector_filled_test Setup -- start" << std::endl;

            value1 = ALLOC_HEAP(size_t, literal_value1);
            value2 = ALLOC_HEAP(size_t, literal_value2);

            Vector_append(vec, value1);
            Vector_append(vec, value2);

            std::cout << "Vector_filled_test Setup -- end" << std::endl;
            std::cout << vec->size << std::endl;
        }

        void TearDown() override
        {
            std::cout << "TearDown -- start" << std::endl;
            std::free(vec->data);
            std::free(vec);
            std::cout << "TearDown -- end" << std::endl;
        }

        Vector *vec;

        Type value1;
        Type value2;

        static constexpr size_t literal_value1 = 128;
        static constexpr size_t literal_value2 = 256;
};

TEST_F(Vector_filled_test, clear)
{
    Vector_clear(vec);

    ASSERT_EQ(vec->size, 0);
    ASSERT_EQ(vec->_rsize, 2);
    ASSERT_EQ(vec->_objsize, sizeof(size_t));

    for (size_t it = 0; it < 2; it++)
        ASSERT_EXIT({ VEC_AT_VAL(vec, size_t, it); },
            EXIT_WITH_SEGV,
            ".*");
}

TEST_F(Vector_filled_test, erase_first_last)
{
    Vector_erase(vec, 0);

    ASSERT_EQ(vec->size, 1);
    ASSERT_EQ(vec->_rsize, 2);
    ASSERT_EQ(vec->_objsize, sizeof(size_t));
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value2);

    ASSERT_EXIT({ VEC_AT_VAL(vec, size_t, 1); },
        EXIT_WITH_SEGV,
        ".*");

    Vector_erase(vec, 0);

    ASSERT_EQ(vec->size, 0);
    ASSERT_EQ(vec->_rsize, 2);
    ASSERT_EQ(vec->_objsize, sizeof(size_t));

    for (size_t it = 0; it < 2; it++)
        ASSERT_EXIT({ VEC_AT_VAL(vec, size_t, it); },
            EXIT_WITH_SEGV,
            ".*");
}

TEST_F(Vector_filled_test, erase_next)
{
    Vector_erase(vec, 1);

    ASSERT_EQ(vec->size, 1);
    ASSERT_EQ(vec->_rsize, 2);
    ASSERT_EQ(vec->_objsize, sizeof(size_t));
    ASSERT_EQ(VEC_AT_VAL(vec, size_t, 0), literal_value1);

    ASSERT_EXIT({ VEC_AT_VAL(vec, size_t, 1); },
        EXIT_WITH_SEGV,
        ".*");
}