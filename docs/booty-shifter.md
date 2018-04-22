## Table of contents:
[Growler](#growler)

[Booty Shifter](#shifter)

[Attenuverters](#atten)

[CV ranges](#cv)
# Growler
![vocal animator image](./growler.jpg) 

**Growler** is a recreation of the Vocal Animator circuit invented by Bernie Hutchins, and published in Electronotes magazine in the late 70's. It continuously morphs between different vaguely "voice like" tones.

**To get a good sound:** run any harmonically rich signal into the input, and something good will come out. Low frequency pulse waves and distorted sounds make great input.

The controls do pretty much what you would expect:
* **LFO** controls the speed of the modulation LFOs.
* **Fc** controls the average frequency of the multiple filters.
* **Q** controls the sharpness of the filters.
* **Depth** controls how much of the modulation LFOs are applied to the filters.

## How Growler works
Each **CV input stage** is the same: a knob that supplies a fixed  offset and a CV input that is processed by an attenuverter. The processed CV is added to the knob voltage. See below for more on [Attenuverters](#atten) and [CV ranges](#cv).

There are four **bandpass filters**, roughly tuned to some typical vocal formant frequencies: 522, 1340, 2570, and 3700 Hz. The filters are run in parallel, with their outputs summed together.

The first three filters are modulated by an LFO comprised of **4 triangle wave LFOs** running at different frequencies. They are summed together in various combinations to drive each of the filters.

The **LFO** Rate control shifts the speed of all 4 LFOs while maintaining the ratio of their frequencies.

The **Fc** control moves the frequencies of the first three filters, but not by equal amounts. The lowest filter moves at 1V/Oct, but the middle two move less. The top filter is fixed at 3700 Hz.

The **Q** control does just what it says - controls the Q (resonance) of the filters.

The **Modulation Depth** controls how much of the summed LFOs get to each filter. Again, the lower filters move farther, and the top filter is fixed.

The smaller knobs next to the main knobs are **attenuverters**, which scale control voltages. For more on attenuverters, [see below](#atten)

There are three LFO outputs next to the blinking LFOs. These may be used to modulate other modules, or as semi-random voltage sources.

**Bass boost** switch. When it’s in the up position (on) there should be more bass. This is done by switching some or all of the filters from bandpass to lowpass.

LFO **Matrix** switch. This is the unlabeled switch in the LFO section. When it’s down (default position) the LFOs are closely correlated. In the middle we try to make them a little bit more independent. When it’s in the up position the LFOs will often go in different directions.

# Booty Shifter frequency shifter <a name="shifter"></a>
**Booty Shifter** is a frequency shifter inspired by the Moog/Bode frequency shifter module.

![boooty shifter image](./booty-shifter.png)

The name "Booty Shifter" is a nod to the classic analog module, as well as to a black cat named Booty.

Booty Shifter  will take an audio input and shift the frequencies up or down. This is not like a pitch shift where harmonics will remain in tune; it is an absolute frequency shift in Hz, so in general **harmonics will go way out of tune.**
## Getting good sounds from Booty Shifter
Feed in music and shift the frequency a good amount.

Feed in **speech or radio** and shift it. 

Feed the CV from a **sequencer** to sequence the mayhem.

Shift **drums** up or down a little bit to re-tune them without the usual pitch-shifting artifacts.

Small shifts in conjunction with delays can make a chorus-like effect to thicken music.
## Inputs and outputs
* **IN** is the audio input.
* **CV** is the pitch shift control voltage. -5V will give minimum shift, +5 will give maximum.
* **DN** is the down-shifted output.
* **UP** is the up-shifted output.

## Controls
**RANGE** sets the total shift range in Hz. For example, the 50 hz. Setting means that the minimum shift is 50 Hz down, and the maximum is 50 hz up.

Range value **Exp is different**, here minimum shift is 2 hz, maximum is 2 kHz, with an exponential response. As of version 0.6.2 the response is an accurate 1 Volt per Octave.

Shift **AMT** is added to the control voltage, with a range or -5..5.
## Oddities and limitations
If you shift the frequency up too far, it will alias. There is no anti-aliasing, so if the highest input frequency + shift amount > sample_rate / 2, you will get aliasing. The Bode original of course did not alias.

If you shift the input down a lot, frequencies will go **below zero and wrap around**. Taken far enough this will completely **reverse the spectrum** of the input. This was a prized feature of the Bode original.

As you shift the input down, you may start to generate a lot of subsonic energy. A **High Pass filter** may clean this up.

The down shift **frequency fold-over**, while true to the original, does cause problems when trying to pitch drum tracks down a lot. High pass filtering the input before it is down-shifted can control this.

## About Attenuverters <a name="atten"></a>

The small knobs next to the bigger knobs are **attenuverters**.  They scale and/or invert the control voltage inputs next to them. When they are turned all the way up the full CV comes through. As they are turned down less CV comes through. Straight up none passes. As they are turned down further the CV comes back, but inverted.

## Control voltage ranges <a name="cv"></a>

Our modules usually expect a control voltage range of **-5 to +5**. The associated offset knobs will also add -5 to +5. After attenuverters are applied to CV the knob value is added. After all that, the result is usually clipped to the -5 to +5 range.


