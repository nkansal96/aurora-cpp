#include "aurora/utils.h"
#include <cmath>
#include <cinttypes>

namespace aurora {

uint16_t littleendian_to_uint(char first, char second){
	uint16_t res = uint16_t(first) | (uint16_t(second) << 8);
	return res;
}

// rms calculates the root-mean-square of a sequence of audio data. For
// now, it assumes that the data is in 16-bit mono samples. Thus, the passed
// value for `sampleSize` MUST be 2. This will change once we figure out
// how to read a variable size of data during runtime.

//translated from https://github.com/auroraapi/aurora-go/blob/c1aa007a72c1eb02b3cb2b85dfe164162157d32b/audio/utils.go

double rms(int sampleSize, Buffer &audioData){
	double sum = 0.0;
	for (int i = 0; i < audioData.size()-1; i += sampleSize) {
		// had to hard code to Uint16 or else it tries to 8 bytes for Uint64
		// note that sampleSize MUST be 2 for this to work
		uint16_t val = littleendian_to_uint(audioData[i], audioData[i+1]);
		sum += double(val * val);
	}

	return std::sqrt(sum / (double(audioData.size() / sampleSize)));
}

} // namespace aurora
