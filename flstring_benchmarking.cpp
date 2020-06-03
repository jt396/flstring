/*
===============================================================================

    flstring
    ===
    File    :   flstrings_benchmarks.cpp
    Author  :   Jamie Taylor
    Desc    :   Benchmark the performance of flstring -vs- std::string 
                in a number of use-cases/scenarios. [WIP]

===============================================================================
*/
#include "flstring.hpp"
#include <iostream>
#include <chrono>
#include <numeric>
#include <iomanip>


// Guaranteed to -not- fit within any library implementation's SSO buffer.
using flstring = fl::string<32>;
// Guaranteed to fit within any library implementation's SSO buffer.
using flstring_sso = fl::string<8>;


#if 0
static std::size_t allocation_size = 0;
void* operator new( std::size_t n ) {
    //std::cout << "operator new( " << size << " )" << std::endl;
    allocation_size = n;
    return malloc( n );
}

void operator delete( void *ptr ) noexcept {
    free( ptr );
}

static std::size_t sso_size = 0;
void force_stdstring_dynamic_alloc() {
    // Currently, Clang has the largest SSO buffer (at 24 characters).
    // So we loop enough times to make sure that even theirs will overflow
    // and be forced into dynamic allocation >:)
    for( auto i=1; i<24; ++i ) {
        // Size of the string and a random character.
        std::string( i, '*' );
        if( allocation_size != 0 ) {
            sso_size = i;
            break;	
        }
    }
}


void benchMemoryFootprint() {
    // Compare sizes of flstring and std::string with a message guaranted to fit within SSO.
    {
        std::cout << "---\nMemory Footprint (8 Character String): SSO\n---" << std::endl;

        flstring_sso flstring( "8c fls." );
        std::string stdstring( "8c std." );

        std::cout << "\"" << flstring.data() << "\" takes: " << sizeof( flstring ) << " bytes." << std::endl;
        std::cout << "\"" << stdstring.data() << "\" takes: " << sizeof( stdstring ) << " bytes." << std::endl;
    }

    // Now with a message guaranteed -not- to fit within SSO.
    {
        std::cout << "---\nMemory Footprint (32 Character String): non-SSO\n---" << std::endl;

        flstring flstring( "A 32 character flstring........" );

        force_stdstring_dynamic_alloc();
        std::string stdstring( "A 32 character std::string.....", 31 /*, allocator*/ );

        std::cout << "\"" << flstring.data() << "\" takes: " << sizeof( flstring ) << " bytes." << std::endl;
        std::cout << "\"" << stdstring.data() << "\" takes: " << sizeof( stdstring ) + allocation_size << " bytes." << std::endl;
        std::cout << "*Your current std::string implementation uses a " << sso_size << " byte SSO buffer." << std::endl;
    }
}
#endif

