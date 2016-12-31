#ifndef __SPEC_HELPER_H__
#define __SPEC_HELPER_H__

#include "Matchers.h"
#include "Elara.h"
#include "ElaraList.h"

// Measures an elara list, tests that it has that size
ElaraMatcherReturn have_list_size(int expected_list_size);

#endif /* __SPEC_HELPER_H__ */
