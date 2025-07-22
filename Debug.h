/*

  MIT Licence
Author -> Brett Rogers
Reference -> Pythagorious


this file is for printing to the console

*/

#include <cstddef>

extern "C" long syscall(long number, ...); //<----- credit chatgpt
/*
extern "C" long syscall(long number, ...);
This line declares the system-provided syscall function.
It's a generic entry point to Linux system calls,
where I pass the syscall number first — like 1 for write — followed
by any syscall arguments. I declare it with extern "C" so the linker
can find the real system-level implementation, which uses C naming conventions.

*/

constexpr long SYS_write = 1; //<------- credit chatgpt

/*

I'm defining a compile-time constant named SYS_writewith the value 1,
which is the syscall number forwriteon Linux. Usingconstexpr ensures
it's known at compile time, and gives my code better clarity and portability
than using a raw number.

*/

namespace Debug 
{


struct Print 
{

  void operator()(const char *s) const {
    std::size_t len = 0;
    while (s[len] != '\0') {
      ++len;
    }

    syscall(SYS_write, 1, s, len);
  }

  void operator()(int n) const {
    char buf[32]; 
    int i = 30;
    buf[31] = '\0';

    bool isNegative = n < 0;
    unsigned int num = isNegative ? -n : n;

    do {
      buf[i--] = '0' + (num % 10);
      num /= 10;
    } while (num > 0);

    if (isNegative) {
      buf[i--] = '-';
    }

    syscall(SYS_write, 1, &buf[i + 1], 30 - i);
  }


};

} 
