#include <string>
#include <iomanip>
#include <vector>
#include <math.h>
#include <iostream>
#include "MD5.h"

std::string toBinary(std::uint32_t n)
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

int main()
{
    std::string message;
    for (int i = 0; i < 447; i++)
    {
        if (i % 1 == 0)
            message.append("1");
        else
            message.append("0");
    }
    message.append("1");
    const int MESSAGE_LEN = message.length();
    int paddedMessageLength = MESSAGE_LEN;
    std::string stringMESSAGE_LEN = toBinary(MESSAGE_LEN);
    int stringMESSAGE_LEN_Length = stringMESSAGE_LEN.length();
    if (stringMESSAGE_LEN_Length < 64)
    {
        for (int i = 0; i < (64 - stringMESSAGE_LEN_Length); i++)
        {
            stringMESSAGE_LEN.insert(0, "0");
        }
    }

    while (paddedMessageLength % 512 != 448)
    {
        message.append("0");
        paddedMessageLength = message.length();
    }
    message.append(stringMESSAGE_LEN);
    MD5 md5(message);
    std::cout << md5.getDigest() << std::endl;
}





/*
std::string toBinary(std::uint32_t);
std::uint32_t toInt(std::string);
std::uint32_t leftRotate(std::uint32_t, std::uint32_t);
std::string formatOutput(std::uint32_t);
std::string MD5Hash(std::string);

int main()
{
    std::string message;
    for (int i = 0; i < 447; i++)
    {
        if (i % 1 == 0)
            message.append("1");
        else
            message.append("0");
    }
    message = MD5Hash(message);
    std::cout << message << std::endl;
}

std::string toBinary(std::uint32_t n)
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

std::uint32_t toInt(std::string binaryString)
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

std::uint32_t leftRotate(std::uint32_t x, std::uint32_t c)
{
    return (x << c) | (x >> (32 - c));
}

std::string formatOutput(std::uint32_t input)
{
    std::ostringstream output;
    output << std::internal << std::hex << std::setfill('0') <<
    std::setw(8) << input;
    return output.str();
}

std::string MD5Hash(std::string message)
{
    message.append("1");
    const int MESSAGE_LEN = message.length();
    int paddedMessageLength = MESSAGE_LEN;
    std::string stringMESSAGE_LEN = toBinary(MESSAGE_LEN);
    int stringMESSAGE_LEN_Length = stringMESSAGE_LEN.length();
    if (stringMESSAGE_LEN_Length < 64)
    {
        for (int i = 0; i < (64 - stringMESSAGE_LEN_Length); i++)
        {
            stringMESSAGE_LEN.insert(0, "0");
        }
    }

    while (paddedMessageLength % 512 != 448)
    {
        message.append("0");
        paddedMessageLength = message.length();
    }
    message.append(stringMESSAGE_LEN);
    paddedMessageLength = message.length();

    std::vector<std::string> messageChunks;
    std::string messageChunk;
    int numberOfChunks = paddedMessageLength / 512;
    std::cout << "number of chunks: " << numberOfChunks << std::endl;
    std::cout << "padded length: " << paddedMessageLength << std::endl;
    for (int i = 0; i < numberOfChunks; i++)
    {
        messageChunk = message.substr((i * 512), 512);
        messageChunks.push_back(messageChunk);
    }

    unsigned int shiftAmount_s[64] =
    {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    std::uint32_t sinesOfIntegers_K[64] =
    {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    std::uint32_t a0 = 0x67452301;
    std::uint32_t b0 = 0xefcdab89;
    std::uint32_t c0 = 0x98badcfe;
    std::uint32_t d0 = 0x10325476;

    for (int i = 0; i < messageChunks.size(); i++)
    {
        std::uint32_t A = a0;
        std::uint32_t B = b0;
        std::uint32_t C = c0;
        std::uint32_t D = d0;

        std::uint32_t M[16];
        std::string segment;
        for (int j = 0; j < 16; j++)
        {
            segment = messageChunks[i].substr((j * 32), 32);
            M[j] = toInt(segment);
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
    }

    std::string digest;
    digest.append(formatOutput(a0));
    digest.append(formatOutput(b0));
    digest.append(formatOutput(c0));
    digest.append(formatOutput(d0));
    return digest;
}
*/
