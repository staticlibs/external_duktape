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


#include <iostream>

#include "duktape.h"

#include "staticlib/config/assert.hpp"


void test_duktape() {
    duk_context* ctx = duk_create_heap_default();
    slassert(nullptr != ctx);
    duk_push_string(ctx, "print('Hello world!'); 42;");
    duk_eval(ctx);
    int num = static_cast<int>(duk_get_number(ctx, -1));
    slassert(42 == num);
    duk_pop(ctx);
    duk_destroy_heap(ctx);
}

int main() {
    try {
        test_duktape();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
