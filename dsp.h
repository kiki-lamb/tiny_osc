template <uint8_t S = 8, typename T1>
inline T1 mul_U8S(T1 x, uint8_t y) {
  return x * y >> S;  
}

template <uint8_t Y, uint8_t S = 8, typename T1>
inline T1 mul_U8S(T1 x) {
  return x * Y >> S;  
}

