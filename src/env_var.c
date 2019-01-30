#include "env_var.h"

#undef new_env_var
#define new_env_var(name, env_name, type, default_value, process_fn) \
    type name(void) { \
        bool defined = false; \
        type previous = default_value; \
        if (!defined) { \
            defined = true; \
            char *str = getenv(#env_name); \
            if (str != NULL) previous = process_fn(str); \
        } \
        return previous; \
    }

#include "env_var.incl"
