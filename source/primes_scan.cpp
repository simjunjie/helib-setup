// Created by btan on 6/12/16.
// #include <iostream>
#include <cmath>
#include "NTL/ZZ.h"
#include "NTL/ZZ_p.h"
#include "helib/PAlgebra.h"

using namespace helib;

bool isPrime(uint64_t n)
{
	for (uint64_t i = 2; i <= sqrt(n); i++)
		if (n % i == 0)
			return false;

	return true;
}
float eulerTot(uint64_t n)
{
	float result = n;
	// Compute prime factors and execute multiplication.
	if(NTL::ProbPrime(n))
		result -= 1;
	else
	{
		for (uint64_t i = 2; i <= n / 2; i++)
			if (n % i == 0 && NTL::ProbPrime(i))
				result *= (1 - (1 / (float) i));
	}
	std::cout << result << std::endl;
	return result;
}
// Modified from Wen-jie Lu @fionser
long qualitySearch(long m, long p)
{
	long result = 0;
	PAlgebra zMStar(m, p);

	// Best
	if (zMStar.numOfGens() == 1 && zMStar.SameOrd(0))
	{
		// cout << "Best" << ", ";
		zMStar.printout();
		result = 1;
	}
	// Somehow ok to use
	else if (zMStar.numOfGens() <= 2)
	{
		// cout << "Decent" << ", ";
		zMStar.printout();
		result = 2;
	}
	return result;
}

int main( int argc, char *argv[])
{
	if(argc != 5)
	{
		std::clog << "Wrong inputs!" << std::endl << "Inputs: m min_prime max_prime max_degree" << std::endl;
		return 1;
	}
	uint64_t m = std::stoull(argv[1]);
	uint64_t min = std::stoull(argv[2]);
	uint64_t max = std::stoull(argv[3]);
	uint64_t max_degree = std::stoull(argv[4]);

	float phiM = 0;
	if(NTL::ProbPrime(m))
		phiM = m - 1;
	else
		phiM = eulerTot(m);

	NTL::ZZ_p::init(NTL::ZZ(m));
	std::cout << "Ring Dimension = " << phiM << std::endl;

	for (uint64_t i = min; i < max; i++)
	{
		if ( NTL::ProbPrime(i) and (m % i != 0) )
		{
			uint64_t counter = 1;
			NTL::ZZ_p x = NTL::ZZ_p(i);
			while (!NTL::IsOne(x)) // why not IsZero, ANS: to check that i generates the group
			{
				x = x * NTL::ZZ_p(i);
				counter++; // order of the prime
				if(counter > max_degree+2) // why? just some exit condition? ANS: stop testing if i can be a generator
					break;
			}
			// why split these 2 conditions?
			if(counter <= max_degree)
				std::cout << phiM << "\t ord(" << i << "): " << counter << "\t Slots: " << phiM / counter << std::endl;
			if( (counter <= i) && (counter <= max_degree) )
			{
				qualitySearch(m,i);
				std::cout << std::endl;
			}
		}
	}
	return 0;
}
