# BigIntCalculator

### Info

This is calculator created for BigInt numbers in C++. Works with negative and positive numbers.

#### Contents

* [Info](#Info)
* [Actions list](#Actions)

## Actions

Except operators shown below. There are operators like `i--`, `i++`, `bool ones` and `output/input`.
Operators like `*=` are also overloaded.

```c++
BigNumber first("100000000")      #Example
BigNumber second ("200")          #Example

first + second;                   #Addition                
//Output: 100000200;

first - second;                   #Subtraction
//Output: 99999800;

first * second;                   #Multiplication
//Output: 20000000000;

first / second;                   #Division
//Output: 500000;

first % second;                   #Modulus
//Output: 0;

± first                           #Unary minus
//Output: -100000000;
  
AC                                #Clears the number inputed
//Output: ;
```
