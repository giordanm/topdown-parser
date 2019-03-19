# example from intro lecture, slide 12
	loadI	1	=> r8
	loadI 	1	=> r9  
	loadI 	1025	=> r10 
	loadI 	1026	=> r11 
	loadI 	1027	=> r12 
	loadI 	1028	=> r13 
	store 	r8	=> r10
	add   	r8, r9	=> r8 
	store 	r8	=> r11
	add   	r8, r9	=> r8 
	store 	r8	=> r12
	store 	r9	=> r13
	load  	r10	=> r1 
	lshift 	r1, r9	=> r1 
	load  	r11	=> r2 
	mult   	r1, r2	=> r1 
	load  	r12	=> r2
	mult   	r1, r2	=> r1 
	load  	r13	=> r2 
	mult   	r1, r2	=> r1 
	store 	r1	=> r10 
	output   	1025
#
