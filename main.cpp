
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of FlatJSON(https://github.com/niXman/flatjson) project.
//
// Copyright (c) 2019-2022 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include "flatjson.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cassert>

#define FJ_RUN_TEST(test) \
    test(); std::cout << "test \"" << #test "\" passed!" << std::endl;

/*************************************************************************************************/

std::string read_file(const char *fname) {
    std::ifstream file(fname);
    assert(file.good());

    std::string str(
        (std::istreambuf_iterator<char>(file))
        ,std::istreambuf_iterator<char>())
    ;
    return str;
}

/*************************************************************************************************/

struct test_result {
    int ec;
    const char *name;
};

int parse_file(const char *path, const char *fname) {
    std::string p{path};
    p += fname;

    const std::string buf = read_file(p.c_str());
    auto res = flatjson::fj_num_tokens(buf.c_str(), buf.c_str()+buf.length());

    return res.ec;
}

test_result test_conformance() {
    static const char *part0_path = "jsonchecker/";
    static const char *part0[] = {
         "0:pass01.json","0:pass02.json","0:pass03.json","1:fail02.json"
        ,"1:fail03.json","1:fail04.json","1:fail05.json","1:fail06.json"
        ,"1:fail07.json","1:fail08.json","1:fail09.json","1:fail10.json"
        ,"1:fail11.json","1:fail12.json","1:fail13.json","1:fail14.json"
        ,"1:fail15.json","1:fail16.json","1:fail17.json","1:fail19.json"
        ,"1:fail20.json","1:fail21.json","1:fail22.json","1:fail23.json"
        ,"1:fail24.json","1:fail25.json","1:fail26.json","1:fail27.json"
        ,"1:fail28.json","1:fail29.json","1:fail30.json","1:fail31.json"
        ,"1:fail32.json","1:fail33.json"
        ,nullptr
    };
    for ( const char **it = part0; *it; ++it ) {
        int exp = *it[0] - '0';
        int ec = parse_file(part0_path, *it+2);
        if ( exp && 0 == ec ) {
            return {ec, *it+2};
        }
    }

    static const char *part1_path = "roundtrip/";
    static const char *part1[] = {
         "0:roundtrip01.json","0:roundtrip04.json","0:roundtrip07.json","0:roundtrip10.json"
        ,"0:roundtrip13.json","0:roundtrip16.json","0:roundtrip19.json","0:roundtrip22.json"
        ,"0:roundtrip25.json","0:roundtrip02.json","0:roundtrip05.json","0:roundtrip08.json"
        ,"0:roundtrip11.json","0:roundtrip14.json","0:roundtrip17.json","0:roundtrip20.json"
        ,"0:roundtrip23.json","0:roundtrip26.json","0:roundtrip03.json","0:roundtrip06.json"
        ,"0:roundtrip09.json","0:roundtrip12.json","0:roundtrip15.json","0:roundtrip18.json"
        ,"0:roundtrip21.json","0:roundtrip24.json","0:roundtrip27.json"
        ,nullptr
    };
    for ( const char **it = part1; *it; ++it ) {
        int exp = *it[0] - '0';
        int ec = parse_file(part1_path, *it+2);
        if ( exp && 0 == ec ) {
            return {ec, *it+2};
        }
    }

    return {};
}

/*************************************************************************************************/

static void unit_00() {
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_INVALID) == false);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_STRING) == true);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_NUMBER) == true);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_BOOL) == true);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_NULL) == true);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_OBJECT) == false);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_OBJECT_END) == false);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_ARRAY) == false);
    assert(flatjson::details::fj_is_simple_type(flatjson::FJ_TYPE_ARRAY_END) == false);
}

/*************************************************************************************************/

static void unit_01() {
    flatjson::fjson json0;
    assert(json0.is_valid() == false);

    flatjson::fjson json1{""};
    assert(json1.is_valid() == false);

    flatjson::fjson json2{nullptr, nullptr};
    assert(json2.is_valid() == false);
}

