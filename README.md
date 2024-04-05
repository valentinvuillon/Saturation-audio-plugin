How to build the plug-in:  
Install the Juce Projucer  
Set up a default project for an audio plug-in  
Replace the source code from the default project with the source code from this repository  
Build the code, this should produce a VST3 plug-in  


How the plug-in works:  
The sample n of the output signal out[n] is f(in[n]) with in[n] the sample n of the input signal and f a waveshaping function which is a saturatiing function. The function can be visualized here: https://www.desmos.com/calculator/afrb5jz5m8  

The definition of this function is done in DSP_functions.cpp.  

The plug-in has two knobs. The first, "Input gain", multiply the input signal by a constant before applying the waveshaping function. The second, "Dry/Wet", allows to blend the output signal of the plug-in (wet signal) with the untouched input signal (wet signal).  



