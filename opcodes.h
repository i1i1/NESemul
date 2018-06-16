#ifndef _OPCODES_H_
#define _OPCODES_H_

struct opcode {
	void (*cmd)(void);
	void (*mode)(void);
	int cycles;
	char *name;
};

extern struct opcode ops[256];

#endif /* _OPCODES_H_ */
