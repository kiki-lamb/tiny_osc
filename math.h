class Math {
  public:
  
  template <uint8_t RSHIFT = 8, typename T1>
  static inline __attribute__((always_inline)) T1 mul_U8S(T1 x, uint8_t y) {
    return x * y >> RSHIFT;  
  }

  template <uint8_t Y, uint8_t RSHIFT = 8, typename T1>
  static inline  __attribute__((always_inline)) T1 mul_U8S(T1 x) {
    return x * Y >> RSHIFT;  
  }
  
  private:
  Math();
};
