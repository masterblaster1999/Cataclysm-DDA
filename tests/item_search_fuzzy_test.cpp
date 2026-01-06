#include "cata_catch.h"

#include "item.h"
#include "item_search.h"
#include "type_id.h"

static const itype_id itype_test_apple( "test_apple" );

TEST_CASE( "item_filter_from_string_supports_fuzzy_prefix", "[item][item_search][filter][fuzzy]" )
{
    const item apple( itype_test_apple );

    // Sanity: the plain substring matcher should not consider this a match.
    CHECK_FALSE( item_filter_from_string( "apl" )( apple ) );

    // Fuzzy mode (prefix with '~') is a subsequence match.
    CHECK( item_filter_from_string( "~apl" )( apple ) );
    CHECK_FALSE( item_filter_from_string( "~zz" )( apple ) );
}

TEST_CASE( "item_filter_fuzzy_works_for_category_prefix", "[item][item_search][filter][fuzzy]" )
{
    const item apple( itype_test_apple );

    // Most test comestibles live in the Food category; this intentionally uses a fuzzy abbreviation.
    CHECK( item_filter_from_string( "c:~fd" )( apple ) );
    CHECK_FALSE( item_filter_from_string( "c:~wep" )( apple ) );
}
