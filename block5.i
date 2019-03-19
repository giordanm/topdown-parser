# COMP 412, LAB 1
#
# Fibonacci numbers, using few registers (6)
#
	loadI	0	=> r0 
	loadI	1	=> r1 
	loadI	0	=> r2 
	loadI	1	=> r3 
	loadI   2000	=> r10
# f0
	store	r2	=> r10
# f1
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f2
	add	r2,r3 	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f3
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f4
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f5
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f6
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f7
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f8
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f9
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
# f10
	add	r2,r3	=> r4
	add	r3,r0	=> r2
	add 	r4,r0	=> r3
	add	r10,r1	=> r10
	store	r2	=> r10
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
	output  2009
	output  2010
# end of block
