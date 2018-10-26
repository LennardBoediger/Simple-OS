
kernel:     file format elf32-littlearm


Disassembly of section .init:

00008000 <_start>:
    8000:	e10f0000 	mrs	r0, CPSR
    8004:	e200001f 	and	r0, r0, #31
    8008:	e3a0101a 	mov	r1, #26
    800c:	e1500001 	cmp	r0, r1
    8010:	0a000001 	beq	801c <_exitHyper>

00008014 <_bsprak>:
    8014:	eb000029 	bl	80c0 <echo>
    8018:	eafffffe 	b	8018 <_bsprak+0x4>

0000801c <_exitHyper>:
    801c:	e59fe014 	ldr	lr, [pc, #20]	; 8038 <_exitHyper+0x1c>
    8020:	e12ef30e 	msr	ELR_hyp, lr
    8024:	e10f0000 	mrs	r0, CPSR
    8028:	e3c0001f 	bic	r0, r0, #31
    802c:	e3800013 	orr	r0, r0, #19
    8030:	e16ef300 	msr	SPSR_hyp, r0
    8034:	e160006e 	eret
    8038:	00008014 	andeq	r8, r0, r4, lsl r0

Disassembly of section .text:

0000803c <uart_recive>:
    803c:	e3a02a01 	mov	r2, #4096	; 0x1000
    8040:	e3432f20 	movt	r2, #16160	; 0x3f20
    8044:	e5923018 	ldr	r3, [r2, #24]
    8048:	e3130010 	tst	r3, #16
    804c:	0afffffc 	beq	8044 <uart_recive+0x8>
    8050:	e3a01a01 	mov	r1, #4096	; 0x1000
    8054:	e3431f20 	movt	r1, #16160	; 0x3f20
    8058:	e5912018 	ldr	r2, [r1, #24]
    805c:	e3a03a01 	mov	r3, #4096	; 0x1000
    8060:	e3433f20 	movt	r3, #16160	; 0x3f20
    8064:	e3120020 	tst	r2, #32
    8068:	1afffffa 	bne	8058 <uart_recive+0x1c>
    806c:	e3a02072 	mov	r2, #114	; 0x72
    8070:	e1a01003 	mov	r1, r3
    8074:	e5832000 	str	r2, [r3]
    8078:	e5930000 	ldr	r0, [r3]
    807c:	e6ef0070 	uxtb	r0, r0
    8080:	e5912018 	ldr	r2, [r1, #24]
    8084:	e3a03a01 	mov	r3, #4096	; 0x1000
    8088:	e3433f20 	movt	r3, #16160	; 0x3f20
    808c:	e3120020 	tst	r2, #32
    8090:	1afffffa 	bne	8080 <uart_recive+0x44>
    8094:	e5830000 	str	r0, [r3]
    8098:	e12fff1e 	bx	lr

0000809c <uart_transmit>:
    809c:	e3a01a01 	mov	r1, #4096	; 0x1000
    80a0:	e3431f20 	movt	r1, #16160	; 0x3f20
    80a4:	e5912018 	ldr	r2, [r1, #24]
    80a8:	e3a03a01 	mov	r3, #4096	; 0x1000
    80ac:	e3433f20 	movt	r3, #16160	; 0x3f20
    80b0:	e3120020 	tst	r2, #32
    80b4:	1afffffa 	bne	80a4 <uart_transmit+0x8>
    80b8:	e5830000 	str	r0, [r3]
    80bc:	e12fff1e 	bx	lr

000080c0 <echo>:
    80c0:	e3a02a01 	mov	r2, #4096	; 0x1000
    80c4:	e3432f20 	movt	r2, #16160	; 0x3f20
    80c8:	e92d4038 	push	{r3, r4, r5, lr}
    80cc:	e3a03a01 	mov	r3, #4096	; 0x1000
    80d0:	e5921018 	ldr	r1, [r2, #24]
    80d4:	e3433f20 	movt	r3, #16160	; 0x3f20
    80d8:	e3110020 	tst	r1, #32
    80dc:	1afffffa 	bne	80cc <echo+0xc>
    80e0:	e1a04003 	mov	r4, r3
    80e4:	e1a05003 	mov	r5, r3
    80e8:	e3a02063 	mov	r2, #99	; 0x63
    80ec:	e5832000 	str	r2, [r3]
    80f0:	ebffffd1 	bl	803c <uart_recive>
    80f4:	e5943018 	ldr	r3, [r4, #24]
    80f8:	e3130020 	tst	r3, #32
    80fc:	1afffffc 	bne	80f4 <echo+0x34>
    8100:	e5850000 	str	r0, [r5]
    8104:	eafffff9 	b	80f0 <echo+0x30>

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00003641 	andeq	r3, r0, r1, asr #12
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	0000002c 	andeq	r0, r0, ip, lsr #32
  10:	726f4305 	rsbvc	r4, pc, #335544320	; 0x14000000
  14:	2d786574 	cfldr64cs	mvdx6, [r8, #-464]!	; 0xfffffe30
  18:	06003741 	streq	r3, [r0], -r1, asr #14
  1c:	0841070a 	stmdaeq	r1, {r1, r3, r8, r9, sl}^
  20:	12020901 	andne	r0, r2, #16384	; 0x4000
  24:	15011404 	strne	r1, [r1, #-1028]	; 0xfffffbfc
  28:	18031701 	stmdane	r3, {r0, r8, r9, sl, ip}
  2c:	22011a01 	andcs	r1, r1, #4096	; 0x1000
  30:	2c012a01 	stccs	10, cr2, [r1], {1}
  34:	Address 0x0000000000000034 is out of bounds.


Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <echo+0x10c8c64>
   4:	35312820 	ldrcc	r2, [r1, #-2080]!	; 0xfffff7e0
   8:	392e343a 	stmdbcc	lr!, {r1, r3, r4, r5, sl, ip, sp}
   c:	732b332e 			; <UNDEFINED> instruction: 0x732b332e
  10:	33326e76 	teqcc	r2, #1888	; 0x760
  14:	37373131 			; <UNDEFINED> instruction: 0x37373131
  18:	2029312d 	eorcs	r3, r9, sp, lsr #2
  1c:	2e392e34 	mrccs	14, 1, r2, cr9, cr4, {1}
  20:	30322033 	eorscc	r2, r2, r3, lsr r0
  24:	35303531 	ldrcc	r3, [r0, #-1329]!	; 0xfffffacf
  28:	28203932 	stmdacs	r0!, {r1, r4, r5, r8, fp, ip, sp}
  2c:	72657270 	rsbvc	r7, r5, #112, 4
  30:	61656c65 	cmnvs	r5, r5, ror #24
  34:	00296573 	eoreq	r6, r9, r3, ror r5
