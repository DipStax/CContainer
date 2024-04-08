#include <cstring>

#include <gtest/gtest.h>

extern "C" {
    #include "CContainer/List.h"
}

static void no_destroy(Type _value)
{
    std::ignore = _value;
}

// static void destroy(Type _value)
// {
//     std::free(_value);
// }

class IteratorList_test : public ::testing::Test
{
    protected:
        IteratorList_test()
        {
            value = std::malloc(sizeof(size_t));
            std::memcpy(value, &literal_value, sizeof(size_t));
        }

        ~IteratorList_test()
        {
            std::free(value);
        }

        Type value = NULL;

        static constexpr size_t literal_value = 128;
        inline static bool destroyed = false;
};

TEST_F(IteratorList_test, simple_create)
{
    IteratorList *node = IteratorList_create(value);

    ASSERT_TRUE(node != NULL);
    ASSERT_EQ(node->data, value);
    ASSERT_EQ(*(reinterpret_cast<size_t *>(node->data)), literal_value);
    IteratorList_destroy(node, &no_destroy);
}

TEST_F(IteratorList_test, custom_destroy)
{
    IteratorList *node = IteratorList_create(value);

    ASSERT_TRUE(node != NULL);
    IteratorList_destroy(node, &no_destroy);
}

class priv_IteratorList_test : public ::testing::Test
{
    protected:
        priv_IteratorList_test()
        {
            value1 = std::malloc(sizeof(size_t));
            std::memcpy(value1, &literal_value1, sizeof(size_t));

            value2 = std::malloc(sizeof(size_t));
            std::memcpy(value2, &literal_value2, sizeof(size_t));
        }

        virtual ~priv_IteratorList_test()
        {
            std::free(value1);
            std::free(value2);
        }

        IteratorList *node1 = NULL;
        IteratorList *node2 = NULL;

        Type value1 = NULL;
        Type value2 = NULL;

        static constexpr size_t literal_value1 = 128;
        static constexpr size_t literal_value2 = 256;
        inline static bool destroyed = false;
};

class priv_IteratorList_connect_test : public priv_IteratorList_test
{
    protected:
        void SetUp() override
        {
            node1 = IteratorList_create(value1);
            node2 = IteratorList_create(value2);
        }

        void TearDown() override
        {
            IteratorList_destroy(node1, &no_destroy);
            IteratorList_destroy(node2, &no_destroy);
        }

};

TEST_F(priv_IteratorList_connect_test, null)
{
    priv_IteratorList_connect(NULL, NULL);
}

TEST_F(priv_IteratorList_connect_test, null_first)
{
    priv_IteratorList_connect(node1, NULL);
    ASSERT_TRUE(node1->next == NULL);
}

TEST_F(priv_IteratorList_connect_test, null_second)
{
    priv_IteratorList_connect(NULL, node2);
    ASSERT_TRUE(node2->prev == NULL);
}

TEST_F(priv_IteratorList_connect_test, complet)
{
    priv_IteratorList_connect(node1, node2);
    ASSERT_TRUE(node1->next == node2);
    ASSERT_EQ(*(reinterpret_cast<size_t *>(node1->next->data)), literal_value2);

    ASSERT_TRUE(node2->prev == node1);
    ASSERT_EQ(*(reinterpret_cast<size_t *>(node2->prev->data)), literal_value1);
}

class priv_IteratorList_last_test : public priv_IteratorList_test
{
    protected:
        priv_IteratorList_last_test()
            : priv_IteratorList_test()
        {
            node1 = IteratorList_create(value1);
            node2 = IteratorList_create(value2);

            priv_IteratorList_connect(node1, node2);
        }

        ~priv_IteratorList_last_test()
        {
            IteratorList_destroy(node1, &no_destroy);
            IteratorList_destroy(node2, &no_destroy);
        }
};

TEST_F(priv_IteratorList_last_test, null)
{
    ASSERT_EXIT({ priv_IteratorList_last(NULL); exit(0); },
        ::testing::KilledBySignal(SIGSEGV),
        ".*");
}

TEST_F(priv_IteratorList_last_test, element)
{
    IteratorList *node = priv_IteratorList_last(node1);

    ASSERT_TRUE(node == node2);
}

TEST_F(priv_IteratorList_last_test, last_element)
{
    IteratorList *node = priv_IteratorList_last(node2);

    ASSERT_TRUE(node == node2);
}

class priv_IteratorList_at_test : public priv_IteratorList_test
{
    protected:
        priv_IteratorList_at_test()
            : priv_IteratorList_test()
        {
            node1 = IteratorList_create(value1);
            node2 = IteratorList_create(value2);

            priv_IteratorList_connect(node1, node2);
        }

        ~priv_IteratorList_at_test()
        {
            IteratorList_destroy(node1, &no_destroy);
            IteratorList_destroy(node2, &no_destroy);
        }
};

TEST_F(priv_IteratorList_last_test, second_null)
{
    ASSERT_EXIT({ priv_IteratorList_at(NULL, 0); exit(0); },
        ::testing::KilledBySignal(SIGSEGV),
        ".*");
}

TEST_F(priv_IteratorList_last_test, oor)
{
    ASSERT_EXIT({ priv_IteratorList_at(node1, 2); exit(0); },
        ::testing::KilledBySignal(SIGSEGV),
        ".*");
}

TEST_F(priv_IteratorList_last_test, same_element)
{
    IteratorList *node = priv_IteratorList_at(node1, 0);

    ASSERT_TRUE(node == node1);
}

TEST_F(priv_IteratorList_last_test, other_element)
{
    IteratorList *node = priv_IteratorList_at(node1, 1);

    ASSERT_TRUE(node == node2);
}