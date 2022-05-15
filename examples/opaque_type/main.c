// This example demonstrates how to implement interfaces for opaque types.
// Note: you can export interface implementations outside of a TU for non-opaque types in the same
// way.

#include "croak.h"
#include "frog.h"

/*
 * Output:
 * Paul: croak!
 * Steve: croak!
 */
int main(void) {
    Frog *paul = Frog_new("Paul");
    Frog *steve = Frog_new("Steve");

    VCALL(DYN(Frog, Croak, paul), croak);
    VCALL(DYN(Frog, Croak, steve), croak);

    Frog_free(paul);
    Frog_free(steve);

    return 0;
}
