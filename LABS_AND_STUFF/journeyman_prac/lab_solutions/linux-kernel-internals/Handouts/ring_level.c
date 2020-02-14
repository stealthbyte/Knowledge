#define RING_LEVEL(x)   (x & 0x3)

static int get_ring_level(void) {
    int cs_reg;
    asm("mov %%cs, %[x]" : [x] "=r" (cs_reg));
    return RING_LEVEL(cs_reg);
}
