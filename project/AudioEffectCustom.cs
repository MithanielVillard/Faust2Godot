using Godot;
using System;

[GlobalClass][Tool]
public partial class AudioEffectCustom : AudioEffect
{
	[Export]
	public float testValue = 3.0f;
	
	 public AudioEffectCustom() : this(5.0f) {}
	 public AudioEffectCustom(float testValue)
	 {
	 	this.testValue = testValue;
	 }
}
