# COMP 412, Lab 1, Block 2
#
# a simplified linear algebra kernel
#
	loadI 	2	=> r100
	loadI	1025	=> r99
	store 	r100	=> r99
	loadI 	3	=> r100
	loadI 	1026	=> r99
	store 	r100	=> r99
	loadI 	5	=> r100
	loadI 	1027	=> r99
	store 	r100	=> r99
	loadI 	2048	=> r99
	store 	r100	=> r99
	loadI 	6	=> r100
	loadI 	4104	=> r99
	store 	r100	=> r99
	loadI 	1	=> r100
	loadI 	8192	=> r99
	store 	r100	=> r99
#
	loadI 	2	=> r1 
	loadI 	1025	=> r2
	load  	r2	=> r3
	sub   	r3,r1	=> r4
	loadI 	4	=> r5 
	mult   	r4,r5	=> r6
	loadI 	2048	=> r7
	add   	r6,r7	=> r8
	load  	r8	=> r9
#
	loadI 	1	=> r10 
	loadI 	1026	=> r11
	load  	r11	=> r12
	sub   	r12,r10	=> r13
	mult   	r13,r5	=> r14
	loadI 	4096	=> r15
	add   	r14,r15	=> r16
	load  	r16	=> r17
#
	loadI 	8192	=> r18
	load  	r18	=> r19
#
	mult   	r19,r9	=> r20
	add   	r17,r20 => r21
#
	loadI 	3	=> r22 
	loadI 	1027	=> r23
	load  	r23	=> r24
	sub   	r24,r22 => r25
	mult   	r25,r5	=> r26
	loadI 	16384	=> r27
	add   	r26,r27 => r28
	store 	r21	=> r28
#
	loadI 	1	=> r29 
	add   	r3,r29	=> r3
	store 	r3	=> r2
	add   	r12,r29 => r12
	store 	r12	=> r11
	add   	r24,r29 => r24
	store 	r24	=> r23
#
	output 1025
	output 1026
	output 1027
	output 16392
# end of block 2
