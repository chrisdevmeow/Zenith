#include "EffectFactory.h"
#include "Effects/Dynamics/Dynamics.h"
#include "Effects/Dynamics/DynamicsExtra.h"
#include "Effects/EQ/EQ.h"
#include "Effects/EQ/EQExtra.h"
#include "Effects/Delay/Delay.h"
#include "Effects/Delay/DelayExtra.h"
#include "Effects/Reverb/Reverb.h"
#include "Effects/Reverb/ReverbExtra.h"
#include "Effects/Modulation/Modulation.h"
#include "Effects/Modulation/ModulationExtra.h"
#include "Effects/Distortion/Distortion.h"
#include "Effects/Distortion/DistortionExtra.h"
#include "Effects/Spatial/Spatial.h"
#include "Effects/Spatial/SpatialExtra.h"
#include "Effects/Creative/Creative.h"
#include "Effects/Creative/CreativeExtra.h"
#include "Effects/MIDI/MIDI.h"
#include "Effects/MIDI/MIDIExtra.h"
#include "Effects/Utility/Utility.h"
#include "Effects/Utility/UtilityExtra.h"

std::unique_ptr<Effect> EffectFactory::create (const juce::String& name)
{
    // ===== DYNAMICS =====
    if (name == "Compressor") return std::make_unique<Compressor>();
    if (name == "Limiter") return std::make_unique<Limiter>();
    if (name == "Expander") return std::make_unique<Expander>();
    if (name == "Gate") return std::make_unique<Gate>();
    if (name == "DeEsser") return std::make_unique<DeEsser>();
    if (name == "TransientShaper") return std::make_unique<TransientShaper>();
    if (name == "MultibandCompressor") return std::make_unique<MultibandCompressor>();
    if (name == "MultibandExpander") return std::make_unique<MultibandExpander>();
    if (name == "PeakCompressor") return std::make_unique<PeakCompressor>();
    if (name == "RMSCompressor") return std::make_unique<RMSCompressor>();
    if (name == "OpticalCompressor") return std::make_unique<OpticalCompressor>();
    if (name == "FETCompressor") return std::make_unique<FETCompressor>();
    if (name == "VCACompressor") return std::make_unique<VCACompressor>();
    if (name == "VariMuCompressor") return std::make_unique<VariMuCompressor>();
    if (name == "DiodeCompressor") return std::make_unique<DiodeCompressor>();
    if (name == "FeedbackCompressor") return std::make_unique<FeedbackCompressor>();
    if (name == "FeedForwardCompressor") return std::make_unique<FeedForwardCompressor>();
    if (name == "EnvelopeFollower") return std::make_unique<EnvelopeFollower>();

    // ===== EQ =====
    if (name == "ParametricEQ") return std::make_unique<ParametricEQ>();
    if (name == "GraphicEQ") return std::make_unique<GraphicEQ>();
    if (name == "DynamicEQ") return std::make_unique<DynamicEQ>();
    if (name == "HighPassFilter") return std::make_unique<HighPassFilter>();
    if (name == "LowPassFilter") return std::make_unique<LowPassFilter>();
    if (name == "BandPassFilter") return std::make_unique<BandPassFilter>();
    if (name == "BandStopFilter") return std::make_unique<BandStopFilter>();
    if (name == "HighShelfFilter") return std::make_unique<HighShelfFilter>();
    if (name == "LowShelfFilter") return std::make_unique<LowShelfFilter>();
    if (name == "NotchFilter") return std::make_unique<NotchFilter>();
    if (name == "AllpassFilter") return std::make_unique<AllpassFilter>();

    // ===== DELAY =====
    if (name == "DigitalDelay") return std::make_unique<DigitalDelay>();
    if (name == "AnalogDelay") return std::make_unique<AnalogDelay>();
    if (name == "TapeDelay") return std::make_unique<TapeDelay>();
    if (name == "BBBDelay") return std::make_unique<BBBDelay>();
    if (name == "PingPongDelay") return std::make_unique<PingPongDelay>();
    if (name == "SlapbackDelay") return std::make_unique<SlapbackDelay>();
    if (name == "ReverseDelay") return std::make_unique<ReverseDelay>();
    if (name == "DuckingDelay") return std::make_unique<DuckingDelay>();
    if (name == "ModulatedDelay") return std::make_unique<ModulatedDelay>();
    if (name == "GoldenDelay") return std::make_unique<GoldenDelay>();
    if (name == "OilCanDelay") return std::make_unique<OilCanDelay>();
    if (name == "TubeDelay") return std::make_unique<TubeDelay>();
    if (name == "MultiTapDelay_4Tap") return std::make_unique<MultiTapDelay_4Tap>();
    if (name == "MultiTapDelay_8Tap") return std::make_unique<MultiTapDelay_8Tap>();
    if (name == "MultiTapDelay_16Tap") return std::make_unique<MultiTapDelay_16Tap>();
    if (name == "ChorusDelay") return std::make_unique<ChorusDelay>();
    if (name == "FlangerDelay") return std::make_unique<FlangerDelay>();
    if (name == "PitchDelay") return std::make_unique<PitchDelay>();
    if (name == "ShimmerDelay") return std::make_unique<ShimmerDelay>();
    if (name == "GrainDelay") return std::make_unique<GrainDelay>();

    // ===== REVERB =====
    if (name == "HallReverb") return std::make_unique<HallReverb>();
    if (name == "RoomReverb") return std::make_unique<RoomReverb>();
    if (name == "ChamberReverb") return std::make_unique<ChamberReverb>();
    if (name == "PlateReverb") return std::make_unique<PlateReverb>();
    if (name == "SpringReverb") return std::make_unique<SpringReverb>();
    if (name == "CathedralReverb") return std::make_unique<CathedralReverb>();
    if (name == "ArenaReverb") return std::make_unique<ArenaReverb>();
    if (name == "AmbienceReverb") return std::make_unique<AmbienceReverb>();
    if (name == "ShimmerReverb") return std::make_unique<ShimmerReverb>();
    if (name == "ReverseReverb") return std::make_unique<ReverseReverb>();
    if (name == "GatedReverb") return std::make_unique<GatedReverb>();
    if (name == "NonLinearReverb") return std::make_unique<NonLinearReverb>();
    if (name == "ConvolutionReverb") return std::make_unique<ConvolutionReverb>();
    if (name == "GoldPlateReverb") return std::make_unique<GoldPlateReverb>();
    if (name == "EMTPlateReverb") return std::make_unique<EMTPlateReverb>();
    if (name == "LexiconHall") return std::make_unique<LexiconHall>();
    if (name == "LexiconRoom") return std::make_unique<LexiconRoom>();
    if (name == "LexiconPlate") return std::make_unique<LexiconPlate>();
    if (name == "LargeHall") return std::make_unique<LargeHall>();
    if (name == "SmallRoom") return std::make_unique<SmallRoom>();
    if (name == "MediumRoom") return std::make_unique<MediumRoom>();
    if (name == "DeepSpace") return std::make_unique<DeepSpace>();
    if (name == "GlimmerReverb") return std::make_unique<GlimmerReverb>();
    if (name == "EchoChamber") return std::make_unique<EchoChamber>();
    if (name == "StudioReverb") return std::make_unique<StudioReverb>();

    // ===== MODULATION =====
    if (name == "Chorus") return std::make_unique<Chorus>();
    if (name == "Flanger") return std::make_unique<Flanger>();
    if (name == "Phaser") return std::make_unique<Phaser>();
    if (name == "Tremolo") return std::make_unique<Tremolo>();
    if (name == "Vibrato") return std::make_unique<Vibrato>();
    if (name == "AutoPan") return std::make_unique<AutoPan>();
    if (name == "RotarySpeaker") return std::make_unique<RotarySpeaker>();
    if (name == "LeslieSimulator") return std::make_unique<LeslieSimulator>();
    if (name == "UniVibe") return std::make_unique<UniVibe>();
    if (name == "RingModulator") return std::make_unique<RingModulator>();
    if (name == "FrequencyShifter") return std::make_unique<FrequencyShifter>();
    if (name == "PitchShifter") return std::make_unique<PitchShifter>();
    if (name == "Harmonizer") return std::make_unique<Harmonizer>();
    if (name == "Choir_4Voice") return std::make_unique<Choir_4Voice>();
    if (name == "Choir_8Voice") return std::make_unique<Choir_8Voice>();
    if (name == "FormantFilter") return std::make_unique<FormantFilter>();
    if (name == "Vocoder") return std::make_unique<Vocoder>();
    if (name == "EnvelopeFilter") return std::make_unique<EnvelopeFilter>();
    if (name == "WahWah") return std::make_unique<WahWah>();
    if (name == "AutoWah") return std::make_unique<AutoWah>();
    if (name == "TouchWah") return std::make_unique<TouchWah>();
    if (name == "Spatializer") return std::make_unique<Spatializer>();

    // ===== DISTORTION =====
    if (name == "Overdrive") return std::make_unique<Overdrive>();
    if (name == "Distortion") return std::make_unique<Distortion>();
    if (name == "Fuzz") return std::make_unique<Fuzz>();
    if (name == "Bitcrusher") return std::make_unique<Bitcrusher>();
    if (name == "SampleRateReducer") return std::make_unique<SampleRateReducer>();
    if (name == "Waveshaper") return std::make_unique<Waveshaper>();
    if (name == "Foldback") return std::make_unique<Foldback>();
    if (name == "Rectifier") return std::make_unique<Rectifier>();
    if (name == "Clipper") return std::make_unique<Clipper>();
    if (name == "SoftClipper") return std::make_unique<SoftClipper>();
    if (name == "HardClipper") return std::make_unique<HardClipper>();
    if (name == "TubeSaturation") return std::make_unique<TubeSaturation>();
    if (name == "TapeSaturation") return std::make_unique<TapeSaturation>();
    if (name == "TransformerSaturation") return std::make_unique<TransformerSaturation>();
    if (name == "ConsoleSaturation") return std::make_unique<ConsoleSaturation>();
    if (name == "PreampSaturation") return std::make_unique<PreampSaturation>();
    if (name == "DiodeClipper") return std::make_unique<DiodeClipper>();
    if (name == "AsymmetricClipper") return std::make_unique<AsymmetricClipper>();
    if (name == "MultibandDistortion") return std::make_unique<MultibandDistortion>();

    // ===== SPATIAL =====
    if (name == "StereoWidener") return std::make_unique<StereoWidener>();
    if (name == "StereoImager") return std::make_unique<StereoImager>();
    if (name == "MSDecoder") return std::make_unique<MSDecoder>();
    if (name == "MSEncoder") return std::make_unique<MSEncoder>();
    if (name == "HaasEffect") return std::make_unique<HaasEffect>();
    if (name == "StereoDelay") return std::make_unique<StereoDelay>();
    if (name == "StereoChorus") return std::make_unique<StereoChorus>();
    if (name == "StereoFlanger") return std::make_unique<StereoFlanger>();
    if (name == "StereoPhaser") return std::make_unique<StereoPhaser>();
    if (name == "StereoSpread") return std::make_unique<StereoSpread>();
    if (name == "CrossFeed") return std::make_unique<CrossFeed>();
    if (name == "BalanceControl") return std::make_unique<BalanceControl>();
    if (name == "WidthControl") return std::make_unique<WidthControl>();

    // ===== CREATIVE =====
    if (name == "GranularProcessor") return std::make_unique<GranularProcessor>();
    if (name == "GlitchMachine") return std::make_unique<GlitchMachine>();
    if (name == "Stutter") return std::make_unique<Stutter>();
    if (name == "Looper") return std::make_unique<Looper>();
    if (name == "ReverseLooper") return std::make_unique<ReverseLooper>();
    if (name == "BitMangler") return std::make_unique<BitMangler>();
    if (name == "SpectralBlur") return std::make_unique<SpectralBlur>();
    if (name == "SpectralFreeze") return std::make_unique<SpectralFreeze>();
    if (name == "HarmonicGenerator") return std::make_unique<HarmonicGenerator>();
    if (name == "SubharmonicGenerator") return std::make_unique<SubharmonicGenerator>();
    if (name == "Octaver") return std::make_unique<Octaver>();
    if (name == "Resonator") return std::make_unique<Resonator>();
    if (name == "FMProcessor") return std::make_unique<FMProcessor>();
    if (name == "AMProcessor") return std::make_unique<AMProcessor>();
    if (name == "CrossSynthesis") return std::make_unique<CrossSynthesis>();
    if (name == "FFTConvolution") return std::make_unique<FFTConvolution>();
    if (name == "PhaseScrambler") return std::make_unique<PhaseScrambler>();

    // ===== MIDI =====
    if (name == "ChordGenerator") return std::make_unique<ChordGenerator>();
    if (name == "Arpeggiator") return std::make_unique<Arpeggiator>();
    if (name == "StepSequencer") return std::make_unique<StepSequencer>();
    if (name == "ScaleFilter") return std::make_unique<ScaleFilter>();
    if (name == "NoteMangler") return std::make_unique<NoteMangler>();

    // ===== UTILITY =====
    if (name == "Gain") return std::make_unique<Gain>();
    if (name == "Trim") return std::make_unique<Trim>();
    if (name == "Pan") return std::make_unique<Pan>();
    if (name == "PhaseInvert") return std::make_unique<PhaseInvert>();
    if (name == "MonoMaker") return std::make_unique<MonoMaker>();
    if (name == "StereoToMono") return std::make_unique<StereoToMono>();
    if (name == "ChannelRouter") return std::make_unique<ChannelRouter>();
    if (name == "PeakMeter") return std::make_unique<PeakMeter>();
    if (name == "RMSMeter") return std::make_unique<RMSMeter>();
    if (name == "LoudnessMeter") return std::make_unique<LoudnessMeter>();
    if (name == "SpectrumAnalyzer") return std::make_unique<SpectrumAnalyzer>();
    if (name == "Oscilloscope") return std::make_unique<Oscilloscope>();
    if (name == "Tuner") return std::make_unique<Tuner>();
    if (name == "TestToneGenerator") return std::make_unique<TestToneGenerator>();
    if (name == "NoiseGenerator") return std::make_unique<NoiseGenerator>();

    return nullptr;
}

