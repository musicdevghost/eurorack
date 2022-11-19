/*
 ==============================================================================

 RNBO_JuceAudioProcessorEditor.h
 Created: 21 Sep 2015 11:50:17am
 Author:  stb

 ==============================================================================
 */

#ifndef RNBO_JUCEAUDIOPROCESSOREDITOR_H_INCLUDED
#define RNBO_JUCEAUDIOPROCESSOREDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "RNBO.h"

namespace RNBO {

	//==============================================================================
	/**

		this is mostly a copy of a Juce GenericAudioProcessorEditor, with the important
		difference that it can be refreshed via a ParameterInterfacHandle

	 */
	class RNBOAudioProcessorEditor : public AudioProcessorEditor, public AsyncUpdater, public RNBO::EventHandler
	{
	public:
		//==============================================================================
		RNBOAudioProcessorEditor (AudioProcessor* owner, CoreObject& rnboObject);
		~RNBOAudioProcessorEditor() override;

		//==============================================================================
		void paint (Graphics&) override;
		void resized() override;

		void handleAsyncUpdate() override;

		void eventsAvailable() override;
		void handleParameterEvent(const RNBO::ParameterEvent& event) override;
		void handlePresetEvent(const RNBO::PresetEvent& event) override;
		
	private:

		void updateAllParams();

		//==============================================================================
		PropertyPanel							_panel;
		CoreObject&								_rnboObject;
		ParameterEventInterfaceUniquePtr		_parameterInterface;

		Array <PropertyComponent*>	_params;
        Array <PropertyComponent*>  _presetChooserComponents;
	};

} // namespace RNBO

#endif  // RNBO_JUCEAUDIOPROCESSOREDITOR_H_INCLUDED