void benchStringOperations() {
#if 1
    // Compare the time taken to create both an flstring and std::string - SSO.
    {
        std::cout << "---\nString Construction: via ctor( const char* ) SSO\n---" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        flstring_sso flstring( "8c fls." );
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating flstring: \"" << flstring.data() << 
            "\" using constructor \"flstring( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        start = std::chrono::high_resolution_clock::now();
        std::string stdstring( "8c std." );
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating std::string: \"" << stdstring.data() << 
            "\" using constructor \"string( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to create both an flstring and std::string - non-SSO.
    {
        std::cout << "---\nString Construction: via ctor( const char* ) non-SSO\n---" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        flstring flstring( "A 32 character flstring........" );
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating flstring: \"" << flstring.data() << 
            "\" using constructor \"flstring( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        start = std::chrono::high_resolution_clock::now();
        std::string stdstring( "A 32 character std::string.....", 32 /*, allocator*/ );
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating std::string: \"" << stdstring.data() << 
            "\" using constructor \"string( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to create both an flstring and std::string using the assignment operator - SSO.
    {
        std::cout << "---\nString Construction: via operator=( const char* ) SSO\n---" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        flstring_sso flstring;
        flstring = "8c fls.";
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating flstring: \"" << flstring.data() << 
            "\" using \"operator=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        start = std::chrono::high_resolution_clock::now();
        std::string stdstring;
        stdstring = "8c std.";
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating std::string: \"" << stdstring.data() << 
            "\" using \"operator=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to create both an flstring and std::string using the assignment operator - non-SSO.
    {
        std::cout << "---\nString Construction: via operator=( const char* ) non-SSO\n---" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        flstring flstring;
        flstring = "A 32 character flstring........";
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating flstring: \"" << flstring.data() << 
            "\" using \"operator=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        start = std::chrono::high_resolution_clock::now();
        std::string stdstring;
        stdstring = "A 32 character std::string.....";
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Creating std::string: \"" << stdstring.data() << 
            "\" using \"operator=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to iterate through an flstring and std::string using the [] operator - SSO.
    {
        std::cout << "---\nString Iteartion: via operator[]( size_type ) SSO\n---" << std::endl;

        flstring_sso flstring( "8c fls." );
        const auto string_size = 8;

        // Generate a 'random' value based on string contents and print it.
        // (Trying to make sure the compiler doesn't optimise things out).
        auto marker = 0;

        auto start = std::chrono::high_resolution_clock::now();
        for( auto i=0; i<string_size; ++i ) {
            marker |= flstring[i];
        }
        auto stop = std::chrono::high_resolution_clock::now();
        
        std::cout << "Itearting flstring: \"" << flstring.data() << 
            "\" using \"operator[]( size_type )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << "[" << marker << "]" << std::endl;

        // ---

        std::string stdstring( "8c std." );

        marker = 0;
        start = std::chrono::high_resolution_clock::now();
        for( auto i=0; i<string_size; ++i ) {
            marker |= stdstring[i];
        }
        stop = std::chrono::high_resolution_clock::now();
        
        std::cout << "Itearting flstring: \"" << stdstring.data() << 
            "\" using \"operator[]( size_type )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << "[" << marker << "]" << std::endl;
    }

    // Compare the time taken to iterate through an flstring and std::string using the [] operator - non-SSO.
    {
        std::cout << "---\nString Iteartion: via operator[]( size_type ) non-SSO\n---" << std::endl;

        flstring flstring( "A 32 character flstring........" );
        const auto string_size = 32;

        auto marker = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for( auto i=0; i<string_size; ++i ) {
            marker |= flstring[i];
        }
        auto stop = std::chrono::high_resolution_clock::now();
        
        std::cout << "Itearting flstring: \"" << flstring.data() << 
            "\" using \"operator[]( size_type )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << "[" << marker << "]" << std::endl;

        // ---

        std::string stdstring( "A 32 character std::string....." );

        marker = 0;
        start = std::chrono::high_resolution_clock::now();
        for( auto i=0; i<string_size; ++i ) {
            marker |= stdstring[i];
        }
        stop = std::chrono::high_resolution_clock::now();		
        
        std::cout << "Itearting std::string: \"" << stdstring.data() << 
            "\" using \"operator[]( size_type )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << "[" << marker << "]" << std::endl;
    }
#endif
    // Compare the time taken to ...compare an flstring with another flstring and std::string with another std::string - SSO.
    {
        std::cout << "---\nString Comparison: Where (str0 == str1) SSO\n---" << std::endl;

        flstring_sso flstring0( "8c fls." );
        flstring_sso flstring1( "8c fls." );		

        auto start = std::chrono::high_resolution_clock::now();
        flstring0.compare( flstring1 );
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing flstring: \"" << flstring0.data() << 
            "\" with flstring: \"" << flstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring0( "8c std." );
        std::string stdstring1( "8c std." );		

        start = std::chrono::high_resolution_clock::now();
        stdstring0.compare( stdstring1 );
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing stdstring: \"" << stdstring0.data() << 
            "\" with stdstring: \"" << stdstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to ...compare an flstring with another flstring and std::string with another std::string - non-SSO.
    {
        std::cout << "---\nString Comparison: Where (str0 == str1) non-SSO\n---" << std::endl;

        flstring flstring0( "A 32 character flstring........" );
        flstring flstring1( "A 32 character flstring........" );		

        auto start = std::chrono::high_resolution_clock::now();
        flstring0.compare( flstring1 );
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing flstring: \"" << flstring0.data() << 
            "\" with flstring: \"" << flstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring0( "A 32 character std::string....." );
        std::string stdstring1( "A 32 character std::string....." );		

        start = std::chrono::high_resolution_clock::now();
        stdstring0.compare( stdstring1 );
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing stdstring: \"" << stdstring0.data() << 
            "\" with stdstring: \"" << stdstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to ...compare an flstring with another flstring and std::string with another std::string - SSO.
    {
        std::cout << "---\nString Comparison: Where (str0 != str1) SSO\n---" << std::endl;

        flstring_sso flstring0( "8c fls." );
        flstring_sso flstring1( "8c std." );		

        auto start = std::chrono::high_resolution_clock::now();
        flstring0.compare( flstring1 );
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing flstring: \"" << flstring0.data() << 
            "\" with flstring: \"" << flstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring0( "8c std." );
        std::string stdstring1( "8c fls." );		

        start = std::chrono::high_resolution_clock::now();
        stdstring0.compare( stdstring1 );
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing stdstring: \"" << stdstring0.data() << 
            "\" with stdstring: \"" << stdstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to ...compare an flstring with another flstring and std::string with another std::string - non-SSO.
    {
        std::cout << "---\nString Comparison: Where (str0 != str1) non-SSO\n---" << std::endl;

        flstring flstring0( "A 32 character flstring........" );
        flstring flstring1( "A 32 character std::string....." );		

        auto start = std::chrono::high_resolution_clock::now();
        flstring0.compare( flstring1 );
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing flstring: \"" << flstring0.data() << 
            "\" with flstring: \"" << flstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring0( "A 32 character std::string....." );
        std::string stdstring1( "A 32 character flstring........" );		

        start = std::chrono::high_resolution_clock::now();
        stdstring0.compare( stdstring1 );
        stop = std::chrono::high_resolution_clock::now();

        std::cout << "Comparing stdstring: \"" << stdstring0.data() << 
            "\" with stdstring: \"" << stdstring1.data() << "\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }
#if 1
    // Compare the time taken to concatenate an flstring and using operator+=( const char * )  - SSO.
    {
        std::cout << "---\nString Concatenation: via operator+=( const char* ) - SSO\n---" << std::endl;

        flstring_sso flstring( "8c" );
        std::cout << "Concatenating a flstring: \"" << flstring.data() << "\"";

        auto start = std::chrono::high_resolution_clock::now();
        flstring += " fls.";
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << " with \"fls." << 
            "\" using \"operator+=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring( "8c" );
        std::cout << "Concatenating an std::string: \"" << stdstring.data() << "\"";

        start = std::chrono::high_resolution_clock::now();
        stdstring += " std.";
        stop = std::chrono::high_resolution_clock::now();

        std::cout << " with \"std." << 
            "\" using \"operator+=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to concatenate an flstring and using operator+=( const char * )  - non-SSO.
    {
        std::cout << "---\nString Concatenation: via operator+=( const char* ) - non-SSO\n---" << std::endl;

        fl::string<64> flstring( "A 32 character flstring........" );
        std::cout << "Concatenating a flstring: \"" << flstring.data() << "\"";

        auto start = std::chrono::high_resolution_clock::now();
        flstring += "A 32 character flstring........";
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << " with \"A 32 character flstring........" << 
            "\" using \"operator+=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring( "A 32 character std::string....." );
        std::cout << "Concatenating a flstring: \"" << stdstring.data() << "\"";

        start = std::chrono::high_resolution_clock::now();
        stdstring += "A 32 character std::string.....";
        stop = std::chrono::high_resolution_clock::now();

        std::cout << " with \"A 32 character std::string....." << 
            "\" using \"operator+=( const char* )\" took: " <<
            ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to clear an flstring and std::string (string reports a length() of 0) - SSO.
    {	
        std::cout << "---\nString Clear/Reset/Make-Empty: String must report length of 0 - SSO\n---" << std::endl;

        flstring_sso flstring( "8c fls." );

        std::cout << "Clearing flstring: \"" << flstring.data();

        auto start = std::chrono::high_resolution_clock::now();
        flstring.clear();
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "\" took: " << ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // TODO: ShallowClear()?

        // ---

        std::string stdstring( "8c std." );

        std::cout << "Clearing std::string: \"" << stdstring.data();

        start = std::chrono::high_resolution_clock::now();
        stdstring.clear();
        stop = std::chrono::high_resolution_clock::now();
        
        std::cout << "\" took: " << ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }

    // Compare the time taken to clear an flstring and std::string (string reports a length() of 0) - non-SSO.
    {	
        std::cout << "---\nString Clear/Reset/Make-Empty: String must report length of 0 - non-SSO\n---" << std::endl;

        flstring flstring( "A 32 character flstring........" );

        std::cout << "Clearing flstring: \"" << flstring.data();

        auto start = std::chrono::high_resolution_clock::now();
        flstring.clear();
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << "\" took: " << ( stop - start ).count() / 1000.0 << " ms." << std::endl;

        // ---

        std::string stdstring( "A 32 character std::string....." );

        std::cout << "Clearing std::string: \"" << stdstring.data();

        start = std::chrono::high_resolution_clock::now();
        stdstring.clear();
        stop = std::chrono::high_resolution_clock::now();
        
        std::cout << "\" took: " << ( stop - start ).count() / 1000.0 << " ms." << std::endl;
    }
    #endif
}

// http://www.maltron.com/word-lists---qwerty-layout.html
static const unsigned int key_count = 128;
const char* three_character_container_strings[key_count] = {
    "aas", "aba", "abs", "ace", "act", "add", "ads", "adz", "aff", "aft",
    "aga", "age", "arb", "arc", "are", "arf", "ars", "art", "ass", "ate",
    "att", "ava", "ave", "awa", "awe", "axe", "baa", "bad", "bag", "bar",
    "bas", "bat", "bed", "bee", "beg", "bet", "bra", "brr", "cab", "cad",
    "car", "cat", "caw", "cee", "dab", "dad", "dag", "daw", "deb", "dee",
    "dev", "dew", "dex", "ear", "eat", "ebb", "eff", "efs", "eft", "egg",
    "era", "ere", "erg", "err", "ers", "ess", "eta", "eve", "ewe", "fad",
    "fag", "far", "fas", "fat", "fax", "fed", "fee", "fer", "fet", "few",
    "fez", "gab", "gad", "gae", "gag", "gar", "gas", "gat", "ged", "gee",
    "get", "qat", "rad", "rag", "ras", "rat", "raw", "rax", "reb", "rec",
    "red", "ree", "ref", "reg", "res", "ret", "rev", "rex", "rts", "sab",
    "sac", "sad", "sae", "sag", "sat", "saw", "sax", "sea", "sec", "see",
    "seg", "ser", "set", "sew", "sex", "tab", "tad", "tae"
};
const char* seven_character_container_strings[key_count] = {
    "abasers", "abaters", "abetted", "abetter", "abfarad", "abraded", "abrader", "abrades", "abreact", "abreast",
    "abscess", "abwatts", "acceded", "acceder", "accedes", "accrete", "acerate", "acerber", "acetate", "acreage",
    "actress", "addaxes", "address", "addrest", "advects", "adverbs", "adverse", "adverts", "aerated", "aerates",
    "afeared", "affects", "afreets", "aggrade", "aggress", "agrafes", "agraffe", "arcaded", "arcades", "arrased",
    "arrears", "arrests", "asceses", "asserts", "asswage", "attests", "attract", "avatars", "average", "averred",
    "averted", "avgases", "awarded", "awardee", "awarder", "axseeds", "baccara", "baccate", "baddest", "badgers",
    "bagasse", "baggage", "baggers", "barbate", "barbers", "barbets", "bareges", "bargees", "barrage", "barrets",
    "barters", "barware", "bassets", "bassett", "bastard", "basters", "batters", "bawbees", "bazaars", "bearcat",
    "bearded", "bearers", "beaters", "beavers", "bedders", "bedewed", "bedfast", "bedward", "bedwarf", "beebees",
    "beeswax", "beezers", "befrets", "begazed", "begazes", "beggars", "berated", "berates", "bereave", "beretta",
    "bergere", "bestead", "bestrew", "betaxed", "betters", "bewared", "bewares", "bracers", "bracted", "bradded",
    "bragged", "bragger", "brassed", "brasses", "bravers", "bravest", "brawest", "brazers", "breaded", "breasts",
    "breeder", "breezed", "breezes", "brevets", "brewage", "brewers", "cabaret", "cabbage"
};


#include <cstdlib>
#include <map>
void benchOrderedMapOperations() {
    const unsigned int loop_count = 1024;
    const unsigned int lookup_count = 256;
    std::array<unsigned int, lookup_count> lookup_key_indices;

    srand( time( NULL ) );

    // Generate a random sequence of keys to look-up; this is intended to try and keep things fair.
    for( auto& key_index : lookup_key_indices ) {
        key_index = rand() % (key_count-1) + 1;
    }

    // Compare time taken to look-up entries in a hash-map using flstrings for keys -vs- std::strings for keys - (3 characters + null = 4 characters total)
    {
        std::cout << "---\nUsing strings as keys in a std::map; Creation and Lookup (4 character keys inc. null-terminator)\n---" << std::endl;

        using flstring_4ch = fl::string<4>;
        std::map<flstring_4ch, unsigned int> flstring_map;
        
        std::array<double, loop_count*2> flstring_map_creation_times;
        std::array<double, loop_count*2> flstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                flstring_map.emplace( three_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                //flstring_map[three_character_container_strings[lookup]]
                flstring_map.find( three_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // ---

        std::map<std::string, unsigned int> stdstring_map;
        
        std::array<double, loop_count*2> stdstring_map_creation_times;
        std::array<double, loop_count*2> stdstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                stdstring_map.emplace( three_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                stdstring_map.find( three_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // Now flip the order and run again		
        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                stdstring_map.emplace( three_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                stdstring_map.find( three_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                flstring_map.emplace( three_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                //flstring_map[three_character_container_strings[lookup]]
                flstring_map.find( three_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        // Now print the results
        double flstring_map_creation_time_avg = std::accumulate( flstring_map_creation_times.begin(), flstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double flstring_map_lookup_time_avg = std::accumulate( flstring_map_lookup_times.begin(), flstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "flstring ordered_map creation time: " << flstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "flstring ordered_map look-up time: " << flstring_map_lookup_time_avg << " ms.\n---" << std::endl;

        double stdstring_map_creation_time_avg = std::accumulate( stdstring_map_creation_times.begin(), stdstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double stdstring_map_lookup_time_avg = std::accumulate( stdstring_map_lookup_times.begin(), stdstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "stdstring ordered_map creation time: " << stdstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "stdstring ordered_map look-up time: " << stdstring_map_lookup_time_avg << " ms." << std::endl;
    }

    // Compare time taken to look-up entries in a hash-map using flstrings for keys -vs- std::strings for keys - (7 characters + null = 8 characters total)
    {
        std::cout << "---\nUsing strings as keys in a std::map; Creation and Lookup (8 character keys inc. null-terminator)\n---" << std::endl;

        using flstring_4ch = fl::string<8>;
        std::map<flstring_4ch, unsigned int> flstring_map;
        
        std::array<double, loop_count*2> flstring_map_creation_times;
        std::array<double, loop_count*2> flstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                flstring_map.emplace( seven_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                flstring_map.find( seven_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // ---

        std::map<std::string, unsigned int> stdstring_map;
        
        std::array<double, loop_count*2> stdstring_map_creation_times;
        std::array<double, loop_count*2> stdstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                stdstring_map.emplace( seven_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                stdstring_map.find( seven_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // Now flip the order and run again		
        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                stdstring_map.emplace( seven_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                stdstring_map.find( seven_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                flstring_map.emplace( seven_character_container_strings[j], 0 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                flstring_map.find( seven_character_container_strings[lookup] )->second += 1;
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        // Now print the results
        double flstring_map_creation_time_avg = std::accumulate( flstring_map_creation_times.begin(), flstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double flstring_map_lookup_time_avg = std::accumulate( flstring_map_lookup_times.begin(), flstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "flstring ordered_map creation time: " << flstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "flstring ordered_map look-up time: " << flstring_map_lookup_time_avg << " ms.\n---" << std::endl;

        double stdstring_map_creation_time_avg = std::accumulate( stdstring_map_creation_times.begin(), stdstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double stdstring_map_lookup_time_avg = std::accumulate( stdstring_map_lookup_times.begin(), stdstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "stdstring ordered_map creation time: " << stdstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "stdstring ordered_map look-up time: " << stdstring_map_lookup_time_avg << " ms." << std::endl;
    }
}

#include <unordered_map>
#include "crc32.hpp"
// hash function: calculateCRC32() from crc32.hpp

/*
unsigned long hashKey( fl::string<4> str ) {
    return 0;
}

// hash key equal function
bool keysEqual( fl::string<4> lhs, fl::string<4> rhs ) {
    return lhs.compare( rhs ) == 0;
}
*/
template<size_t strlen>
class KeyHash {
public:
    unsigned long operator()( const fl::string<strlen>& key ) const {
        return calculateCRC32<strlen>( key );
    };
};
template<size_t strlen>
class KeyCompare {
public:
    bool operator()( const fl::string<strlen>& lhs, const fl::string<strlen>& rhs ) const {
        return lhs.compare( rhs ) == 0;
    };
};

template<size_t strlen>
class KeyHash_std {
public:
    unsigned long operator()( const std::string& key ) const {
        return calculateCRC32<4>( key );
    };
};
class KeyCompare_std {
public:
    bool operator()( const std::string& lhs, const std::string& rhs ) const {
        return lhs.compare( rhs ) == 0;
    };
};

void benchUnorderedMapOperations() {
    const unsigned int loop_count = 32;
    const unsigned int lookup_count = 256;
    std::array<unsigned int, lookup_count> lookup_key_indices;

    // Use pre-constructed strings to avoid constructing temporary objects when
    // populating and searching the maps. Would be worthwhile doing this for
    // ordered_map as well, I was unable to due to time constraints. :(
//#define USE_PREMADE_OBJECTS 0
#if 0//defined(USE_PREMADE_OBJECTS)
    std::array<fl::string<4>, key_count> flstrings4ch;
    std::array<fl::string<8>, key_count> flstrings8ch;

    std::array<std::string, key_count> stdstrings4ch;
    std::array<std::string, key_count> stdstrings8ch;

    for(unsigned int i=0; i<key_count; ++i) {
        flstrings4ch[i] = three_character_container_strings[i];
        stdstrings4ch[i] = three_character_container_strings[i];

        flstrings8ch[i] = seven_character_container_strings[i];
        stdstrings8ch[i] = seven_character_container_strings[i];
    }	
#endif

    srand( time( NULL ) );

    // Generate a random sequence of keys to look-up; this is intended to try and keep things fair.
    for( auto& key_index : lookup_key_indices ) {
        key_index = rand() % (key_count-1) + 1;
    }

    // Compare time taken to look-up entries in a hash-map using flstrings for keys -vs- std::strings for keys - (3 characters + null = 4 characters total)
    {
        std::cout << "---\nUsing strings as keys in a std::unordered_map; Creation and Lookup (4 character keys inc. null-terminator)\n---" << std::endl;

        using flstring_4ch = fl::string<4>;
        std::unordered_map<flstring_4ch, unsigned int, KeyHash<4>, KeyCompare<4>> flstring_map;

        std::array<double, loop_count*2> flstring_map_creation_times;
        std::array<double, loop_count*2> flstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.emplace( flstrings4ch[j], 0 );
                #else
                    flstring_map.emplace( three_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.find( flstrings4ch[lookup] )->second += 1;
                #else
                    flstring_map.find( three_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // ---

        std::unordered_map<std::string, unsigned int, KeyHash_std<4>, KeyCompare_std> stdstring_map;
        
        std::array<double, loop_count*2> stdstring_map_creation_times;
        std::array<double, loop_count*2> stdstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.emplace( stdstrings4ch[j], 0 );
                #else
                    stdstring_map.emplace( three_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.find( stdstrings4ch[lookup] )->second += 1;
                #else
                    stdstring_map.find( three_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // Now run the tests again, but with the order reversed (fl::string->std::string becomes std::string->fl::string)
        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.emplace( stdstrings4ch[j], 0 );
                #else
                    stdstring_map.emplace( three_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.find( stdstrings4ch[lookup] )->second += 1;
                #else
                    stdstring_map.find( three_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.emplace( flstrings4ch[j], 0 );
                #else
                    flstring_map.emplace( three_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.find( flstrings4ch[lookup] )->second += 1;
                #else
                    flstring_map.find( three_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        // Now print the results
        double flstring_map_creation_time_avg = std::accumulate( flstring_map_creation_times.begin(), flstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double flstring_map_lookup_time_avg = std::accumulate( flstring_map_lookup_times.begin(), flstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "flstring unordered_map creation time: " << flstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "flstring unordered_map look-up time: " << flstring_map_lookup_time_avg << " ms.\n---" << std::endl;

        double stdstring_map_creation_time_avg = std::accumulate( stdstring_map_creation_times.begin(), stdstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double stdstring_map_lookup_time_avg = std::accumulate( stdstring_map_lookup_times.begin(), stdstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "stdstring unordered_map creation time: " << stdstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "stdstring unordered_map look-up time: " << stdstring_map_lookup_time_avg << " ms." << std::endl;
    }

    // ...

    // Compare time taken to look-up entries in a hash-map using flstrings for keys -vs- std::strings for keys - (7 characters + null = 8 characters total)
    {
        std::cout << "---\nUsing strings as keys in a std::unordered_map; Creation and Lookup (8 character keys inc. null-terminator)\n---" << std::endl;

        using flstring_8ch = fl::string<8>;
        std::unordered_map<flstring_8ch, unsigned int, KeyHash<8>, KeyCompare<8>> flstring_map;

        std::array<double, loop_count*2> flstring_map_creation_times;
        std::array<double, loop_count*2> flstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.emplace( flstrings8ch[j], 0 );
                #else
                    flstring_map.emplace( seven_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.find( flstrings8ch[lookup] )->second += 1;
                #else
                    flstring_map.find( seven_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // ---

        std::unordered_map<std::string, unsigned int, KeyHash_std<8>, KeyCompare_std> stdstring_map;
        
        std::array<double, loop_count*2> stdstring_map_creation_times;
        std::array<double, loop_count*2> stdstring_map_lookup_times;

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.emplace( stdstrings8ch[j], 0 );
                #else
                    stdstring_map.emplace( seven_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.find( stdstrings8ch[lookup] )->second += 1;
                #else
                    stdstring_map.find( seven_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[i] = (stop - start).count() / 1000.0;
        }

        // Now run the tests again, but with the order reversed (fl::string->std::string becomes std::string->fl::string)
        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.emplace( stdstrings8ch[j], 0 );
                #else
                    stdstring_map.emplace( seven_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    stdstring_map.find( stdstrings8ch[lookup] )->second += 1;
                #else
                    stdstring_map.find( seven_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            stdstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        for( unsigned int i=0; i<loop_count; ++i ) {
            // Create the map
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.emplace( flstrings8ch[j], 0 );
                #else
                    flstring_map.emplace( seven_character_container_strings[j], 0 );
                #endif
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_map_creation_times[loop_count+i] = (stop - start).count() / 1000.0;

            // Now perform 'random' lookups
            start = std::chrono::high_resolution_clock::now();
            for( auto& lookup : lookup_key_indices ) {
                #if 0//defined(USE_PREMADE_OBJECTS)
                    flstring_map.find( flstrings8ch[lookup] )->second += 1;
                #else
                    flstring_map.find( seven_character_container_strings[lookup] )->second += 1;
                #endif
            }
            stop = std::chrono::high_resolution_clock::now();
            flstring_map_lookup_times[loop_count+i] = (stop - start).count() / 1000.0;
        }

        // Now print the results
        double flstring_map_creation_time_avg = std::accumulate( flstring_map_creation_times.begin(), flstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double flstring_map_lookup_time_avg = std::accumulate( flstring_map_lookup_times.begin(), flstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "flstring unordered_map creation time: " << flstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "flstring unordered_map look-up time: " << flstring_map_lookup_time_avg << " ms.\n---" << std::endl;

        double stdstring_map_creation_time_avg = std::accumulate( stdstring_map_creation_times.begin(), stdstring_map_creation_times.end(), 0.0 ) / (loop_count*2);
        double stdstring_map_lookup_time_avg = std::accumulate( stdstring_map_lookup_times.begin(), stdstring_map_lookup_times.end(), 0.0 ) / (loop_count*2);
        std::cout << "stdstring unordered_map creation time: " << stdstring_map_creation_time_avg << " ms." << std::endl;
        std::cout << "stdstring unordered_map look-up time: " << stdstring_map_lookup_time_avg << " ms." << std::endl;
    }
}

#if 0
#include "crc32.hpp"
void benchCRC32Operations() {
    const unsigned int loop_count = 128;
    std::array<double, loop_count> flstring_crc32_generation_times;
    std::array<double, loop_count> stdstring_crc32_generation_times;

    unsigned int flstring_crc32_fingerprint = 0;
    unsigned int stdstring_crc32_fingerprint = 0;

    std::array<fl::string<4>, key_count> flstring_array;
    for( unsigned int i=0; i<key_count; ++i ) {
        flstring_array[i] = three_character_container_strings[i];
    }

    for( unsigned int i=0; i<loop_count; ++i ) {
        auto start = std::chrono::high_resolution_clock::now();
        for( unsigned int j=0; j<1; ++j ) {
            flstring_crc32_fingerprint |= calculateCRC32( 0, reinterpret_cast<const void*>( flstring_array[j].data() ), 3 );
        }
        auto stop = std::chrono::high_resolution_clock::now();
        flstring_crc32_generation_times[i] = (stop-start).count() / 1000.0;
    }
    double flstring_crc32_generation_time_avg = std::accumulate( flstring_crc32_generation_times.begin(), flstring_crc32_generation_times.end(), 0.0 ) / loop_count;
    std::cout << std::setprecision( 3 ) << "flstring CRC32 generation time: " << flstring_crc32_generation_time_avg << " ms." << flstring_crc32_fingerprint << std::endl;

    // ---

    std::array<std::string, key_count> stdstring_array;
    for( unsigned int i=0; i<key_count; ++i ) {
        stdstring_array[i] = three_character_container_strings[i];
    }

    for( unsigned int i=0; i<loop_count; ++i ) {
        auto start = std::chrono::high_resolution_clock::now();
        for( unsigned int j=0; j<1; ++j ) {
            stdstring_crc32_fingerprint |= calculateCRC32( 0, reinterpret_cast<const void*>( stdstring_array[j].data() ), 3 );
        }
        auto stop = std::chrono::high_resolution_clock::now();
        stdstring_crc32_generation_times[i] = (stop-start).count() / 1000.0;
    }
    double stdstring_crc32_generation_time_avg = std::accumulate( stdstring_crc32_generation_times.begin(), stdstring_crc32_generation_times.end(), 0.0 ) / loop_count;
    std::cout << std::setprecision( 3 ) << "stdstring CRC32 generation time: " << stdstring_crc32_generation_time_avg << " ms." << stdstring_crc32_fingerprint << std::endl;
#if 1
    {
        unsigned int flstring_crc32_fingerprint = 0;
        unsigned int stdstring_crc32_fingerprint = 0;

        std::array<fl::string<8>, key_count> flstring_array;
        for( unsigned int i=0; i<key_count; ++i ) {
            flstring_array[i] = seven_character_container_strings[i];
        }

        for( unsigned int i=0; i<loop_count; ++i ) {
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                flstring_crc32_fingerprint |= calculateCRC32( 0, reinterpret_cast<const void*>( flstring_array[j].data() ), 7 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            flstring_crc32_generation_times[i] = (stop-start).count() / 1000.0;
        }
        double flstring_crc32_generation_time_avg = std::accumulate( flstring_crc32_generation_times.begin(), flstring_crc32_generation_times.end(), 0.0 ) / loop_count;
        std::cout << std::setprecision( 3 ) << "flstring CRC32 generation time: " << flstring_crc32_generation_time_avg << " ms." << flstring_crc32_fingerprint << std::endl;

        // ---

        std::array<std::string, key_count> stdstring_array;
        for( unsigned int i=0; i<key_count; ++i ) {
            stdstring_array[i] = seven_character_container_strings[i];
        }

        for( unsigned int i=0; i<loop_count; ++i ) {
            auto start = std::chrono::high_resolution_clock::now();
            for( unsigned int j=0; j<key_count; ++j ) {
                stdstring_crc32_fingerprint |= calculateCRC32( 0, reinterpret_cast<const void*>( stdstring_array[j].data() ), 7 );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdstring_crc32_generation_times[i] = (stop-start).count() / 1000.0;
        }
        double stdstring_crc32_generation_time_avg = std::accumulate( stdstring_crc32_generation_times.begin(), stdstring_crc32_generation_times.end(), 0.0 ) / loop_count;
        std::cout << std::setprecision( 3 ) << "stdstring CRC32 generation time: " << stdstring_crc32_generation_time_avg << " ms." << stdstring_crc32_fingerprint << std::endl;
    }
#endif
    return;
}
#endif

int main( int argc, char* argv[] ) {
    benchMemoryFootprint();
    benchStringOperations();
    benchOrderedMapOperations();
    benchUnorderedMapOperations();
    benchCRC32Operations();
    return 0;
}
