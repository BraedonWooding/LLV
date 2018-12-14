#ifndef LLV_ENV_VAR_H
#define LLV_ENV_VAR_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../include/helper.h"

#undef new_env_var
#define new_env_var(name, env_name, type, default_value, process_fn) \
    type name(void);

#include "env_var.incl"
#undef new_env_var

#endif
