class DecayEnvelope  {
  public:
    uint16_t decay;
    uint16_t decay_remaining;
    
    DecayEnvelope(uint16_t _decay = 65535) : 
      decay(_decay), 
      decay_remaining(0) {
    }

    inline uint16_t read() {
      return ( 
        decay_remaining?
        decay_remaining-- :
        0
      );
    }

    inline void trigger() {
      decay_remaining = decay;
    }
};

DecayEnvelope denv;
