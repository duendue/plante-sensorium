#use_real_time
#this is set up for close to live music output. The "sleep 1" in the bottumn creates a small delay, so every
#input recieved plays at the same time. This makes it possible for the pi to not play
# a note if it is feeling overwhelmed by data. :)
use_real_time

#the following musical scales are paired. The "major scale" and natural minor scale sounds good together.
#major_scale = [40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73] #major_scale
#natural_minor_scale = [40, 42, 43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 60, 62, 64, 66, 67, 69, 71, 72] #natural_minor_scale

#dorian_mode = [40, 42, 43, 45, 47, 49, 50, 52, 54, 55, 57, 59, 61, 62, 64, 66, 67, 69, 71, 73] #dorian_mode
#mixolydian_mode = [40, 42, 44, 45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73] #mixolydian_mode

#phrygian_mode = [43, 45, 47, 48, 50, 52, 53, 55, 56, 58, 60, 62, 64, 65, 67, 68, 71, 72, 74, 76] #phrygian_mode
#locrian_mode = [43, 44, 46, 48, 49, 51, 52, 55, 56, 58, 60, 61, 63, 64, 67, 68, 70, 72, 73, 75] #locrian_mode

#pentatonic_major_scale = [40, 42, 44, 47, 49, 52, 54, 56, 59, 61, 64, 66, 68, 71, 73, 76, 78, 80, 83, 85] #pentatonic_major_scale
#pentatonic_minor_scale = [40, 43, 45, 47, 50, 52, 55, 57, 59, 62, 64, 67, 69, 71, 74, 76, 79, 81, 83, 86] #pentatonic_minor_scale





live_loop :osc do
  
  cc, val = sync "/midi:w-fader_0:1/control_change" #
  osc_in = (val / 127.0) * 90.0 + 40.0
  
  
  if cc == 3
    
    with_fx :echo,decay: 0.1 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73] #major_scale
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Ascale = output_value
      
      use_synth :pretty_bell
      play Ascale, amp: 0.4, attack: 0.125, sustain: 0.5, release: 0.5
      wait 0.1
    end
    
  end
  
  if cc == 4
    
    with_fx :echo , phase: 0.125, decay: 2 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 60, 62, 64, 66, 67, 69, 71, 72] #natural_minor_scale
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Bscale = output_value
      
      use_synth :hollow
      play Bscale, amp: 0.8
      wait 0.1
    end
    
  end
  
  
  
  if cc == 5
    
    with_fx :normaliser, level: 0.3 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 43, 45, 47, 49, 50, 52, 54, 55, 57, 59, 61, 62, 64, 66, 67, 69, 71, 73] #dorian_mode
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Cscale = output_value
      
      use_synth :tb303
      play Cscale, amp: 0.3, attack: 0, release: 0.1, cutoff: rrand_i(60, 90)
      sleep 0.1
    end
    
  end
  
  
  if cc == 6
    
    with_fx :normaliser, level: 0.3 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 44, 45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73] #mixolydian_mode
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Dscale = output_value
      
      use_synth :kalimba
      play Dscale, amp: 3, attack: 0.5, sustain: 0.25, release: 0.1
      sleep 0.1
    end
    
  end
  
  if cc == 7
    
    with_fx :normaliser, level: 0.3  do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 44, 45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73] #mixolydian_mode
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Escale = output_value
      
      use_synth :growl
      play Escale, amp: 1, attack: 0.5, sustain: 0.25, release: 1
      sleep 0.1
    end
    
  end
  
  
  if cc == 8
    
    with_fx :normaliser, level: 0.3 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 44, 45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 63, 64, 66, 68, 69, 71, 73] #mixolydian_mode
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Fscale = output_value
      
      use_synth :beep
      play Fscale, amp: 3, attack: 0.5, sustain: 0.25, release: 0.1
      sleep 0.1
    end
    
  end
  
  
  if cc == 9
    
    with_fx :normaliser, level: 0.3 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 42, 44, 47, 49, 52, 54, 56, 59, 61, 64, 66, 68, 71, 73, 76, 78, 80, 83, 85] #pentatonic_major_scale
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Gscale = output_value
      
      use_synth :dull_bell
      play Gscale, amp: 0.3, attack: 0.5, sustain: 1, release: 0.5
      sleep 0.1
    end
    
  end
  
  if cc == 10
    
    with_fx :normaliser, level: 0.3 do
      input_value = osc_in #replace "val" with input integer from osc.
      output_values = [40, 43, 45, 47, 50, 52, 55, 57, 59, 62, 64, 67, 69, 71, 74, 76, 79, 81, 83, 86] #pentatonic_minor_scale
      output_index = (input_value - 40) * output_values.size / 90 # map input range to output range
      output_index = output_index.round # round to the nearest integer
      output_value = output_values[output_index] # select the corresponding output value
      Hscale = output_value
      
      use_synth :dark_ambience
      play Hscale, amp: 2, attack: 0.2, sustain: 0.5, release: 3
      sleep 0.1
    end
    
  end
  
  
  sleep 2.0 / 8
end