static void unit_02() {
    static const char str[] = R"({"a":true, "b":false, "c":null, "d":0, "e":"e"})";
    auto json = flatjson::parse(std::begin(str), std::end(str));

    assert(json.is_valid());
    assert(json.size() == 5);
    assert(json.is_object());
    assert(!json.is_array());
    assert(!json.is_number());
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());
}

static void unit_03() {
    static const char str[] = R"({"a":true, "b":false, "c":null, "d":0, "e":"e"})";
    auto json = flatjson::parse(std::begin(str), std::end(str));
    json.load(str);

    assert(json.is_valid());
    assert(json.size() == 5);
    assert(json.is_object());
    assert(!json.is_array());
    assert(!json.is_number());
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());
}

static void unit_04() {
    static const char str[] = R"({})";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 0);
    assert(json.is_empty());
    assert(json.is_object());
    assert(!json.is_array());
    assert(!json.is_null());
    assert(!json.is_string());
    assert(!json.is_bool());
    assert(!json.is_number());
}

static void unit_05() {
    static const char str[] = R"([])";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 0);
    assert(json.is_empty());
    assert(json.is_array());
    assert(!json.is_object());
    assert(!json.is_null());
    assert(!json.is_string());
    assert(!json.is_bool());
    assert(!json.is_number());
}

static void unit_06() {
    static const char str[] = R"(true)";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_bool());
    assert(json.to_bool() == true);
    assert(!json.is_array());
    assert(!json.is_object());
    assert(!json.is_null());
    assert(!json.is_string());
    assert(!json.is_number());
}

static void unit_07() {
    static const char str[] = R"(false)";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_bool());
    assert(json.to_bool() == false);
    assert(!json.is_array());
    assert(!json.is_object());
    assert(!json.is_null());
    assert(!json.is_string());
    assert(!json.is_number());
}

static void unit_08() {
    static const char str[] = R"(null)";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_null());
    assert(!json.is_bool());
    assert(!json.is_array());
    assert(!json.is_object());
    assert(!json.is_string());
    assert(!json.is_number());
}

static void unit_09() {
    static const char str[] = R"("")";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_string());
    assert(json.to_sstring() == "");
    assert(json.to_string() == "");
    assert(!json.is_null());
    assert(!json.is_bool());
    assert(!json.is_array());
    assert(!json.is_object());
    assert(!json.is_number());
}

static void unit_10() {
    static const char str[] = R"("string")";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_string());
    assert(json.to_sstring() == "string");
    assert(json.to_string() == "string");
    assert(!json.is_null());
    assert(!json.is_bool());
    assert(!json.is_array());
    assert(!json.is_object());
    assert(!json.is_number());
}

static void unit_11() {
    static const char str[] = R"(1234)";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_number());
    assert(json.to_uint() == 1234);
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());
    assert(!json.is_array());
    assert(!json.is_object());
}

static void unit_12() {
    static const char str[] = R"(3.14)";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(!json.is_empty());
    assert(json.is_number());
    assert(json.to_double() == 3.14);
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());
    assert(!json.is_array());
    assert(!json.is_object());
}

static void unit_13() {
    static const char str[] = R"([0, "1", 3.14])";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 3);
    assert(!json.is_empty());
    assert(json.is_array());
    assert(!json.is_number());
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());
    assert(!json.is_object());

    assert(json.at(0).is_number());
    assert(json.at(0).to_uint() == 0);

    assert(json.at(1).is_string());
    assert(json.at(1).to_sstring() == "1");

    assert(json.at(2).is_number());
    assert(json.at(2).to_double() == 3.14);
}

