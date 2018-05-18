#include <vector>

#ifndef UTILS_H
#define UTILS_H

using Buffer = std::vector<char>;


//converts two little-endian bytes to a system two-byte value
uint16_t littleendian_to_uint(char first, char second);

//writes uint of arbitrary size to littleendian char array.
//function definition needs to be in header because it's a templated function
template<typename T> void write_uint_to_littleendian(T number, char* bytes){
	for(int i=0; i< sizeof(number); i++){
		bytes[i] = (number >> (8 * i)) & 0xff; 
	}
}

//void write_uint_to_littleendian(unsigned number, char* bytes);

// rms calculates the root-mean-square of a sequence of audio data. For
// now, it assumes that the data is in 16-bit mono samples. Thus, the passed
// value for `sampleSize` MUST be 2. This will change once we figure out
// how to read a variable size of data during runtime.
//translated from https://github.com/auroraapi/aurora-go/blob/c1aa007a72c1eb02b3cb2b85dfe164162157d32b/audio/utils.go
double rms(int sampleSize, Buffer audioData);

#endif 