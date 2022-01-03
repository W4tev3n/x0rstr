#pragma once

/*
MIT License

Copyright (c) 2022 W4tev3n

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if _MSC_VER 
#define __forceinline__ __forceinline
#else
#define __forceinline__ __attribute__((always_inline))
#endif

namespace xorstr {
    template<size_t n>
    struct string_literal {
        constexpr string_literal(const char(&str)[n]) {
            for (int i = 0; i < 16; i++) //For the whatever reason, MSVC keeps optimizing strings smaller than 16 bytes. Here's a dirty fix for that.
                value[i] = '\0';
            std::copy_n(str, n, value);
        }
        char value[n < 16 ? 16 : n];
    };

    template <string_literal lit, unsigned long key>
    class string_hidden {
    private:
        char value[sizeof(lit.value)];
    public:
        __forceinline__ char* decrypt() {
            char str[sizeof(lit.value)]{}; //Keep in mind that if you ever want to remove forced inlining, you should allocate the buffer on the heap.
            unsigned long x{ key };
            for (size_t i = 0; i < sizeof(lit.value); i++) {
                //As the seed of the xorshift32 is the same, we will get the exact same sequence of characters.
                x ^= x << 13;
                x ^= x >> 17;
                x ^= x << 5;
                str[i] = value[i] ^ x;
            }
            return str;
        }

        //This NEEDS to be consteval, cuz othervise compiler would just inline it, leaving the string untouched.
        consteval string_hidden() {
            unsigned long x{ key };
            for (size_t i = 0; i < sizeof(lit.value); i++) {
                //Here i'm using xorshift32 to generate random characters sequence from a key.
                x ^= x << 13;
                x ^= x >> 17;
                x ^= x << 5;
                value[i] = lit.value[i] ^ x;
            }
        }
    };

    //PJW lets us to generate unique keys for every string.
    consteval unsigned long pjw(const char* s) {
        unsigned long h{}, high{};
        while (*s) {
            h = (h << 4) + *s++;
            if (high = h & 0xF0000000)
                h ^= high >> 24;
            h &= ~high;
        }
        return h;
    }

#pragma optimize( "", off )
    template <string_literal lit>
    char* str() {
        auto str = string_hidden<lit, pjw(lit.value)>();
        return str.decrypt();
    }
#pragma optimize( "", on )
};

#define xorstr_(_str) xorstr::str<_str>()
#undef __forceinline__