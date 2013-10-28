#include "vars.h"

Singleton::Singleton() {
};

Singleton& Vars() {
    static Singleton vars;
    return vars;
};
