BR x
LOAD x
STORE V0
LOAD 0
SUB V0
BRZNEG L0
LOAD x
STORE V1
LOAD 0
SUB V1
BRZERO L1
x: NOOP
L1: NOOP
L0: NOOP
LOAD x
STORE V2
WRITE V2
STOP
x 0
V0 0
V1 0
V2 0
