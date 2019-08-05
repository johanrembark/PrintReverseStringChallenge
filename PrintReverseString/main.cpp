#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <chrono>
#include <iostream>

typedef void(*PrintCharFunc)(char C);

//#define CPPIMPL // Use the cpp implementation
#define BENCHMARK // Run benchmark

#ifdef CPPIMPL
__declspec(noinline) int PrintReverseString(const char* __restrict String, int CharCount, PrintCharFunc PrintChar)
{
	int StrLength = 0;
	for (int i = 0; i < CharCount; ++i)
	{
		if (String[i] == '\0')
		{
			break;
		}

		StrLength++;
	}

	for (int i = StrLength - 1; i >= 0; i--)
	{
		char NextChar = String[i];

		PrintChar(NextChar);
	}
	return StrLength;
}
#endif

// Print Min(strlen(String), CharCount) characters from String in reverse order
// using the PrintChar function.
// Return the number of characters printed.
// E.g.
//   PrintReverseString("FooBar", 4) prints "BooF" and returns 4
//   PrintReverseString("Foo", 4) prints "ooF" and returns 3
#ifndef CPPIMPL
extern "C" int PrintReverseString(const char* String, int CharCount, PrintCharFunc PrintChar);
#else
int PrintReverseString(const char* String, int CharCount, PrintCharFunc PrintChar);
#endif
extern "C" int PrintHelloWorld(PrintCharFunc PrintChar);

__declspec(noinline) void DoPrintChar(char C)
{
#ifndef BENCHMARK
	printf("%c", C);
#endif
}

int main()
{
#ifdef BENCHMARK
	auto Start = std::chrono::high_resolution_clock::now();
	int NumIterations = 100000000;
	for (int i = 0; i < NumIterations; ++i)
	{
		int N = PrintReverseString("FooBar", 4, DoPrintChar);
	}
	auto End = std::chrono::high_resolution_clock::now();
	auto Duration = (End - Start) / std::chrono::milliseconds(1);
	std::cout << "NumIterations: " << NumIterations << " Result = " << Duration << "ms";

	int WaitForInput;
	std::cin >> WaitForInput;
#else
	//PrintHelloWorld(DoPrintChar); // what would a library be if it didn't have an implementation to print hello world ;)
	//printf("\n");

	int N = PrintReverseString("FooBar", 4, DoPrintChar);
	assert(N == 4);
	printf("\n");

	N = PrintReverseString("Foo", 4, DoPrintChar);
	assert(N == 3);
	printf("\n");
#endif
	return 0;
}