#include <iostream>
#include <random>
#include "gtest/gtest.h"

#include "helib/helib.h"

#define POINT_AT_INFINITY 1

class HELIB_PARAMS: public ::testing::Test
{
	protected:
	HELIB_PARAMS(){}

	virtual void SetUp(){}
};

TEST_F(HELIB_PARAMS, helib_params_exploration)
{
	// Plaintext prime modulus
	unsigned long p = 17;
	// Cyclotomic polynomial - defines phi(m)
	unsigned long m = 8;
	// Hensel lifting (default = 1)
	unsigned long r = 1;
	// Number of bits of the modulus chain
	unsigned long bits = 500;
	// Number of columns of Key-Switching matrix (default = 2 or 3)
	unsigned long c = 2;

	std::cout << "Initialising context object..." << std::endl;
	// Initialize context
	// This object will hold information about the algebra created from the
	// previously set parameters
	helib::Context context = helib::ContextBuilder<helib::BGV>()
                               .m(m)
                               .p(p)
                               .r(r)
                               .bits(bits)
                               .c(c)
                               .build();

	// Print the context
	context.printout();
	std::cout << std::endl;

	// Print the security level
	std::cout << "Security: " << context.securityLevel() << std::endl;

	// Secret key management
	std::cout << "Creating secret key..." << std::endl;
	// Create a secret key associated with the context
	helib::SecKey secret_key(context);
	// Generate the secret key
	secret_key.GenSecKey();
	std::cout << "Generating key-switching matrices..." << std::endl;
	// Compute key-switching matrices that we need
	helib::addSome1DMatrices(secret_key);

	// Public key management
	// Set the secret key (upcast: SecKey is a subclass of PubKey)
	const helib::PubKey& public_key = secret_key;

	// Get the EncryptedArray of the context
	const helib::EncryptedArray& ea = context.getEA();

	// Get the number of slots (phi(m))
	long nslots = ea.size();
	std::cout << "Number of slots: " << nslots << std::endl;
	std::cout << "HE Initialization completed." << std::endl;

	std::cout << "Height of slot, d: " << context.getPhiM()/nslots << std::endl;
}