#include <string>
#include <vector>

#include "cata_catch.h"
#include "input_context.h"
#include "translation.h"

TEST_CASE( "keybinding_menu_filter_strings_by_phrase", "[input][filter]" )
{
    input_context ctxt( "TEST" );
    ctxt.register_action( "open_inventory", translation::no_translation( "Open inventory" ) );
    ctxt.register_action( "inventory_open", translation::no_translation( "Inventory open" ) );
    ctxt.register_action( "open_map", translation::no_translation( "Open map" ) );

    const std::vector<std::string> actions = { "open_inventory", "open_map", "inventory_open" };

    SECTION( "substring_filter_matches_action_name_or_id" ) {
        CHECK( ctxt.filter_strings_by_phrase( actions, "open" ) ==
               std::vector<std::string>( { "open_inventory", "open_map", "inventory_open" } ) );
    }

    SECTION( "multi_term_and_filter" ) {
        CHECK( ctxt.filter_strings_by_phrase( actions, "open inv" ) ==
               std::vector<std::string>( { "open_inventory", "inventory_open" } ) );
    }

    SECTION( "exclude_terms" ) {
        CHECK( ctxt.filter_strings_by_phrase( actions, "open -map" ) ==
               std::vector<std::string>( { "open_inventory", "inventory_open" } ) );
    }

    SECTION( "id_qualifier" ) {
        CHECK( ctxt.filter_strings_by_phrase( actions, "id:open_" ) ==
               std::vector<std::string>( { "open_inventory", "open_map" } ) );
    }

    SECTION( "fuzzy_prefix_ranks_start_matches_higher" ) {
        const std::vector<std::string> filtered = ctxt.filter_strings_by_phrase( actions, "~inventory" );
        REQUIRE( filtered.size() == 2 );
        CHECK( filtered[0] == "inventory_open" );
        CHECK( filtered[1] == "open_inventory" );
    }
}
