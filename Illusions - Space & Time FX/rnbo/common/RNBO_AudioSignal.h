#ifndef _RNBO_AUDIOSIGNAL_H_
#define _RNBO_AUDIOSIGNAL_H_

// RNBO_AudioSignal.h -- utilities used by generated code

#include "RNBO_Math.h"
#include "RNBO_Types.h"
#include "RNBO_PlatformInterface.h"

namespace RNBO {

	static SampleIndex msToSamps(MillisecondTime ms, number sr);
	static MillisecondTime sampsToMs(SampleIndex samps, number sr);
	static SampleValue* resizeSignal(SampleValue* sig, size_t oldSize, size_t vs);
	static inline void zeroSignal(SampleValue* sig, size_t size);
	static inline void copySignal(SampleValue *dst, SampleValue *src, size_t size);
	static inline bool isNaN(number v);
	static void* allocateArray(size_t count, const char* type);
	static number rand01();
	static float bitwiseFloat(unsigned long n);
	static BinOpInt imul(BinOpInt a, BinOpInt b);
	static number pi01();
	static inline int stringCompare(const char *a, const char *b);
	static SampleValue rms(SampleValue *sig, SampleIndex n);
	static SampleValue peakvalue(SampleValue *sig, SampleIndex n);
	static inline int64_t nextpoweroftwo(int64_t v);

	static inline SampleIndex msToSamps(MillisecondTime ms, number sr)
	{
		return SampleIndex(ms * sr * 0.001);
	}

	static inline MillisecondTime sampsToMs(SampleIndex samps, number sr)
	{
		return samps / (sr * 0.001);
	}

	static Sample *resizeSignal(Sample *sig, size_t oldSize, size_t vs)
	{
		// alloc and clear signals

		Sample *newsig = static_cast<Sample*>(Platform::get()->realloc(sig, vs * sizeof(Sample)));
		for (size_t i = oldSize; i < vs; i++) {
			newsig[i] = 0;
		}

		return newsig;
	}

	static inline void zeroSignal(SampleValue* sig, size_t size)
	{
		Platform::get()->memset(sig, 0, size * sizeof(SampleValue));
	}

	static void fillSignal(SampleValue* sig, size_t size, SampleValue value, size_t offset = 0)
	{
		for (size_t i = offset; i < size; i++) {
			sig[i] = value;
		}
	}

	static inline void copySignal(SampleValue *dst, SampleValue *src, size_t size)
	{
		Platform::get()->memcpy(dst, src, size * sizeof(SampleValue));
	}

	static inline void *allocateArray(size_t count, const char *type)
	{
		RNBO_UNUSED(type);
		return Platform::get()->malloc(count * sizeof(number));
	}

	static inline bool isNaN(number v)
	{
		return RNBO_Math::rnbo_isnan<number>(v) ? true : false;
	}

	#define getArrayValueAtIndex(a, index) a[int(index)]

	static inline number rand01()
	{
		return number(rand()) / RAND_MAX;
	}

	static inline float bitwiseFloat(unsigned long n)
	{
		return (*(reinterpret_cast<float*>(&n)));
	}

	static inline BinOpInt imul(BinOpInt a, BinOpInt b)
	{
		return a * b;
	}

	static inline unsigned long systemticks()
	{
		return static_cast<unsigned long>(clock());
	}

	static inline number pi01() // can't call it pi because that is used in some Apple framework that conflicts
	{
		// M_PI is not standard so just returning the value
		return number(3.14159265358979323846);
	}

	static inline int stringCompare(const char *a, const char *b)
	{
		return Platform::get()->strcmp(a, b);
	}

	static inline SampleValue rms(SampleValue *sig, SampleIndex n)
	{
		SampleValue avg = 0;

		for (SampleIndex i = 0; i < n; i++) {
			avg += sig[i] * sig[i];
		}

		return sqrt(avg / n);
	}

	static inline SampleValue peakvalue(SampleValue *sig, SampleIndex n)
	{
		SampleValue peak = 0;
		SampleValue val;

		for (SampleIndex i = 0; i < n; i++) {
			val = fabs(sig[i]);
			if (val > peak)
				peak = val;
		}

		return peak;
	}

	static inline int64_t nextpoweroftwo(int64_t v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		v++;
		return v;
	}

	static inline void crash() {
		Platform::get()->abort();
	}

#define uint32_add(x, y) (UBinOpInt)x + (UBinOpInt)y
#define uint32_trunc(x)	(UBinOpInt)(x)
#define uint32_rshift(x, y)	((UBinOpInt)x >> (UBinOpInt)y)

} // namespace RNBO

#endif // #ifndef _RNBO_AUDIOSIGNAL_H_
