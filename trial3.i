loadI 1024 => r1
loadI 1028 => r2
loadI 24 => r3
store r3 => r1
load r1 => r4
store r4 => r2
output 1024
output 1028
add r1, r2 => r3
mult r1, r2 => r4
lshift r1, r2 => r3
rshift r1, r2 => r4
