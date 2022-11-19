//
//  RNBO_AudioBufferConverter.h
//
//  Created by Rob Sussman on 1/7/16.
//
//

#ifndef _RNBO_AudioBufferConverter_h
#define _RNBO_AudioBufferConverter_h

#include "RNBO_Types.h"
#include "RNBO_Vector.h"

namespace RNBO {

	/**
	 * @private
	 * Container class for audio data to be converted
	 */
	class AudioBufferConverterData {
	public:
		~AudioBufferConverterData() {
			deleteBuffers();
		}

		void setup(size_t numChannels, size_t sampleFrames) {
			if (numChannels != _numChannels || sampleFrames != _sampleFrames) {
				deleteBuffers();
				_conversionBuffer  = new SampleValue*[numChannels];
				for (size_t i = 0; i < numChannels; i++) {
					_conversionBuffer[i] = new SampleValue[sampleFrames];
				}

				_numChannels = numChannels;
				_sampleFrames = sampleFrames;
			}
		}

		SampleValue** getConversionBuffer() {
			return _conversionBuffer;
		}

		size_t getNumChannels() {
			return _numChannels;
		}

		size_t getSampleFrames() {
			return _sampleFrames;
		}

	private:

		void deleteBuffers() {
			if (_conversionBuffer) {
				for (size_t i = 0; i < _numChannels; i++) {
					delete _conversionBuffer[i];
				}
				delete _conversionBuffer;
				_conversionBuffer = nullptr;
			}
		}

		size_t _numChannels = 0;
		size_t _sampleFrames = 0;
		SampleValue** _conversionBuffer = nullptr;
	};

	/**
	 * @private
	 * Convert input audio data to internal audio buffer data.
	 */
	class AudioBufferConverterBase {
	public:
		AudioBufferConverterBase(AudioBufferConverterData* data)
		: _data(data)
		{}

		SampleValue** getInternalBuffers() {
			return _audioBufferInternalPointers;
		}

		template <typename U>
		static void clearAudioBuffers(U audioBuffers, size_t numChannels, size_t sampleFrames, size_t startChannel = 0) {
			for (size_t chan = startChannel; chan < numChannels; chan++) {
				auto pDest = audioBuffers[chan];

				size_t n = sampleFrames;
				while (n--) {
					*pDest++ = 0;
				}
			}
		}

	protected:

		template <typename U, typename V>
		void copyBuffer(U src, V** dst, size_t numChannels, size_t sampleFrames)
		{
			for (size_t i = 0; i < numChannels; i++) {
				for (size_t j = 0; j < sampleFrames; j ++) {
					dst[i][j] = static_cast<V>(src[i][j]);
				}
			}
		}

		template <typename U, typename V>
		void copyFromInterleaved(U src, V dst, size_t numChannels, size_t sampleFrames)
		{
			for (size_t i = 0; i < numChannels; i++) {
				for (size_t j = 0; j < sampleFrames; j ++) {
					dst[i][j] = src[j * numChannels + i];
				}
			}
		}

		template <typename U, typename V>
		void copyToInterleaved(U src, V* dst, size_t numChannels, size_t sampleFrames)
		{
			for (size_t i = 0; i < numChannels; i++) {
				for (size_t j = 0; j < sampleFrames; j ++) {
					dst[j * numChannels + i] = static_cast<V>(src[i][j]);
				}
			}
		}

		SampleValue** 				_audioBufferInternalPointers = nullptr;
		AudioBufferConverterData*	_data;
	};

	/**
	 * @private
	 * only use the specializations
	 */
	template <typename T>
	class AudioInBufferConverter : public AudioBufferConverterBase {
	private:
		AudioInBufferConverter();
	};

