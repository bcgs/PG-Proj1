#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "ponto.h"
#include "math.h"
#include <vector>

namespace MathUtil {

    unsigned long long factorial(int n)
    {
        long long fat;
        
        if (n <= 1) {
            fat = 1;
        } else {
            fat = n;
            while (n > 1) {
                fat *= (n - 1);
                n--;
            }
        }
        return fat;
    }
    
    signed long long comb(int n, int i)
    {
        unsigned long long ret = factorial(n)/(factorial(i) * factorial(n - i));
        return ret;
    }
    
    inline void bezier(const std::vector<Ponto>& pontos, float t, Ponto& pout)
    {
        float x = 0.0;
        float y = 0.0;
        const unsigned int n = (unsigned int)pontos.size()-1;
        
        for (int i = 0; i < pontos.size(); i++) {
            x += comb(n,i) * pow((1-t), n-i) * pow(t,i) * pontos[i].x;
            y += comb(n,i) * pow((1-t), n-i) * pow(t,i) * pontos[i].y;
        }
        pout.x = x;
        pout.y = y;
    }

    inline void derivative(const std::vector<Ponto>& pontos, float t, Ponto& pout)
    {
    	float x = 0.0;
		float y = 0.0;
		const unsigned int n = (unsigned int)pontos.size()-1;

		for (int i = 0; i < pontos.size(); i++) {
			x += (-comb(n,i) * (n-i) * pow((1-t), n-i-1) * pow(t,i) + comb(n,i) * i * pow((1-t),n-i) * pow(t,i-1)) * pontos[i].x;
			y += (-comb(n,i) * (n-i) * pow((1-t), n-i-1) * pow(t,i) + comb(n,i) * i * pow((1-t),n-i) * pow(t,i-1)) * pontos[i].y;
		}
		pout.x = x;
		pout.y = y;
    }
}

#endif