static void unit_14() {
    static const char str[] = R"({"a":true, "b":false, "c":null, "d":0, "e":"e"})";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 5);
    assert(json.is_object());
    assert(!json.is_array());
    assert(!json.is_number());
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());

    auto j0 = json.at("a");
    assert(j0.is_valid());
    assert(j0.size() == 1);
    assert(!j0.is_empty());
    assert(j0.is_bool());
    assert(j0.to_bool() == true);
    assert(!j0.is_array());
    assert(!j0.is_object());
    assert(!j0.is_null());
    assert(!j0.is_string());
    assert(!j0.is_number());

    auto j1 = json.at("b");
    assert(j1.is_valid());
    assert(j1.size() == 1);
    assert(!j1.is_empty());
    assert(j1.is_bool());
    assert(j1.to_bool() == false);
    assert(!j1.is_array());
    assert(!j1.is_object());
    assert(!j1.is_null());
    assert(!j1.is_string());
    assert(!j1.is_number());

    auto j2 = json.at("c");
    assert(j2.is_valid());
    assert(j2.size() == 1);
    assert(!j2.is_empty());
    assert(j2.is_null());
    assert(j2.to_sstring() == "");
    assert(!j2.is_array());
    assert(!j2.is_object());
    assert(!j2.is_bool());
    assert(!j2.is_string());
    assert(!j2.is_number());

    auto j3 = json.at("d");
    assert(j3.is_valid());
    assert(j3.size() == 1);
    assert(!j3.is_empty());
    assert(j3.is_number());
    assert(j3.to_uint() == 0);
    assert(j3.to_sstring() == "0");
    assert(!j3.is_array());
    assert(!j3.is_object());
    assert(!j3.is_bool());
    assert(!j3.is_string());
    assert(!j3.is_null());

    auto j4 = json.at("e");
    assert(j4.is_valid());
    assert(j4.size() == 1);
    assert(!j4.is_empty());
    assert(j4.is_string());
    assert(j4.to_sstring() == "e");
    assert(!j4.is_array());
    assert(!j4.is_object());
    assert(!j4.is_bool());
    assert(!j4.is_number());
    assert(!j4.is_null());
}

static void unit_15() {
    static const char str[] = R"({"a":{"b":true, "c":1234}})";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(json.is_object());
    assert(!json.is_array());
    assert(!json.is_number());
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());

    auto j0 = json.at("a");
    assert(j0.is_valid());
    assert(j0.size() == 2);
    assert(j0.is_object());
    assert(!j0.is_array());
    assert(!j0.is_number());
    assert(!j0.is_string());
    assert(!j0.is_null());
    assert(!j0.is_bool());

    auto j1 = j0.at("b");
    assert(j1.is_valid());
    assert(j1.size() == 1);
    assert(j1.is_bool());
    assert(j1.to_bool() == true);
    assert(!j1.is_array());
    assert(!j1.is_number());
    assert(!j1.is_string());
    assert(!j1.is_null());
    assert(!j1.is_object());

    auto j2 = j0.at("c");
    assert(j2.is_valid());
    assert(j2.size() == 1);
    assert(j2.is_number());
    assert(j2.to_int() == 1234);
    assert(!j2.is_array());
    assert(!j2.is_string());
    assert(!j2.is_null());
    assert(!j2.is_object());
    assert(!j2.is_bool());
}

static void unit_16() {
    static const char str[] = R"({"a":[0,1,2]})";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 1);
    assert(json.is_object());
    assert(!json.is_array());
    assert(!json.is_number());
    assert(!json.is_string());
    assert(!json.is_null());
    assert(!json.is_bool());

    auto j0 = json.at("a");
    assert(j0.is_valid());
    assert(j0.size() == 3);
    assert(j0.is_array());
    assert(!j0.is_object());
    assert(!j0.is_number());
    assert(!j0.is_string());
    assert(!j0.is_null());
    assert(!j0.is_bool());
    for ( auto idx = 0u; idx < j0.size(); ++idx ) {
        const flatjson::fjson item = j0[idx];
        assert(item.is_simple_type());
        assert(item.to_uint() == idx);
    }
}

