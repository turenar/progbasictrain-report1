#pragma once

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef __attribute_format__
#define __attribute_format__(type, string_index, first_to_check) __attribute((format(type, string_index, first_to_check)))
#endif
#define UNUSED_VAR(x) ((void)x)
