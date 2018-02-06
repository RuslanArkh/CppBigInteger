#include "big_integer_1_a.h"
#include <ctime>

int main()
{
    char * digitString0 = "1000000000065445677799800000000000000000000000";	//	4294967296
    clock_t clock1, clock2;

    //clock1 = clock();
    BigInt d1(digitString0);

    BigInt d2;
    d2 = "1000000000000000000543211460000000000";
    BigInt d3;
    d3 = d1 + d2;
    std::cout << d1 << "\n+\n";
    std::cout << d2 << "\n=\n";
    std::cout << "d3:\n";
    std::cout << "Decimal number: \n";
    std::cout << d3 << std::endl;
    std::cout << "Binary number: \n";
    d3.PrintBits();
    std::cout << "Number of bits: \n";
    std::cout << BigInt::getNumBits(d3.getBinarySet());
    std::cout << std::endl;

    //	Checking the bit
    std::cout << "\nEnter position to check bit, or press 0 to continue!\n";
    while (true)
    {
        int pos;
        std::cout << "Pos: ";
        std::cin >> pos;
        if (std::cin.fail())
        {
            std::cout << "Invalid data type!\n";
            std::cin.clear();
            std::cin.ignore(999, '\n');
            continue;
        }
        if (pos == 0)
            break;
        bool boolBit;
        try
        {
            boolBit = d3.getBit(pos);
        }
        catch (const char * e)
        {
            std::cout << e << std::endl;
            continue;
        }
        std::cout << "Bit is: " << boolBit << "\n";
    }

    //	Setting the bit
    d3.setBit(3, true);
    assert(d3.getBit(3));
    std::cout << "\nBinary number after setting a bit to position 3: \n";
    d3.PrintBits();

    return 0;
}
