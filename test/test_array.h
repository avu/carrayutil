
static void CONCATENATE(test_array_, TYPE)() {
    ARRAY(TYPE) a = ARRAY_ALLOC(TYPE, 10);

    if (ARRAY_CAPACITY(a) != 10) fail();

    ARRAY_PUSH_BACK(a) = 0;
    ARRAY_PUSH_BACK(a) = 1;
    ARRAY_PUSH_BACK(a) = 2;
    ARRAY_PUSH_BACK(a) = 3;

    if (ARRAY_SIZE(a) != 4) fail();

    for (TYPE i = 0; i < ARRAY_SIZE(a); i++) {
        if (a[i] != i) fail();
    }
    ARRAY_FREE(a);
}

#undef TYPE