	// these converters only make sense when SampleValue != converted value
#ifdef RNBO_USE_FLOAT32
	/**
	 * @private
	 * Convert to double pointers
	 */
	template <>
	class AudioInBufferConverter<double**> : public AudioBufferConverterBase {
	public:
		AudioInBufferConverter(double** audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
			copyBuffer(audioBuffers, _audioBufferInternalPointers, numChannels, sampleFrames);
		}
	};
#else
	/**
	 * @private
	 * Convert to float pointers
	 */
	template <>
	class AudioInBufferConverter<float**> : public AudioBufferConverterBase {
	public:
		AudioInBufferConverter(float** audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
			copyBuffer(audioBuffers, _audioBufferInternalPointers, numChannels, sampleFrames);
		}
	};
#endif // RNBO_USE_FLOAT32

	/**
	 * @private
	 * Convert to floats
	 */
	template <>
	class AudioInBufferConverter<float*> : public AudioBufferConverterBase {
	public:
		AudioInBufferConverter(float* audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
			copyFromInterleaved(audioBuffers, _audioBufferInternalPointers, numChannels, sampleFrames);
		}
	};

	/**
	 * @private
	 * Convert to doubles
	 */
	template <>
	class AudioInBufferConverter<double*> : public AudioBufferConverterBase {
	public:
		AudioInBufferConverter(double* audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
			copyFromInterleaved(audioBuffers, _audioBufferInternalPointers, numChannels, sampleFrames);
		}
	};


	/**
	 * @private
	 * only use the specializations
	 */
	template <typename T>
	class AudioOutBufferConverter : public AudioBufferConverterBase {
	private:
		// only use the specializations
		AudioOutBufferConverter();
	};

	// these converters only make sense when SampleValue != converted value
#ifdef RNBO_USE_FLOAT32
	/**
	 * @private
	 * Convert to double pointers
	 */
	template <>
	class AudioOutBufferConverter<double**> : public AudioBufferConverterBase {
	public:
		AudioOutBufferConverter(double** audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
			_audioBuffers = audioBuffers;
		}

		~AudioOutBufferConverter() {
			copyBuffer(_audioBufferInternalPointers, _audioBuffers, _data->getNumChannels(), _data->getSampleFrames());
		}

	private:
		double** _audioBuffers = nullptr;
	};
#else
	/**
	 * @private
	 * Convert to float pointers
	 */
	template <>
	class AudioOutBufferConverter<float**> : public AudioBufferConverterBase {
	public:
		AudioOutBufferConverter(float** audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
			_audioBuffers = audioBuffers;
		}

		~AudioOutBufferConverter() {
			copyBuffer(_audioBufferInternalPointers, _audioBuffers, _data->getNumChannels(), _data->getSampleFrames());
		}

	private:
		float** _audioBuffers = nullptr;
	};
#endif // RNBO_USE_FLOAT32

	/**
	 * @private
	 * Convert to floats
	 */
	template <>
	class AudioOutBufferConverter<float*> : public AudioBufferConverterBase {
	public:
		AudioOutBufferConverter(float* audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		, _audioBuffers(audioBuffers)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
		}

		~AudioOutBufferConverter() {
			copyToInterleaved(_audioBufferInternalPointers, _audioBuffers, _data->getNumChannels(), _data->getSampleFrames());
		}

	private:
		float* _audioBuffers = nullptr;
	};

	/**
	 * @private
	 * Convert to doubles
	 */
	template <>
	class AudioOutBufferConverter<double*> : public AudioBufferConverterBase {
	public:
		AudioOutBufferConverter(double* audioBuffers, size_t numChannels, size_t sampleFrames, AudioBufferConverterData* data)
		: AudioBufferConverterBase(data)
		, _audioBuffers(audioBuffers)
		{
			_data->setup(numChannels, sampleFrames);
			_audioBufferInternalPointers = _data->getConversionBuffer();
		}

		~AudioOutBufferConverter() {
			copyToInterleaved(_audioBufferInternalPointers, _audioBuffers, _data->getNumChannels(), _data->getSampleFrames());
		}

	private:
		double* _audioBuffers = nullptr;
	};

} // namespace RNBO

#endif // #ifndef _RNBO_AudioBufferConverter_h

