
switch(cn) {
case 1: { MEM2_SUBMATRIX(uint8); } break;
case 2: { MEM2_SUBMATRIX(uint16); } break;
case 3: { MEM2_SUBMATRIX(uint24); } break;
case 4: { MEM2_SUBMATRIX(uint32); } break;
case 8: { MEM2_SUBMATRIX(uint64); } break;
default: ASSERT(0); break;
}
