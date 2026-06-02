declare filename "untitled.dsp";
declare name "untitled";
declare name 		"osc";
declare version 	"1.0";
declare author 		"Grame";
declare license 	"BSD";
declare copyright 	"(c)GRAME 2009";

//-----------------------------------------------
// 			Sinusoidal Oscillator
//-----------------------------------------------

import("stdfaust.lib");

process = os.osc(440) * 0.1 <: dm.zita_light;

