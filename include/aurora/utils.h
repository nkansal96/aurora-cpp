#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <sndfile.h>

namespace aurora {

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

//writes uint of arbitrary size to littleendian char array.
//function definition needs to be in header because it's a templated function
template<typename T> void read_uint_from_littleendian(T &number, char* bytes){
	number = 0; 
	for(int i=0; i< sizeof(number); i++){
		number = number | T(bytes[i]) << (8 * (sizeof(number) - i - 1));
	}
}


//void write_uint_to_littleendian(unsigned number, char* bytes);

// rms calculates the root-mean-square of a sequence of audio data. For
// now, it assumes that the data is in 16-bit mono samples. Thus, the passed
// value for `sampleSize` MUST be 2. This will change once we figure out
// how to read a variable size of data during runtime.
//translated from https://github.com/auroraapi/aurora-go/blob/c1aa007a72c1eb02b3cb2b85dfe164162157d32b/audio/utils.go
double rms(int sampleSize, Buffer &audioData);

// virtual file handlers
// http://www.mega-nerd.com/libsndfile/api.html#open_virtual
/// used for *user_data field of all virtual sound file calls
struct VirtualSoundFileUserData {
  sf_count_t offset;
  Buffer buffer;

  VirtualSoundFileUserData() : offset(0) {}
  VirtualSoundFileUserData(Buffer &b) : offset(0), buffer(b) {}
};

/**
 * Record records audio from the default input device
 * @param length the duration of audio to record, in seconds
 * @param silenceLen if length is 0, the amount of silence in seconds to allow before
 * ending recording
 * @returns a Buffer containing the recorded raw audio data
 */
Buffer record(float length, float silenceLen);

/// struct of function pointers that implement a virtual sound file
extern SF_VIRTUAL_IO VirtualSoundFile;

}

#endif // namespace aurora
