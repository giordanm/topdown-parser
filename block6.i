# COMP 412, FALL 1997, LAB 1
#
# Fibonnaci numbers, using more registers (15)
#
	loadI	0	=> r0
	loadI	1	=> r1
	loadI	0	=> r2
	loadI	1	=> r3
	loadI   2000	=> r20
# f0
	store	r2	=> r20
# f1
	add	r2,r3	=> r4
	add	r20,r1	=> r20
	store	r4	=> r20
# f2
	add	r4,r2	=> r5
	add	r20,r1	=> r20
	store	r5	=> r20
# f3
	add	r5,r4	=> r6
	add	r20,r1	=> r20
	store	r6	=> r20
# f4
	add	r6,r5	=> r7
	add	r20,r1	=> r20
	store	r7	=> r20
# f5
	add	r7,r6	=> r8
	add	r20,r1	=> r20
	store	r8	=> r20
# f6
	add	r8,r7	=> r9
	add	r20,r1	=> r20
	store	r9	=> r20
# f7
	add	r9,r8	=> r10
	add	r20,r1	=> r20
	store	r10	=> r20
# f8
	add	r10,r9	=> r11
	add	r20,r1	=> r20
	store	r11	=> r20
# f9
	add	r11,r10	=> r12
	add	r20,r1	=> r20
	store	r12	=> r20
# f10
	add	r12,r11	=> r13
	add	r20,r1	=> r20
	store	r13	=> r20
# 
	output 	2000
	output  2001
	output  2002
	output  2003
	output 	2004
	output 	2005
	output 	2006
	output  2007
	output  2008
	output 	2009
	output  2010
# end of block