static void unit_17() {
    static const char jsstr[] = R"({"a":true, "b":{"c":{"d":1, "e":2}}, "c":[0,1,2,3]})";
    flatjson::fjson json{jsstr};
    auto str = json.dump(4);

    static const char *expected =
R"({
    "a":true,
    "b":{
        "c":{
            "d":1,
            "e":2
        }
    },
    "c":[
        0,
        1,
        2,
        3
    ]
})";
    assert(str == expected);

    std::ostringstream ss;
    json.dump(ss, 4);
    assert(ss.str() == expected);
}

static void unit_18() {
    static const char jsstr[] = R"({"a":[4,3,2,1], "b":[{"a":0,"b":0,"c":1},{"b":1,"a":0,"c":0},{"c":2,"b":0,"a":0}], "c":[0,1,2,3]})";
    flatjson::fjson json{jsstr};
    assert(json.is_valid());
    assert(json.is_object());

    const auto b = json.at("b");
    assert(b.is_array());
    for ( auto idx = 0u; idx < b.size(); ++idx ) {
        const auto o = b.at(idx);
        assert(o.is_valid());
        assert(o.is_object());
        char key[] = {static_cast<char>('a'+idx), '\0'};
        const auto v = o.at(key);
        assert(v.is_valid());
        assert(v.is_number());
        assert(v.to_uint() == idx);
    }

    const auto c = json.at("c");
    assert(c.is_array());
}

/*************************************************************************************************/

static void unit_19() {
    {
        static const char jsstr[] = R"([0])";

        flatjson::fjson json{jsstr};
        assert(json.is_valid());
        assert(json.is_array());
        assert(json.size() == 1);
        auto tok = json.tokens();
        assert(tok == 3);

        for ( auto idx = 0u; idx < json.size(); ++idx ) {
            auto item = json.at(idx);
            assert(item.is_number());
            assert(item.size() == 1);
            assert(item.to_uint() == idx);
        }
    }

    {
        static const char jsstr[] = R"([0, 1])";

        flatjson::fjson json{jsstr};
        assert(json.is_valid());
        assert(json.is_array());
        assert(json.size() == 2);
        assert(json.tokens() == 4);

        for ( auto idx = 0u; idx < json.size(); ++idx ) {
            auto item = json.at(idx);
            assert(item.is_number());
            assert(item.size() == 1);
            assert(item.to_uint() == idx);
        }
    }

    {
        static const char jsstr[] = R"([[0, 1]])";

        flatjson::fjson json{jsstr};
        assert(json.is_valid());
        assert(json.is_array());
        assert(json.size() == 1);
        assert(json.tokens() == 6);

        auto subarr = json.at(0);
        assert(subarr.is_valid());
        assert(subarr.is_array());
        assert(subarr.size() == 2);
        assert(subarr.tokens() == 4);

        for ( auto idx = 0u; idx < subarr.size(); ++idx ) {
            auto item = subarr.at(idx);
            assert(item.is_number());
            assert(item.size() == 1);
            assert(item.to_uint() == idx);
        }
    }
}

/*************************************************************************************************/

static void unit_20() {
    flatjson::fjson j0;
    assert(j0.begin() == j0.end());

    const flatjson::fjson j1;
    assert(j1.begin() == j1.end());
}

/*************************************************************************************************/

static void unit_21() {
    static const char jsstr[] = R"([0, 1, 2])";

    const flatjson::fjson json{jsstr, jsstr+sizeof(jsstr)-1};
    assert(json.is_valid());
    assert(json.is_array());
    assert(json.size() == 3);
    assert(json.tokens() == 5);

    auto it = json.begin();
    auto end = json.end();
    auto dist = std::distance(it, end);
    assert(dist == 3);

    std::ostringstream os;
    for ( ; it != end; ++it ) {
        auto dist = std::distance(json.begin(), it);
        os << "i=" << dist << ", " << it->type_name() << std::endl;
    }

    static const char res[] =
R"(i=0, NUMBER
i=1, NUMBER
i=2, NUMBER
)";
    assert(os.str() == res);
}

