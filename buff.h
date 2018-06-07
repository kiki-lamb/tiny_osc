
#define declare_buff256(t, name) volatile t name[256]; \
volatile uint8_t name ## _write_ix = 0; \
volatile uint8_t name ## _read_ix = 0; \
volatile uint8_t name ## _count = 0 

#define buff256_read(b) b[b ## _count--, b ## _read_ix++]

#define buff256_peek(b) b[b ## _read_ix]

#define buff256_write(b, v) b[ b ## _count++, b ## _write_ix++] = v

#define buff256_writable(b) (b ## _count < 255)

#define buff256_readable(b) (b ## _count)

#define declare_buff(t, len, name) volatile t name[8]; \
volatile uint8_t name ## _write_ix = 0; \
volatile uint8_t name ## _read_ix = 0; \
volatile uint8_t name ## _count = 0; \
const uint8_t name ## _length = len

#define buff_read(b)     b[b ## _read_ix %= b ## _length, b ## _count--, b ## _read_ix++]
#define buff_peek(b)     b[b ## _read_ix]

#define buff_write(b, v) b[b ## _write_ix %= b ## _length,  b ## _count++, b ## _write_ix++] = v

#define buff_writable(b) (b ## _count < b ## _length)

#define buff_readable(b) (b ## _count)

