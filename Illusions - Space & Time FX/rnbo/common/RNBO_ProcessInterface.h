#ifndef _RNBO_PROCESSINTERFACE_H_
#define _RNBO_PROCESSINTERFACE_H_

#include "RNBO_Types.h"
#include "RNBO_BaseInterface.h"

namespace RNBO {

	/**
	 * @brief An interface for signal-rate data processing
	 */
	class ProcessInterface : public BaseInterface {

	protected:

		~ProcessInterface() {}

	public:

		virtual void prepareToProcess(number sampleRate, Index maxBlockSize, bool force = false) = 0;

		virtual void process(SampleValue** audioInputs, Index numInputs,
							 SampleValue** audioOutputs, Index numOutputs,
							 Index sampleFrames) = 0;

	};

} // namespace RNBO

#endif // #ifndef _RNBO_PROCESSINTERFACE_H_