/*************************************************************************************************/

static void unit_22_get_keys_cb(void *userdata, const char *ptr, std::size_t len) {
    std::size_t &cnt = *static_cast<std::size_t *>(userdata);

    assert(len == 1);

    if ( cnt == 0 ) {
        assert(std::strncmp(ptr, "a", 1) == 0);
    } else if ( cnt == 1 ) {
        assert(std::strncmp(ptr, "b", 1) == 0);
    } else {
        assert(!"unreachable!");
    }

    ++cnt;
}

static void unit_22() {
    static const char jsstr[] = R"({"a":0, "b":1})";

    const flatjson::fjson json{jsstr};
    assert(json.is_valid());
    assert(json.is_object());
    assert(json.size() == 2);
    assert(json.tokens() == 4);

    const auto data = json.data();
    const auto *toksbeg= data.first;
    const auto *toksend = data.second;
    std::size_t calls_cnt{};
    int num = flatjson::fj_get_keys(toksbeg, toksend - toksbeg, &calls_cnt, &unit_22_get_keys_cb);
    assert(num == 2);
    assert(calls_cnt == 2);
    assert(json.keys_num() == 2);

    auto keys = json.keys();
    for ( auto it = keys.begin(); it != keys.end(); ++it ) {
        auto dist = std::distance(keys.begin(), it);
        switch ( dist ) {
            case 0: { assert(*it == "a"); break; }
            case 1: { assert(*it == "b"); break; }
            default:{ assert(!"unreachable!"); }
        }
    }
}

/*************************************************************************************************/

static void unit_23() {
    static const char str[] = R"({"bb":0, "b":1})";
    flatjson::fjson json(str);

    assert(json.is_valid());
    assert(json.size() == 2);
    assert(json.is_object());
    assert(json.at("b").to_int() == 1);
    assert(json.at("bb").to_int() == 0);
}

/*************************************************************************************************/

static void unit_24() {
    static const char str[] = R"({"bb":0, "b":1})";

    flatjson::fj_token<const char *> tokens[4];
    auto parser = flatjson::fj_make_parser(
         tokens
        ,sizeof(tokens)/sizeof(tokens[0])
        ,std::begin(str)
        ,std::end(str)
    );

    auto res = flatjson::fj_parse(&parser);
    assert(res.ec == flatjson::FJ_EC_OK);
    assert(res.toknum == 4);

    assert(tokens[0].is_object());

    assert(tokens[1].is_number());
    assert(tokens[1].key() == "bb");
    assert(tokens[1].value() == "0");

    assert(tokens[2].is_number());
    assert(tokens[2].key() == "b");
    assert(tokens[2].value() == "1");

    assert(tokens[3].type() == flatjson::FJ_TYPE_OBJECT_END);

    flatjson::fjson json{tokens};
    assert(json.size() == 2);
    assert(json.tokens() == 4);
    for ( auto it = json.begin(); it != json.end(); ++it ) {
        auto dist = std::distance(json.begin(), it);
        switch ( dist ) {
            case 0: {
                assert(it->key() == "bb");
                assert(it->value() == "0");
                break;
            }
            case 1: {
                assert(it->key() == "b");
                assert(it->value() == "1");
                break;
            }
            default: assert(!"unreachable");
        }
    }
}

/*************************************************************************************************/

