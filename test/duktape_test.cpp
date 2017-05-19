/*
 * Copyright 2017, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   duktape_test.cpp
 * Author: alex
 *
 * Created on February 10, 2015, 10:08 AM
 */


#include "duktape.h"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "staticlib/config/assert.hpp"

#include "staticlib/support.hpp"


void test_eval() {
    duk_context* ctx = duk_create_heap_default();
    slassert(nullptr != ctx);
    slassert(0 == duk_get_top(ctx));
    duk_push_string(ctx, "print('Hello world!'); 42;");
    slassert(1 == duk_get_top(ctx));
    slassert(0 == duk_peval(ctx));
    slassert(1 == duk_get_top(ctx));
    slassert(42 == static_cast<int> (duk_get_number(ctx, -1)));
    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    duk_destroy_heap(ctx);
}

void test_dump_load() {
    duk_context* ctx = duk_create_heap_default();
    slassert(nullptr != ctx);
    slassert(0 == duk_get_top(ctx));
    duk_push_string(ctx, "var myfun = function() { print('Hello world!'); return 42; }");
    duk_push_string(ctx, "test/hello.js");
    slassert(2 == duk_get_top(ctx));
    slassert(0 == duk_pcompile(ctx, DUK_COMPILE_EVAL));
    slassert(1 == duk_get_top(ctx));
    duk_dump_function(ctx);
    slassert(1 == duk_get_top(ctx));
    size_t sz;
    auto buf_ptr = duk_get_buffer(ctx, -1, std::addressof(sz));
    slassert(nullptr != buf_ptr);
    std::vector<unsigned char> vec;
    vec.resize(sz);
    std::memcpy(vec.data(), buf_ptr, sz);
    
    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    
    duk_push_fixed_buffer(ctx, vec.size());
    slassert(1 == duk_get_top(ctx));
    size_t load_size;
    auto load_ptr = duk_get_buffer(ctx, -1, std::addressof(load_size));
    slassert(nullptr != load_ptr);
    slassert(load_size == vec.size());
    std::memcpy(load_ptr, vec.data(), vec.size());
    
    duk_load_function(ctx);
    slassert(1 == duk_get_top(ctx));
    printf("load result type: %d\n", (int) duk_get_type(ctx, -1));
    slassert(0 == duk_pcall(ctx, 0));
    slassert(1 == duk_get_top(ctx));
    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    
    duk_push_string(ctx, "myfun()");
    slassert(0 == duk_peval(ctx));
    slassert(42 == static_cast<int> (duk_get_number(ctx, -1)));
        
    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    duk_destroy_heap(ctx);
}

void test_errors() {
    duk_context* ctx = duk_create_heap(nullptr, nullptr, nullptr, nullptr, 
            [] (duk_context*, duk_errcode_t code, const char* msg) {
        std::string err = std::string("Duktape error, code: [") + sl::support::to_string(code) + 
                "], message: [" + msg + "]";
        throw std::runtime_error(err);
    });
    slassert(nullptr != ctx);
    slassert(0 == duk_get_top(ctx));

    duk_push_string(ctx, "AAAAAA");
    duk_push_string(ctx, "test/fail.js");
    slassert(2 == duk_get_top(ctx));
//    duk_compile(ctx, DUK_COMPILE_FUNCTION);
    slassert(0 != duk_pcompile(ctx, DUK_COMPILE_FUNCTION));

    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    duk_destroy_heap(ctx);
}

duk_ret_t my_native_func(duk_context* ctx) {
    size_t len;
    const char* input = duk_get_lstring(ctx, 0, std::addressof(len));
    std::cout << "my_native_func called" << std::endl;
    std::cout << len << std::endl;
    std::cout << std::string(input) << std::endl;
    duk_push_string(ctx, "my func returned");
    return 1;
}


void test_call_native() {
    duk_context* ctx = duk_create_heap_default();
    slassert(nullptr != ctx);
    slassert(0 == duk_get_top(ctx));
    duk_push_global_object(ctx);
    slassert(1 == duk_get_top(ctx));
    duk_push_c_function(ctx, my_native_func, 1 /*nargs*/);
    slassert(2 == duk_get_top(ctx));
    duk_put_prop_string(ctx, -2, "my_native_func");
    slassert(1 == duk_get_top(ctx));
    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    
    slassert(0 == duk_peval_string(ctx, "my_native_func('foo bar baz');"));
    slassert(1 == duk_get_top(ctx));
    slassert("my func returned" == std::string(duk_get_string(ctx, -1)));

    duk_pop(ctx);
    slassert(0 == duk_get_top(ctx));
    duk_destroy_heap(ctx);
}

int main() {
    try {
        test_eval();
        test_dump_load();
        test_errors();
        test_call_native();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
