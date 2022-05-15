// This examples demonstrates marker interfaces.

#include <interface99.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// A marker interface whose implementers can be duplicated by simply copying bits.
#define Copy_IFACE

interface(Copy);

typedef struct {
    unsigned health, victories, defeats;
} PlayerStats;

impl(Copy, PlayerStats);

// This is not `Copy`able because of `.name`.
typedef struct {
    const char *name;
    int id;
    PlayerStats stats;
} Player;

void test(Copy src, Copy dst, size_t size) {
    memcpy(src.self, dst.self, size);
}

int main(void) {
    const PlayerStats stats1 = {.health = 100, .victories = 5, .defeats = 2};
    PlayerStats stats2;
    test(DYN(PlayerStats, Copy, &stats1), DYN(PlayerStats, Copy, &stats2), sizeof(PlayerStats));

    assert(memcmp(&stats1, &stats2, sizeof(PlayerStats)) == 0);

    const Player p1 = {.name = "John", .id = 123, .stats = stats1};
    Player p2;
    // COMPILE-TIME ERROR:
    // test(DYN(Player, Copy, &p1), DYN(Player, Copy, &p2), sizeof(Player));

    (void)p1;
    (void)p2;

    return 0;
}
