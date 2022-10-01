#include <stdio.h>
// #include "flgv1.cpp"
#include "flgv2.cpp"

/* Flags list
    p - path
    i - info

    "" - no flags
    0(p0) - exponential_Example(): funtion begin
    0(i1) - exponential_Example(): 0^x useless operation
    0(i2) - exponential_Example(): x^0 useless operation
    0(p3) - exponential_Example(): 1^x returns 1
    0(p4) - exponential_Example(): x^1 returns x
    0(p5) - exponential_Example(): a^b where b is odd number, recursion
    0(p6) - exponential_Example(): a^b where b is even number, recursion
*/

flg_start

unsigned long long exponential_Example(unsigned long long, unsigned long long);

int main(){
    // a^b
    int a = 123;
    int b = 0;
    printf("\t\t%d^%d = %d\n",a,b,exponential_Example(a,b));
    flg_print1("all flags:")
    flg_print2("informative flags:", "i")
    flg_print2("executed path flags:", "p")
    printf("\t\t%d^%d = %d\n",a,b,exponential_Example(a,b));
    flg_print1("all flags:")
    flg_print2("informative flags:", "i")
    flg_print2("executed path flags:", "p")
    a = 8;
    b = 7;
    flg_clear
    printf("\t\t%d^%d = %d\n",a,b,exponential_Example(a,b));
    flg_print1("all flags:")
    flg_print2("informative flags:", "i")
    flg_print2("executed path flags:", "p")
}

unsigned long long exponential_Example(unsigned long long a, unsigned long long b){
    flg(0,'p',0);
    // a^b
    if(a == 0){         // 0^x
        flg(0,'i',1)
        return a;
    }
    else if(b == 0){    // x^0 
        flg(0,'i',2)
        return 1;
    }
    else if(a == 1){    // 1^x
        flg(0,'p',3)
        return a;
    }
    else if(b == 1){    // x^1
        flg(0,'p',4)
        return a;
    }

    if(b % 2){
        flg(0,'p',5)
        return (exponential_Example(a*a, b >> 1) * a);
    }
    else{
        flg(0,'p',6)
        return exponential_Example(a*a, b >> 1);
    }
}