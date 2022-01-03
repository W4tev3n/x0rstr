# x0rstr
A single-header compile time string encryption library.

## Example
C++20:
```cpp
int main(int argc, char* argv[]) {
    puts(xorstr_("secret string!"));
    return 0;
}
```
x64 Assembly:
```x86asm
; int __cdecl main(int argc, const char **argv, const char **envp)
main proc near

var_38= xmmword ptr -38h
Buffer= byte ptr -28h
var_18= qword ptr -18h

; __unwind { // __GSHandlerCheck
sub     rsp, 58h
mov     rax, cs:__security_cookie
xor     rax, rsp
mov     [rsp+58h+var_18], rax
movdqa  xmm0, cs:__xmm@3883e86865e0cc5cb47070455bf4541d
mov     eax, 1104571h
movdqu  [rsp+58h+var_38], xmm0
xor     edx, edx
xorps   xmm0, xmm0
movups  xmmword ptr [rsp+58h+Buffer], xmm0
loc_140001720:
mov     ecx, eax
shl     ecx, 0Dh
xor     eax, ecx
mov     ecx, eax
shr     ecx, 11h
xor     eax, ecx
mov     ecx, eax
shl     ecx, 5
xor     eax, ecx
movzx   ecx, al
xor     cl, byte ptr [rsp+rdx+58h+var_38]
mov     [rsp+rdx+58h+Buffer], cl
inc     rdx
cmp     rdx, 10h
jb      short loc_140001720
lea     rcx, [rsp+58h+Buffer] ; Buffer
call    cs:__imp_puts
xor     eax, eax
mov     rcx, [rsp+58h+var_18]
xor     rcx, rsp        ; StackCookie
call    __security_check_cookie
add     rsp, 58h
retn
; } // starts at 1400016F0
main endp
```
HexRays Decompiler:
```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v3; // eax
  unsigned __int64 v4; // rdx
  unsigned int v5; // eax
  __m128i si128; // [rsp+20h] [rbp-38h]
  char Buffer[16]; // [rsp+30h] [rbp-28h] BYREF

  v3 = 17843569;
  si128 = _mm_load_si128((const __m128i *)&_xmm);
  v4 = 0i64;
  *(_OWORD *)Buffer = 0i64;
  do
  {
    v5 = (((v3 << 13) ^ (unsigned int)v3) >> 17) ^ (v3 << 13) ^ v3;
    v3 = (32 * v5) ^ v5;
    Buffer[v4] = si128.m128i_i8[v4] ^ v3;
    ++v4;
  }
  while ( v4 < 0x10 );
  puts(Buffer);
  return 0;
}
```

## License
[MIT](https://choosealicense.com/licenses/mit/)