static void unit_25() {
    static const char str[] = R"({"bb":0, "b":1})";

    flatjson::fj_token<const char *> tokens[4];
    auto parser = flatjson::fj_make_parser(
         tokens
        ,sizeof(tokens)/sizeof(tokens[0])
        ,std::begin(str)
        ,std::end(str)
    );

    auto res = flatjson::fj_parse(&parser);
    assert(res.ec == flatjson::FJ_EC_OK);
    assert(res.toknum == 4);

    assert(tokens[0].is_object());

    assert(tokens[1].is_number());
    assert(tokens[1].key() == "bb");
    assert(tokens[1].value() == "0");

    assert(tokens[2].is_number());
    assert(tokens[2].key() == "b");
    assert(tokens[2].value() == "1");

    assert(tokens[3].type() == flatjson::FJ_TYPE_OBJECT_END);
}

/*************************************************************************************************/

static void unit_26() {
    static const char str[] = R"({"bb":0, "b":1})";
    const flatjson::fjson json{str};

    assert(json.is_valid());
    assert(json.size() == 2);
    assert(json.is_object());

    for ( auto it = json.begin(), end = json.end(); it != end; ++it ) {
        auto dist = std::distance(json.begin(), it);
        switch ( dist ) {
            case 0: {
                assert(it->key() == "bb");
                assert(it->value() == "0");
                break;
            }
            case 1: {
                assert(it->key() == "b");
                assert(it->value() == "1");
                break;
            }
            default: assert(!"unreachable");
        }
    }
}

/*************************************************************************************************/

static void unit_27() {
    static const char str[] = R"({"a":0, "b":1, "c":{"d":2, "e":3}, "f":4})";
    const flatjson::fjson json{str};

    assert(json.is_valid());
    assert(json.size() == 4);
    assert(json.is_object());

    for ( auto it = json.begin(), end = json.end(); it != end; ++it ) {
        auto dist = std::distance(json.begin(), it);
        switch ( dist ) {
            case 0: {
                assert(it->key() == "a");
                assert(it->value() == "0");
                break;
            }
            case 1: {
                assert(it->key() == "b");
                assert(it->value() == "1");
                break;
            }
            case 2: {
                assert(it->key() == "c");
                assert(it->is_object());
                break;
            }
            case 3: {
                assert(it->key() == "f");
                assert(it->value() == "4");
                break;
            }
            default: assert(!"unreachable");
        }
    }
}

/*************************************************************************************************/

static void unit_28() {
    static const char str[] = R"({"a":0, "b":[0,1,2]})";
    flatjson::fjson json{str};

    assert(json.is_valid());
    assert(json.size() == 2);
    assert(json.is_object());

    auto b = json.at("b");
    auto it = b.begin();
    auto end = b.end();
    auto dist = std::distance(it, end);
    assert(dist == 3);

    std::ostringstream os;
    for ( ; it != end; ++it ) {
        auto dist = std::distance(b.begin(), it);
        os << "i=" << dist << ", " << it->type_name() << std::endl;
    }

    static const char res[] =
R"(i=0, NUMBER
i=1, NUMBER
i=2, NUMBER
)";
    assert(os.str() == res);
}

/*************************************************************************************************/

static void unit_29() {
    static const char str[] = R"({"a":0, "b":1, "c":{"d":2, "e":3}, "f":4})";
    const flatjson::fjson json{str};

    assert(json.is_valid());
    assert(json.size() == 4);
    assert(json.is_object());

    for ( auto it = json.begin(), end = json.end(); it != end; ++it ) {
        auto dist = std::distance(json.begin(), it);
        switch ( dist ) {
            case 0: {
                assert(it->key() == "a");
                assert(it->value() == "0");
                break;
            }
            case 1: {
                assert(it->key() == "b");
                assert(it->value() == "1");
                break;
            }
            case 2: {
                assert(it->key() == "c");
                assert(it->is_object());
                assert(it->childs() == 3);
                auto c = json.at(it);
                for ( auto oit = c.begin(); oit != c.end(); ++oit ) {
                    dist = std::distance(c.begin(), oit);
                    switch ( dist ) {
                        case 0: {
                            assert(oit->key() == "d");
                            assert(oit->value() == "2");
                            break;
                        }
                        case 1: {
                            assert(oit->key() == "e");
                            assert(oit->value() == "3");
                            break;
                        }
                        default: assert(!"unreachable");
                    }
                }

                break;
            }
            case 3: {
                assert(it->key() == "f");
                assert(it->value() == "4");
                break;
            }
            default: assert(!"unreachable");
        }
    }
}

