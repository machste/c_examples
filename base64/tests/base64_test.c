#include <stdint.h>
#include <string.h> // See bug #791532 (bugzilla.gnome.org)
#include <glib.h>

#include "../base64.h"

#define ARRAY_SIZE( _array ) (sizeof(_array) / sizeof(_array)[0])

typedef struct {
    const uint8_t *bin;
    size_t bin_size;
    const char *b64;
    size_t b64_size;
} test_pair;

typedef struct {
    const test_pair *pairs;
    size_t pairs_len;
} test_data;

typedef struct {
    uint8_t *bytes;
    size_t size;
} buffer;

typedef struct {
    char *chars;
    size_t size;
} string;

typedef struct {
    union {
        buffer *bin;
        string *b64;
    } outs;
    size_t outs_len;
} base64_fixture;

const uint8_t BIN_01[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const char B64_01[] = "AQIDBAUGBwgJCg==";

const uint8_t BIN_02[] = {0x12, 0x34, 0x56};
const char B64_02[] = "EjRW";

const uint8_t BIN_03[] = {0x87, 0x65, 0x43, 0x21};
const char B64_03[] = "h2VDIQ==";

const uint8_t BIN_04[] = {0xde, 0xad, 0xbe, 0xef, 0x01};
const char B64_04[] = "3q2+7wE=";

#define INIT_PAIR(x) { \
    .bin = BIN_ ## x, \
    .bin_size = sizeof(BIN_ ## x), \
    .b64 = B64_ ## x, \
    .b64_size = sizeof(B64_ ## x) \
}

const test_pair TEST_PAIRS[] = {
    INIT_PAIR(01),
    INIT_PAIR(02),
    INIT_PAIR(03),
    INIT_PAIR(04)
};

const test_data TEST_DATA = {
    .pairs = TEST_PAIRS,
    .pairs_len = ARRAY_SIZE(TEST_PAIRS)
};

void setup_encoding(base64_fixture *fixture, gconstpointer _data)
{
    const test_data *data = _data;
    // Allocate memory for the output string array
    fixture->outs.b64 = g_new(string, data->pairs_len);
    // Allocate memory for each output of the encoding method
    for (int i = 0; i < data->pairs_len; i++) {
        fixture->outs.b64[i].chars = g_malloc0(data->pairs[i].b64_size);
        fixture->outs.b64[i].size = data->pairs[i].b64_size;
    }
}

void test_encoding(base64_fixture *fixture, gconstpointer _data)
{
    const test_data *data = _data;
    // Test each binary-base64 pair 
    for (int i = 0; i < data->pairs_len; i++) {
        // Encode binary to base64
        int len = base64_encode(data->pairs[i].bin, data->pairs[i].bin_size,
                fixture->outs.b64[i].chars, fixture->outs.b64[i].size);
        // Check generated output length
        g_assert_cmpint(len, ==, data->pairs[i].b64_size - 1);
        // Check content of the output
        g_assert_cmpstr(fixture->outs.b64[i].chars, ==, data->pairs[i].b64);
    }
}

void teardown_encoding(base64_fixture *fixture, gconstpointer _data)
{
    const test_data *data = _data;
    for (int i = 0; i < data->pairs_len; i++) {
        // Free memory of the output strings
        g_free(fixture->outs.b64[i].chars);
    }
    // Free memory of the output string array
    g_free(fixture->outs.b64);
}

void setup_decoding(base64_fixture *fixture, gconstpointer _data)
{
    const test_data *data = _data;
    // Allocate memory for the output string array
    fixture->outs.bin = g_new(buffer, data->pairs_len);
    // Allocate memory for each output of the decoding method
    for (int i = 0; i < data->pairs_len; i++) {
        fixture->outs.bin[i].bytes = g_malloc0(data->pairs[i].bin_size);
        fixture->outs.bin[i].size = data->pairs[i].bin_size;
    }
}

void test_decoding(base64_fixture *fixture, gconstpointer _data)
{
    const test_data *data = _data;
    // Test each binary-base64 pair 
    for (int i = 0; i < data->pairs_len; i++) {
        // Encode binary to base64
        int size = base64_decode(data->pairs[i].b64,
                fixture->outs.bin[i].bytes, fixture->outs.bin[i].size);
        // Check generated output
        g_assert_cmpmem(fixture->outs.bin[i].bytes, size,
                data->pairs[i].bin, data->pairs[i].bin_size);
    }
}

void teardown_decoding(base64_fixture *fixture, gconstpointer _data)
{
    const test_data *data = _data;
    for (int i = 0; i < data->pairs_len; i++) {
        // Free memory of the output strings
        g_free(fixture->outs.bin[i].bytes);
    }
    // Free memory of the output string array
    g_free(fixture->outs.bin);
}

int main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);
    g_test_add("/set1/encoding test", base64_fixture, &TEST_DATA,
            setup_encoding, test_encoding, teardown_encoding);
    g_test_add("/set1/decoding test", base64_fixture, &TEST_DATA,
            setup_decoding, test_decoding, teardown_decoding);
    return g_test_run();
}
