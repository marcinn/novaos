
	STRUC	ExtResident
		.rt_MatchWord	resb	2
		.rt_pad			resb	2
		.rt_MatchTag	resb	4
		.rt_EndSkip		resb	4
		.rt_Flags		resb	1
		.rt_Version		resb	1
		.rt_Type		resb	1
		.rt_Pri			resb	1
		.rt_Name		resb	4
		.rt_IdString	resb	4
		.rt_Init		resb	4
		.rt_Strap		resb	4
	ENDSTRUC
	
	%define	RTC_MATCHWORD	0x4AFC

