#include "test.h"

#define map_key_t uint8_t
#define map_value_t uint8_t
#include "test_map.h"
#define map_key_t uint8_t
#define map_value_t uint16_t
#include "test_map.h"
#define map_key_t uint8_t
#define map_value_t uint32_t
#include "test_map.h"
#define map_key_t uint8_t
#define map_value_t uint64_t
#include "test_map.h"
#define map_key_t uint16_t
#define map_value_t uint8_t
#include "test_map.h"
#define map_key_t uint16_t
#define map_value_t uint16_t
#include "test_map.h"
#define map_key_t uint16_t
#define map_value_t uint32_t
#include "test_map.h"
#define map_key_t uint16_t
#define map_value_t uint64_t
#include "test_map.h"
#define map_key_t uint32_t
#define map_value_t uint8_t
#include "test_map.h"
#define map_key_t uint32_t
#define map_value_t uint16_t
#include "test_map.h"
#define map_key_t uint32_t
#define map_value_t uint32_t
#include "test_map.h"
#define map_key_t uint32_t
#define map_value_t uint64_t
#include "test_map.h"
#define map_key_t uint64_t
#define map_value_t uint8_t
#include "test_map.h"
#define map_key_t uint64_t
#define map_value_t uint16_t
#include "test_map.h"
#define map_key_t uint64_t
#define map_value_t uint32_t
#include "test_map.h"
#define map_key_t uint64_t
#define map_value_t uint64_t
#include "test_map.h"

#define NAME
#define ALIGNMENT sizeof(uint64_t)
#include "test_map_struct.h"
#define NAME _align
#define ALIGNMENT 256
#include "test_map_struct.h"

void test_map() {
    RUN_TEST(map_linear_probing_uint8_t_uint8_t);
    RUN_TEST(map_linear_probing_uint8_t_uint16_t);
    RUN_TEST(map_linear_probing_uint8_t_uint32_t);
    RUN_TEST(map_linear_probing_uint8_t_uint64_t);
    RUN_TEST(map_linear_probing_uint16_t_uint8_t);
    RUN_TEST(map_linear_probing_uint16_t_uint16_t);
    RUN_TEST(map_linear_probing_uint16_t_uint32_t);
    RUN_TEST(map_linear_probing_uint16_t_uint64_t);
    RUN_TEST(map_linear_probing_uint32_t_uint8_t);
    RUN_TEST(map_linear_probing_uint32_t_uint16_t);
    RUN_TEST(map_linear_probing_uint32_t_uint32_t);
    RUN_TEST(map_linear_probing_uint32_t_uint64_t);
    RUN_TEST(map_linear_probing_uint64_t_uint8_t);
    RUN_TEST(map_linear_probing_uint64_t_uint16_t);
    RUN_TEST(map_linear_probing_uint64_t_uint32_t);
    RUN_TEST(map_linear_probing_uint64_t_uint64_t);
    RUN_TEST(map_linear_probing_struct);
    RUN_TEST(map_linear_probing_struct_align);
}
