#include "MD5.h"

MD5::MD5(std::string message)
{
    this->message = message;
    a0 = 0x67452301;
    b0 = 0xefcdab89;
    c0 = 0x98badcfe;
    d0 = 0x10325476;
    MD5Hash();
}

std::string MD5::toBinary(std::uint32_t n)
{
    std::string output;
    while (n != 0)
    {
        if (n % 2 == 0)
        {
            output.insert(0, "0");
        }
        else
        {
            output.insert(0, "1");
        }
        n /= 2;
    }
    return output;
}

std::uint32_t MD5::toInt(std::string binaryString)
{
    int numberOfDigits = binaryString.length();
    std::uint32_t output = 0;
    for (int i = 0; i < numberOfDigits; i++)
    {
        if (binaryString[numberOfDigits - (i + 1)] == '1')
        {
            output += (std::uint32_t)pow(2, i);
        }
    }
    return output;
}

std::uint32_t MD5::leftRotate(std::uint32_t x, std::uint32_t c)
{
    return (x << c) | (x >> (32 - c));
}

std::string MD5::formatOutput(std::uint32_t input)
{
    std::ostringstream output;
    output << std::internal << std::hex << std::setfill('0') <<
        std::setw(8) << input;
    return output.str();
}

void MD5::MD5Hash()
{
    std::uint32_t A = a0;
    std::uint32_t B = b0;
    std::uint32_t C = c0;
    std::uint32_t D = d0;

    std::uint32_t M[16];
    std::string segment;
    for (int i = 0; i < 16; i++)
    {
        segment = message.substr((i * 32), 32);
        M[i] = toInt(segment);
    }

    for (int k = 0; k < 64; k++)
    {
        std::uint32_t F;
        int g;
        if (k >= 0 && k <= 15)
        {
            F = ((B & C) | ((~B) & D)) & 0xFFFFFFFF;
            g = k;
        }
        else if (k >= 16 && k <= 31)
        {
            F = ((D & B) | ((~D) & C)) & 0xFFFFFFFF;
            g = (5 * k + 1) % 16;
        }
        else if (k >= 32 && k <= 47)
        {
            F = (B ^ C ^ D) & 0xFFFFFFFF;
            g = (3 * k + 5) % 16;
        }
        else if (k >= 48 && k <= 63)
        {
            F = (C ^ (B | (~D))) & 0xFFFFFFFF;
            g = (7 * k) % 16;
        }
        F = (F + A + sinesOfIntegers_K[k] + M[g]) & 0xFFFFFFFF;
        A = D;
        D = C;
        C = B;
        B = (B + leftRotate(F, shiftAmount_s[k])) & 0xFFFFFFFF;
    }

    a0 = (a0 + A) & 0xFFFFFFFF;
    b0 = (b0 + B) & 0xFFFFFFFF;
    c0 = (c0 + C) & 0xFFFFFFFF;
    d0 = (d0 + D) & 0xFFFFFFFF;

    digest.append(formatOutput(a0));
    digest.append(formatOutput(b0));
    digest.append(formatOutput(c0));
    digest.append(formatOutput(d0));
}

std::string MD5::getDigest()
{
    return this->digest;
}


