
#define TYPE CONCATENATE(CONCATENATE(CONCATENATE(_, map_key_t), _), map_value_t)

#define map_t CONCATENATE(map, TYPE)
typedef MAP(map_key_t, map_value_t) map_t;

#define equals CONCATENATE(equals, TYPE)
#define good_hash CONCATENATE(good_hash, TYPE)
#define bad_hash CONCATENATE(bad_hash, TYPE)
#define awful_hash CONCATENATE(awful_hash, TYPE)
#define test_hash_map CONCATENATE(test_hash_map, TYPE)
#define test_map_linear_probing CONCATENATE(test_map_linear_probing, TYPE)

static bool equals(const void* a, const void* b) {
    return *((map_key_t*) a) == *((map_key_t*) b);
}
static size_t good_hash(const void* data) {
    return *((map_key_t*) data);
}
static size_t bad_hash(const void* data) {
    return *((map_key_t*) data) / 8 * 8;
}
static size_t awful_hash(const void* data) {
    return 0;
}

// Test lookup, insertion, deletion and clear in the end.
static map_t test_hash_map(map_t map) {
    for (map_key_t i = 1; i < 100; i++) {
        map_key_t* k;
        map_value_t* v;

        v = MAP_FIND(map, i);
        if (v != NULL) fail();

        k = &i;
        v = MAP_RESOLVE(map, k);
        if (v != NULL || k != NULL) fail();

        if (i % 2 == 0) MAP_AT(map, i) = i;
        else {
            k = &i;
            v = MAP_RESOLVE_OR_INSERT(map, k);
            if (k == NULL) fail();
            if (v != NULL) fail();
            v = MAP_FIND(map, *k);
            if (v == NULL) fail();
            *v = i;
        }

        v = MAP_FIND(map, i);
        if (v == NULL) fail();
        if (*v != i) fail();

        k = &i;
        v = MAP_RESOLVE(map, k);
        if (v == NULL || k == NULL) fail();
        if (MAP_RESOLVE(map, k) != v) fail();
        if (*v != i) fail();

        k = &i;
        v = MAP_RESOLVE_OR_INSERT(map, k);
        if (v == NULL || k == NULL) fail();
        if (MAP_FIND(map, *k) != v) fail();
        if (*v != i) fail();
    }

    if (MAP_FIND(map, (map_key_t){0}) != NULL) fail();
    MAP_AT(map, (map_key_t){0}) = 0;

    for (map_key_t i = 0; i < 200; i++) {
        if ((MAP_FIND(map, i) != NULL) != i < 100) fail();
    }
    for (map_key_t i = 100; i < 200; i++) MAP_AT(map, i) = i;
    for (map_key_t i = 0; i < 250; i++) {
        if ((MAP_FIND(map, i) != NULL) != i < 200) fail();
    }

    int count = 0, sum = 0;
    for (const map_key_t* k = NULL; (k = MAP_NEXT_KEY(map, k)) != NULL;) {
        map_value_t* v = MAP_FIND(map, *k);
        if (v == NULL || *v != *k) fail();
        count++;
        sum += *v;
    }
    if (count != 200) fail();
    if (sum != (199 * 200) / 2) fail();

    for (map_key_t i = 0; i < 250; i += 2) {
        if (MAP_REMOVE(map, i) != (i < 200)) fail();
    }

    count = sum = 0;
    for (const map_key_t* k = NULL; (k = MAP_NEXT_KEY(map, k)) != NULL;) {
        map_value_t* v = MAP_FIND(map, *k);
        if (v == NULL || *v != *k) fail();
        count++;
        sum += *v;
    }
    if (count != 100) fail();
    if (sum != 100 * 100) fail();

    MAP_CLEAR(map);
    if (MAP_NEXT_KEY(map, NULL) != NULL) fail();

    return map;
}

static void test_map_linear_probing() {
    map_t map = NULL, new_map;

    // Test fresh map, expanding from the smallest size, rehashing only when full.
    HASH_MAP_REHASH(map, linear_probing, &equals, &good_hash, 0, -1, 1.0);
    new_map = test_hash_map(map);
    if (map == new_map) fail();
    map = new_map;

    // Check the same scenario, expect no reallocations.
    new_map = test_hash_map(map);
    if (map != new_map) fail();

    // Prepare space in advance, permit no collisions, expect no reallocations.
    HASH_MAP_REHASH(map, linear_probing, &equals, &good_hash, 200, 0, 0.0);
    new_map = test_hash_map(map);
    if (map != new_map) fail();

    // Bad hash, allow up to 7 collisions, expect no reallocations.
    HASH_MAP_REHASH(map, linear_probing, &equals, &bad_hash, 200, 7, 0.0);
    new_map = test_hash_map(map);
    if (map != new_map) fail();

    // Bad hash, permit no collisions, but choke reallocation with load factor, expect no reallocations.
    HASH_MAP_REHASH(map, linear_probing, &equals, &bad_hash, 200, 0, 1.0);
    new_map = test_hash_map(map);
    if (map != new_map) fail();

    // Bad hash, permit no collisions, but limit reallocation with load factor, expect no reallocations.
    HASH_MAP_REHASH(map, linear_probing, &equals, &bad_hash, 200, 0, 0.6);
    new_map = test_hash_map(map);
    if (map != new_map) fail();

    // Bad hash, permit no collisions, but loosely limit reallocation with load factor, expect reallocation.
    HASH_MAP_REHASH(map, linear_probing, &equals, &bad_hash, 200, 0, 0.3);
    new_map = test_hash_map(map);
    if (map == new_map) fail();
    map = new_map;

    // Awful hash, expect to reallocate reaching 197 collisions.
    HASH_MAP_REHASH(map, linear_probing, &equals, &awful_hash, 200, 197, 0.0);
    new_map = test_hash_map(map);
    if (map == new_map) fail();
    map = new_map;

    // Awful hash, permit no collisions, but limit reallocation with load factor, expect no reallocations.
    HASH_MAP_REHASH(map, linear_probing, &equals, &awful_hash, 200, 0, 0.6);
    new_map = test_hash_map(map);
    if (map != new_map) fail();

    // Awful hash, permit no collisions, but loosely limit reallocation with load factor, expect reallocation.
    HASH_MAP_REHASH(map, linear_probing, &equals, &awful_hash, 200, 0, 0.3);
    new_map = test_hash_map(map);
    if (map == new_map) fail();
    map = new_map;

    MAP_FREE(map);
    if (map != NULL) fail();
}

#undef map_key_t
#undef map_value_t
#undef map_t

#undef equals
#undef good_hash
#undef bad_hash
#undef awful_hash
#undef test_hash_map
#undef test_map_linear_probing
