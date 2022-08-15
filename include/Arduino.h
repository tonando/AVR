
#define pinMode(p, m) m?SET_OUTPUT(p):SET_INPUT(p)
#define digitalWrite(p, v) v?ON(p):OFF(p)
