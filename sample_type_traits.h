template <typename sample_t> class sample_type_traits {
//  public:
//    typedef sample_t value_type;
//    static const int16_t maximum;
//    static const int16_t silence;
//    static const int16_t minimum;
//    static const int16_t bias_to_signed;
//    static const int16_t bias_to_unsigned;
//    static const value_type * sine_table;
};

template <> class sample_type_traits<int8_t> {
  public:
    typedef int8_t value_type;
    typedef int16_t mix_type;
    static const int16_t maximum = 127;
    static const int16_t silence = 0;
    static const int16_t minimum = -128;
    static const int16_t bias_to_signed = 0;
    static const int16_t bias_to_unsigned = 128;
    static const value_type * sine_table;
    static uint8_t to_uint8_t(value_type v) {
      return v + bias_to_unsigned;
    }
};

const int8_t * sample_type_traits<int8_t>::sine_table = lambOS::Tables::sin256_int8_data;

template <> class sample_type_traits<uint8_t> {
  public:
    typedef uint8_t value_type;
    typedef uint16_t mix_type;
    static const int16_t maximum = 255;
    static const int16_t silence = 128;
    static const int16_t minimum = 0;
    static const int16_t bias_to_signed = -128;
    static const int16_t bias_to_unsigned = 0;
    static const value_type * sine_table;
    static uint8_t to_uint8_t(value_type v) {
      return v;
    }
};

const uint8_t * sample_type_traits<uint8_t>::sine_table = lambOS::Tables::sin256_uint8_data;

