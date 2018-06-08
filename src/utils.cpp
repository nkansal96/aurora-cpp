#include "aurora/utils.h"
#include <cmath>
#include <cinttypes>
#include <memory>
#include <string>
#include <algorithm>
#include <portaudio.h>
#include "aurora/AudioFile.h"
#include "aurora/WAV.h"
#include "aurora/errors/AuroraError.h"
#include <cstring>


namespace aurora {
const int16_t SILENT_THRESH = 1 << 10;

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

bool isSilent(Buffer &b) {
  return isSilent(b, 0, b.size());
}
bool isSilent(Buffer &b, size_t beginningIndex, size_t endingIndex) {  
  int start = std::max((size_t)0, beginningIndex)/sizeof(int16_t);
  int end = std::min(b.size(), endingIndex)/sizeof(int16_t);
  
  int16_t *intBuff = reinterpret_cast<int16_t*>(b.data());
  int16_t maxSoFar = 0;
  for (int i = start; i < end; i++) {
    int16_t val = intBuff[i];
    maxSoFar = std::max(val, maxSoFar);
  }
  return maxSoFar < SILENT_THRESH;
}

Buffer record(float length, float silenceLen) {
  //init portudo
  PaError err = Pa_Initialize();
  
  PaStream *stream;
  err = Pa_OpenDefaultStream(&stream,
                             1, // mono input channel                                                
                             0, // no output channels                                                
                             PORTAUDIO_SAMPLE_FORMAT,
                             defaultSampleRate,
                             paFramesPerBufferUnspecified,
                             nullptr, // use blocking read instead of callback                       
                             nullptr); // no userData for blocking read                              
  if (err != paNoError) {
    // clean up portaudio resources before throwing                                                  
    Pa_Terminate();

    throw AuroraError("PortAudioError", "An error occurred while using a PortAudio stream", Pa_GetErrorText(err));
  }
  // commences audio processing of stream                                                            
  err = Pa_StartStream(stream);



  bool lengthCutoffEnabled = true, silenceCutoffEnabled = true;
  if(length == 0.0) {
    lengthCutoffEnabled = false;
  }
  if(silenceLen == 0.0) {
    silenceCutoffEnabled = false;
  }
  int totalSamplesToRecord = length * (float)defaultSampleRate;
  int maxSilentPeriod = silenceLen * (float)defaultSampleRate;
  int silentBufferPeriod= 0.4 * (float)defaultSampleRate; //max of .4 seconds of silence before soun\
d allowed                                                                                            
  const int CHUNK_SIZE = 512; // max number of samples to read at once   
 Buffer outBuffer(totalSamplesToRecord*BYTES_PER_SAMPLE);
 int progress = 0, silentPeriod = 0;

 if(!silenceCutoffEnabled && !lengthCutoffEnabled) { //if nothing cuts off audio length, return
   return outBuffer;
 }
 //while sound hasn't been detected, wait (also record buffer up to silentBufferPeriod)
 while(true) {
   if((progress + CHUNK_SIZE)*BYTES_PER_SAMPLE >= outBuffer.size()) { //if we need more space, add it
     outBuffer.resize(2*outBuffer.size() + (CHUNK_SIZE*BYTES_PER_SAMPLE)); //multiple space by two (and a little more)
   }
   AudioSampleType *buffPtr = reinterpret_cast<AudioSampleType*>(outBuffer.data()) + progress;
   int samplesRead = std::min(CHUNK_SIZE, (int)outBuffer.size() - progress);
   err = Pa_ReadStream(stream, buffPtr, samplesRead);
   
   if (isSilent(outBuffer, 0, ((progress + samplesRead)*BYTES_PER_SAMPLE))) {
     if(progress + samplesRead < silentBufferPeriod) {
       progress += samplesRead;
     }
   } else {
     break;
   }
 }
 
 //record up to length or silenceLen
 while(true) {
   if((progress + CHUNK_SIZE)*BYTES_PER_SAMPLE > outBuffer.size()) { //if we need more space, add it                                                      
     outBuffer.resize(2*outBuffer.size() + (CHUNK_SIZE*BYTES_PER_SAMPLE)); //multiple space by two (and a little more)    
   }
   AudioSampleType *buffPtr = reinterpret_cast<AudioSampleType*>(outBuffer.data()) + progress;
   int samplesRead = std::min(CHUNK_SIZE, (int)outBuffer.size() - progress);
   err = Pa_ReadStream(stream, buffPtr, samplesRead);

   //count continous silence
   if(isSilent(outBuffer, (progress*BYTES_PER_SAMPLE), ((progress+samplesRead)*BYTES_PER_SAMPLE))) {
     silentPeriod+= samplesRead;
   } else {
     silentPeriod = 0;
   }
   
   //if silence is too long
   if(maxSilentPeriod <= silentPeriod && silenceCutoffEnabled) {
     break;
   }
   
   //if recorded length is too long
   if(totalSamplesToRecord <= progress && lengthCutoffEnabled) {
     break;
   }
   
   progress+=samplesRead;
 }
 // terminates audio processing, waiting for all pending audio buffers to complete                  
 err = Pa_StopStream(stream);

 // close audio stream, discarding any pending buffers!                                             
 err = Pa_CloseStream(stream);

 // clean up portaudio resources                                                                    
 Pa_Terminate();

 //TODO remove pop at beginning                                                                         
 outBuffer.resize(progress * BYTES_PER_SAMPLE);
 return outBuffer;
}


// virtual sound file functions
// implementations converted from libsndfile unit test:
// https://github.com/erikd/libsndfile/blob/2fcf531ac940829cf350f86786fcff5160a32143/tests/virtual_io_test.c

static sf_count_t vfget_filelen (void *userData) {
  VirtualSoundFileUserData *vf = reinterpret_cast<VirtualSoundFileUserData*>(userData);
	return vf->buffer.size() ;
}

static sf_count_t vfseek (sf_count_t offset, int whence, void *userData) {
  VirtualSoundFileUserData *vf = reinterpret_cast<VirtualSoundFileUserData*>(userData);

	switch (whence) {
  case SEEK_SET:
    vf->offset = offset ;
    break;
  case SEEK_CUR:
    vf->offset = vf->offset + offset ;
    break;
  case SEEK_END:
    vf->offset = vf->buffer.size() + offset ;
		break;
  }

	return vf->offset ;
}

static sf_count_t vfread (void *ptr, sf_count_t count, void *userData) {
  char *dst = reinterpret_cast<char*>(ptr);
  VirtualSoundFileUserData *vf = reinterpret_cast<VirtualSoundFileUserData*>(userData);

  if (vf->offset + count > vf->buffer.size()) {
    count = vf->buffer.size() - vf->offset;
  }

  std::memcpy(dst, vf->buffer.data() + vf->offset, count);

  vf->offset += count;

  return count;
}

static sf_count_t vfwrite (const void *ptr, sf_count_t count, void *userData) {
  const char *src = reinterpret_cast<const char*>(ptr);
  VirtualSoundFileUserData *vf = reinterpret_cast<VirtualSoundFileUserData*>(userData);

  if (vf->offset + count > vf->buffer.size()) {
    vf->buffer.resize(vf->offset + count);
  }

  std::memcpy(vf->buffer.data() + vf->offset, src, count);

  vf->offset += count;

	return count ;
}

static sf_count_t vftell (void *userData) {
  VirtualSoundFileUserData *vf = reinterpret_cast<VirtualSoundFileUserData*>(userData);
	return vf->offset ;
}

// initialize VirtualSoundFile with virtual file function pointers
SF_VIRTUAL_IO VirtualSoundFile = {vfget_filelen, vfseek, vfread, vfwrite, vftell};

} // namespace aurora