juce::StringArray EffectFactory::getAvailableEffects()
{
    return {
        // Dynamics
        "Compressor", "Limiter", "Expander", "Gate", "DeEsser", "TransientShaper",
        "MultibandCompressor", "MultibandExpander", "PeakCompressor", "RMSCompressor",
        "OpticalCompressor", "FETCompressor", "VCACompressor", "VariMuCompressor",
        "DiodeCompressor", "FeedbackCompressor", "FeedForwardCompressor", "EnvelopeFollower",
        // EQ
        "ParametricEQ", "GraphicEQ", "DynamicEQ", "HighPassFilter", "LowPassFilter",
        "BandPassFilter", "BandStopFilter", "HighShelfFilter", "LowShelfFilter",
        "NotchFilter", "AllpassFilter",
        // Delay
        "DigitalDelay", "AnalogDelay", "TapeDelay", "BBBDelay", "PingPongDelay",
        "SlapbackDelay", "ReverseDelay", "DuckingDelay", "ModulatedDelay",
        "GoldenDelay", "OilCanDelay", "TubeDelay", "MultiTapDelay_4Tap",
        "MultiTapDelay_8Tap", "MultiTapDelay_16Tap", "ChorusDelay", "FlangerDelay",
        "PitchDelay", "ShimmerDelay", "GrainDelay",
        // Reverb
        "HallReverb", "RoomReverb", "ChamberReverb", "PlateReverb", "SpringReverb",
        "CathedralReverb", "ArenaReverb", "AmbienceReverb", "ShimmerReverb",
        "ReverseReverb", "GatedReverb", "NonLinearReverb", "ConvolutionReverb",
        "GoldPlateReverb", "EMTPlateReverb", "LexiconHall", "LexiconRoom",
        "LexiconPlate", "LargeHall", "SmallRoom", "MediumRoom", "DeepSpace",
        "GlimmerReverb", "EchoChamber", "StudioReverb",
        // Modulation
        "Chorus", "Flanger", "Phaser", "Tremolo", "Vibrato",
        "AutoPan", "RotarySpeaker", "LeslieSimulator", "UniVibe",
        "RingModulator", "FrequencyShifter", "PitchShifter", "Harmonizer",
        "Choir_4Voice", "Choir_8Voice", "FormantFilter", "Vocoder",
        "EnvelopeFilter", "WahWah", "AutoWah", "TouchWah", "Spatializer",
        // Distortion
        "Overdrive", "Distortion", "Fuzz", "Bitcrusher", "SampleRateReducer",
        "Waveshaper", "Foldback", "Rectifier", "Clipper", "SoftClipper",
        "HardClipper", "TubeSaturation", "TapeSaturation", "TransformerSaturation",
        "ConsoleSaturation", "PreampSaturation", "DiodeClipper", "AsymmetricClipper",
        "MultibandDistortion",
        // Spatial
        "StereoWidener", "StereoImager", "MSDecoder", "MSEncoder",
        "HaasEffect", "StereoDelay", "StereoChorus", "StereoFlanger",
        "StereoPhaser", "StereoSpread", "CrossFeed", "BalanceControl", "WidthControl",
        // Creative
        "GranularProcessor", "GlitchMachine", "Stutter", "Looper",
        "ReverseLooper", "BitMangler", "SpectralBlur", "SpectralFreeze",
        "HarmonicGenerator", "SubharmonicGenerator", "Octaver",
        "Resonator", "FMProcessor", "AMProcessor", "CrossSynthesis",
        "FFTConvolution", "PhaseScrambler",
        // MIDI
        "ChordGenerator", "Arpeggiator", "StepSequencer", "ScaleFilter", "NoteMangler",
        // Utility
        "Gain", "Trim", "Pan", "PhaseInvert", "MonoMaker", "StereoToMono",
        "ChannelRouter", "PeakMeter", "RMSMeter", "LoudnessMeter",
        "SpectrumAnalyzer", "Oscilloscope", "Tuner", "TestToneGenerator",
        "NoiseGenerator"
    };
}
