#pragma once

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config.vc.h"
#endif

#ifndef __attribute_format__
#ifdef __GNUC__
#define __attribute_format__(type, string_index, first_to_check) __attribute((format(type, string_index, first_to_check)))
#else
#define __attribute_format__(a,b,c) /**/
#endif
#endif
#define UNUSED_VAR(x) ((void)x)