/*************************************************************************************************/

static void unit_30() {
    static const char str[] = R"({"bb":0, "b":1})";
    flatjson::fj_token<const char *> tokens[5];

    {
        auto parser = flatjson::fj_make_parser(
             tokens
            ,sizeof(tokens)/sizeof(tokens[0])
            ,std::begin(str)
            ,std::end(str)
        );

        auto res = flatjson::fj_parse(&parser);
        assert(res.ec == flatjson::FJ_EC_OK);
        assert(res.toknum == 4);

        flatjson::fjson json(std::begin(tokens), std::end(tokens));

        assert(json.is_valid());
        assert(json.is_object());
        assert(json.size() == 2);

        assert(json.begin()->is_number());
        assert(json.begin()->key() == "bb");
        assert(json.begin()->value() == "0");

        assert(std::next(json.begin())->is_number());
        assert(std::next(json.begin())->key() == "b");
        assert(std::next(json.begin())->value() == "1");

        assert(std::next(std::next(json.begin()))->type() == flatjson::FJ_TYPE_OBJECT_END);
    }

    {
        flatjson::fjson json(std::begin(tokens), std::end(tokens), std::begin(str), std::end(str));

        assert(json.is_valid());
        assert(json.is_object());
        assert(json.size() == 2);

        assert(json.begin()->is_number());
        assert(json.begin()->key() == "bb");
        assert(json.begin()->value() == "0");

        assert(std::next(json.begin())->is_number());
        assert(std::next(json.begin())->key() == "b");
        assert(std::next(json.begin())->value() == "1");

        assert(std::next(std::next(json.begin()))->type() == flatjson::FJ_TYPE_OBJECT_END);
    }
}

/*************************************************************************************************/

int main() {
    std::cout << "sizeof(fj_token) = " << sizeof(flatjson::fjson::element_type) << std::endl;

    auto res = test_conformance();
    if ( res.ec ) {
        std::cout << "test \"" << res.name << "\" FAILED!" << std::endl;
    }

    FJ_RUN_TEST(unit_00);
    FJ_RUN_TEST(unit_01);
    FJ_RUN_TEST(unit_02);
    FJ_RUN_TEST(unit_03);
    FJ_RUN_TEST(unit_04);
    FJ_RUN_TEST(unit_05);
    FJ_RUN_TEST(unit_06);
    FJ_RUN_TEST(unit_07);
    FJ_RUN_TEST(unit_08);
    FJ_RUN_TEST(unit_09);
    FJ_RUN_TEST(unit_10);
    FJ_RUN_TEST(unit_11);
    FJ_RUN_TEST(unit_12);
    FJ_RUN_TEST(unit_13);
    FJ_RUN_TEST(unit_14);
    FJ_RUN_TEST(unit_15);
    FJ_RUN_TEST(unit_16);
    FJ_RUN_TEST(unit_17);
    FJ_RUN_TEST(unit_18);
    FJ_RUN_TEST(unit_19);
    FJ_RUN_TEST(unit_20);
    FJ_RUN_TEST(unit_21);
    FJ_RUN_TEST(unit_22);
    FJ_RUN_TEST(unit_23);
    FJ_RUN_TEST(unit_24);
    FJ_RUN_TEST(unit_25);
    FJ_RUN_TEST(unit_26);
    FJ_RUN_TEST(unit_27);
    FJ_RUN_TEST(unit_28);
    FJ_RUN_TEST(unit_29);
    FJ_RUN_TEST(unit_30);

    return EXIT_SUCCESS;
}

/*************************************************************************************************/
