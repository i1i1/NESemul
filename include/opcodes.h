#ifndef _OPCODES_H_
#define _OPCODES_H_

struct opcode {
	void (*cmd)(void);
	char *cname;
	void (*mode)(void);
	char *mname;
	int cycles;
	int len;
};

extern struct opcode ops[256];

#endif /* _OPCODES_H_ */
