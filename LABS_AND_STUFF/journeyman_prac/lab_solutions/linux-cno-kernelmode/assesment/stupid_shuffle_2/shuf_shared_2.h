#ifndef SHUFSHARED_H
#define SHUFSHARED_H

#define SHUF_MAJ        0x123
#define SHUF_MIN	0x456

#define SHUFBUF_SZ	0x100

// Read
// 	Read does a normal read at the correct offset in the buffer
// 	If a read tries to read over the edge of the buffer, it will only read until the end
// 	If a read tries to read when it's offset is == SHUFBUF_SZ, read returns zero

// Write
// 	Write will not write bytes to the buffer, but rather xors them in
// 	Not with a key, but with the bytes already there, at the correct offset it would have written to. The buffer should start as all zeros
// 	After a write, the affected area in the buffer has been xored with the input buffer (byte for byte) and the key (each byte)
// 	If a write tries to write past the end of a buffer, it will only write up until the end

// Lseek
// 	Lseek acts as expected, honoring SEEK_SET SEEK_CUR and SEEK_END
// 	Lseek will clamp to 0 or SHUFBUF_SZ if the offset tries to become too low or too high, respectively

// Other
// 	Make sure to protect the buffer with some sort of lock

#endif
