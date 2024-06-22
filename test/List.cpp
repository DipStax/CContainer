#include <cstring>

#include <gtest/gtest.h>

#include "Test.hpp"

extern "C" {
    #include "CContainer/List.h"
}

static void no_destroy(Type _value)
{
    std::ignore = _value;
}

static void destroy(Type _value)
{
    std::free(_value);
}

class List_test : public ::testing::Test
{
    protected:

        void SetUp() override
        {
            value = std::malloc(sizeof(size_t));
            std::memcpy(value, &literal_value, sizeof(size_t));
        }

        void TearDown() override
        {
            std::free(value);
        }

        Type value;

        static constexpr size_t literal_value = 128;
};

TEST_F(List_test, create)
{
    List *list = List_create(&destroy);

    ASSERT_TRUE(list != NULL);
    List_destroy(list);
}

TEST_F(List_test, destroy_null_empty)
{
    List *list = List_create(NULL);

    ASSERT_TRUE(list != NULL);
    List_destroy(list);
}

class List_add_test : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            list = List_create(&destroy);

            value1 = std::malloc(sizeof(size_t));
            std::memcpy(value1, &literal_value1, sizeof(size_t));

            value2 = std::malloc(sizeof(size_t));
            std::memcpy(value2, &literal_value2, sizeof(size_t));
        }

        void TearDown() override
        {
            std::free(value1);
            std::free(value2);
        }

        List *list;

        Type value1 = NULL;
        Type value2 = NULL;

        static constexpr size_t literal_value1 = 128;
        static constexpr size_t literal_value2 = 256;
        inline static bool destroyed = false;
};

TEST_F(List_add_test, append_first)
{
    List_append(list, value1);

    EXPECT_TRUE(list->size == 1);
    ASSERT_TRUE(list->head != NULL);
    ASSERT_TRUE(list->head->data == value1);
}

TEST_F(List_add_test, append_multi_value)
{
    List_append(list, value1);
    List_append(list, value2);

    EXPECT_TRUE(list->size == 2);
    ASSERT_TRUE(list->head != NULL);
    EXPECT_TRUE(list->head->data == value1);
    ASSERT_TRUE(list->head->next != NULL);
    EXPECT_TRUE(list->head->next->data == value2);
}

TEST_F(List_add_test, prepend_first)
{
    List_prepend(list, value1);

    EXPECT_TRUE(list->size == 1);
    ASSERT_TRUE(list->head != NULL);
    ASSERT_TRUE(list->head->data == value1);
}

TEST_F(List_add_test, prepend_multi_value)
{
    List_prepend(list, value1);
    List_prepend(list, value2);

    EXPECT_TRUE(list->size == 2);
    ASSERT_TRUE(list->head != NULL);
    EXPECT_TRUE(list->head->data == value2);
    ASSERT_TRUE(list->head->next != NULL);
    EXPECT_TRUE(list->head->next->data == value1);
}

class List_filled : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            list = List_create(&no_destroy);

            value1 = std::malloc(sizeof(size_t));
            std::memcpy(value1, &literal_value1, sizeof(size_t));

            value2 = std::malloc(sizeof(size_t));
            std::memcpy(value2, &literal_value2, sizeof(size_t));

            value3 = std::malloc(sizeof(size_t));
            std::memcpy(value3, &literal_value3, sizeof(size_t));

            List_append(list, value1);
            List_append(list, value2);
            List_append(list, value3);
        }

        void TearDown() override
        {
            // std::free(value1);
            // std::free(value2);
            // std::free(value2);
        }

        List *list;

        Type value1 = NULL;
        Type value2 = NULL;
        Type value3 = NULL;

        static constexpr size_t literal_value1 = 128;
        static constexpr size_t literal_value2 = 256;
        static constexpr size_t literal_value3 = 256;
};

TEST_F(List_filled, erase_oor)
{
    ASSERT_EXIT({ List_erase(list, 3); exit(0); },
        EXIT_WITH_SEGV,
        ".*");
}

TEST_F(List_filled, erase_first)
{
    List_erase(list, 0);

    EXPECT_EQ(list->size, 2);
    ASSERT_TRUE(list->head != NULL);
    ASSERT_TRUE(list->head->data == value2);
    ASSERT_TRUE(list->head->next != NULL);
    ASSERT_TRUE(list->head->next->data == value3);
}

TEST_F(List_filled, erase_last)
{
    List_erase(list, 2);

    EXPECT_EQ(list->size, 2);
    ASSERT_TRUE(list->head != NULL);
    ASSERT_TRUE(list->head->data == value1);
    ASSERT_TRUE(list->head->next != NULL);
    ASSERT_TRUE(list->head->next->data == value2);
}

TEST_F(List_filled, erase_middle)
{
    List_erase(list, 1);

    EXPECT_EQ(list->size, 2);
    ASSERT_TRUE(list->head != NULL);
    ASSERT_TRUE(list->head->data == value1);
    ASSERT_TRUE(list->head->next != NULL);
    ASSERT_TRUE(list->head->next->data == value3);
}

TEST_F(List_filled, at_oor)
{
    ASSERT_EXIT({ List_at(list, 3); exit(0); },
        EXIT_WITH_SEGV,
        ".*");
}

TEST_F(List_filled, at_first)
{
    ASSERT_TRUE(List_at(list, 0) == value1);
}

TEST_F(List_filled, at_middle)
{
    ASSERT_TRUE(List_at(list, 1) == value2);
}

TEST_F(List_filled, at_last)
{
    ASSERT_TRUE(List_at(list, 2) == value3);
}