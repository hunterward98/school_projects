	AREA |.text|, CODE, READONLY, ALIGN=2
	EXPORT ReadWord
	
ReadWord
	PUSH {LR, R1}
	LDR R1, [R0]
	PUSH {R1}
	POP {R0}
	POP {LR, R1}
	BX LR
	
	END