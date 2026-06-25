# Faust2Godot

`faust2godot` is a CLI tool that converts a [Faust](https://faust.grame.fr) written DSP script into a ready-to-use Godot project.

### Installation
`faust2godot` comes pre-installed with [Faust](https://faust.grame.fr) programming language.
Just follow [Faust](https://faust.grame.fr) installation guide then navigate to `tools/faust2appls`.

### Getting started

```
Usage: faust2godot [options] [Faust options] <file.dsp>
Target platform: Linux, MacOSX, Windows
Require: Godot
Generates a ready-to-use Godot engine project.
Options:
   -as-bus-effect : force dsp to be compiled as Godot effect (even though it has no inputs)
   -nvoices <num> : produces a polyphonic DSP with <num> voices, ready to be used with MIDI events
   -effect <effect.dsp> : generates a polyphonic DSP connected to a global output effect, ready to be used with MIDI or OSC
   Faust options : any option (e.g. -vec -vs 8...). See the Faust compiler documentation.
See architecture/godot/readme.md for more info.
```

### Example

```shell
faust2godot myfaustdsp.dsp
```
The output will be a directory with the **same** name as the inputted dsp file.

### Usage

Take a look at the [user documentation](documentation/user-documentation.md) for an in depth guide on how to use the dsp inside Godot.
