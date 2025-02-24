
static void CONCATENATE(test_ring_buffer_wrap_, TYPE)() {
    const int EXPAND_COUNT = 1000;
    const int INNER_COUNT  = 1000;
    RING_BUFFER(TYPE) b = NULL;

    TYPE read = 0;
    TYPE write = 0;
    for (int i = 0; i < EXPAND_COUNT; i++) {
        for (int j = 0; j < INNER_COUNT; j++) {
            RING_BUFFER_PUSH_BACK(b) = write;
            write++;
            TYPE* value = RING_BUFFER_FRONT(b);
            if (value == NULL) fail();
            if (*value != read) fail();
            read++;
            RING_BUFFER_POP_FRONT(b);
        }
        RING_BUFFER_PUSH_BACK(b) = write;
        write++;
    }
    if (RING_BUFFER_SIZE(b) != EXPAND_COUNT) fail();

    for (int i = 0; i < EXPAND_COUNT; i++) {
        TYPE* value = RING_BUFFER_FRONT(b);
        if (value == NULL) fail();
        if (*value != read) fail();
        read++;
        RING_BUFFER_POP_FRONT(b);
    }
    if (RING_BUFFER_FRONT(b) != NULL) fail();
    if (RING_BUFFER_SIZE(b) != 0) fail();

    RING_BUFFER_FREE(b);
}

#undef TYPE
