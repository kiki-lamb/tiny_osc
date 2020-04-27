volatile uint32_t sequencer_time = 0;

uint8_t seq[] = "ffrriufrbbnraamq"; // main riff from 'Sweet Dreams'.

////////////////////////////////////////////////////////////////////////////////////

void sequencer_tick() {
  if (sequencer_time < (interval))
    return;

  sequencer_time = 0;

  flip_led();
  
  static uint8_t iix = 0;

//  env.trigger();

//  uint8_t note = seq[iix >> 0] - 55;

//  for (uint16_t ix = 0, f = note; ix < VOICES; ix ++)
//    oscs[ix].set_note(f);
  
  iix ++;
  iix %= 16;
  
}
