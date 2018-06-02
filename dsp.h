template <typename sample_type>
static inline sample_type attenuate(
    sample_type sample,
    uint8_t level_q0n8
  ) {
  return sample * level_q0n8 >> 8;  
}

