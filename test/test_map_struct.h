


static void CONCATENATE(test_map_linear_probing_struct, NAME)() {
    typedef struct {
        alignas(ALIGNMENT) uint64_t data[123];
    } struct_t;
    MAP(struct_t, uint8_t) big_key_map = NULL;
    MAP(uint8_t, struct_t) big_val_map = NULL;
    HASH_MAP_REHASH(big_key_map, linear_probing, &equals_uint64_t_uint8_t, &good_hash_uint64_t_uint8_t, 0, -1, 1.0);
    HASH_MAP_REHASH(big_val_map, linear_probing, &equals_uint8_t_uint64_t, &good_hash_uint8_t_uint64_t, 0, -1, 1.0);

    for (uint8_t i = 0;; i++) {
        uint8_t key = ((i & 0b10101010) >> 1) | ((i & 0b01010101) << 1);
        struct_t big_key = {key};
        uint8_t  *bkm_val, *bvm_key = &key;
        struct_t *bvm_val, *bkm_key = &big_key;
        bkm_val = MAP_RESOLVE_OR_INSERT(big_key_map, bkm_key);
        bvm_val = MAP_RESOLVE_OR_INSERT(big_val_map, bvm_key);
        if (bkm_key == NULL || bvm_key == NULL) fail();
        if (bkm_val != NULL || bvm_val != NULL) fail();
        bkm_val = MAP_FIND(big_key_map, *bkm_key);
        bvm_val = MAP_FIND(big_val_map, *bvm_key);
        if (bkm_val == NULL || bvm_val == NULL) fail();
        *bkm_val = key;
        *bvm_val = big_key;
        if ((uintptr_t)bkm_key % ALIGNMENT != 0) fail();
        if ((uintptr_t)bvm_val % ALIGNMENT != 0) fail();
        if (i == 255) break;
    }

    uint32_t count = 0;
    for (const struct_t* k = NULL; (k = MAP_NEXT_KEY(big_key_map, k)) != NULL;) {
        count++;
        if (k->data[0] != *MAP_FIND(big_key_map, *k)) fail();
    }
    if (count != 256) fail();

    count = 0;
    for (const uint8_t* k = NULL; (k = MAP_NEXT_KEY(big_val_map, k)) != NULL;) {
        count++;
        if (*k != MAP_FIND(big_val_map, *k)->data[0]) fail();
    }
    if (count != 256) fail();

    for (uint8_t i = 255;; i--) {
        struct_t big_key = {i};
        uint8_t  *bkm_val, *bvm_key = &i;
        struct_t *bvm_val, *bkm_key = &big_key;
        bkm_val = MAP_RESOLVE(big_key_map, bkm_key);
        bvm_val = MAP_RESOLVE(big_val_map, bvm_key);
        if (bkm_key == NULL || bvm_key == NULL) fail();
        if (bkm_val == NULL || bvm_val == NULL) fail();
        if ((uintptr_t)bkm_key % ALIGNMENT != 0) fail();
        if ((uintptr_t)bvm_val % ALIGNMENT != 0) fail();
        if (!MAP_REMOVE(big_key_map, *bkm_key)) fail();
        if (!MAP_REMOVE(big_val_map, *bvm_key)) fail();
        if (i == 0) break;
    }

    if (MAP_NEXT_KEY(big_key_map, NULL) != NULL) fail();
    if (MAP_NEXT_KEY(big_val_map, NULL) != NULL) fail();

    MAP_FREE(big_key_map);
    MAP_FREE(big_val_map);
}

#undef NAME
#undef ALIGNMENT
