#include "vars.h"

Singleton::Singleton() {
};

Singleton& Vars() {
    // The compiler adds branches for thread-safe initialization (other threads, exceptions).
    static Singleton vars; // GCOVR_EXCL_BR_LINE
    return vars;
};
