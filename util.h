#define bit_val(data, y) (data >> y) & 1
#define bit_set(data, y) data |= (1 << y)
#define bit_clear(data, y) data &= ~(1 << y)
#define bit_toggle(data,y) (data ^= bit_val(y))
#define byte_toggle(data) (data = ~data )