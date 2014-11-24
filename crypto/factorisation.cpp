#include "factorisation.h"

template<class T>
T min(T a, T b)
{
    if (a>b)
        return b;
    else
        return a;
}

unsigned long long gcd(unsigned long long a, unsigned long long b)
{
    while (a != 0 && b != 0) {
        while ((b & 1) == 0) {
            b >>= 1;
        }
        while ((a & 1) == 0) {
            a >>= 1;
        }
        if (a > b) {
            a -= b;
        } else {
            b -= a;
        }
    }
    return b == 0 ? a : b;
}

bool isPrime(unsigned long long n)
{
    if (n <= 3)
    {
        return n > 1;
    }
    else if (n % 2 == 0 || n % 3 == 0)
    {
        return 0;
    }
    else
    {
        for (unsigned long long i = 5; i * i < n; i += 6)
        {
            if (n % i == 0 || n % (i + 2) == 0)
            {
                return 0;
            }
        }
        return 1;
    }
}

unsigned long long min (const unsigned long long a, const unsigned long long b)
{
    return !(b < a) ? a : b;
}

unsigned long long factorize(unsigned long long N)
{
    unsigned long long g = 0;
    int it = 0;

    for (int i = 0; i < 3; i++)
    {
        int q = ((rand() % 128) & 15) + 17;
        unsigned long long x = (rand() % 1000000000) + 1;
        unsigned long long y = x;
        int lim = 1 << (i + 18);
        for (int j = 1; j < lim; ++j)
        {
            ++it;
            unsigned long long a = x;
            unsigned long long b = x;
            unsigned long long c = q;
            while (b != 0)
            {
                if ((b & 1) != 0)
                {
                    c += a;
                    if (c >= N)
                    {
                        c -= N;
                    }
                }
                a += a;
                if (a >= N)
                {
                    a -= N;
                }
                b >>= 1;
            }
            x = c;
            unsigned long long z = x < y ? y - x : x - y;
            g = gcd(z, N);
            if (g != 1)
            {
                break;
            }
            if ((j & (j - 1)) == 0)
            {
                y = x;
            }
        }
        if (g > 1)
        {
            break;
        }
    }

    unsigned long long p = N / g;

    return min(p, g);
}