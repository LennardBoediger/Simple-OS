
./bs-logic/kernel:     file format elf32-littlearm


Disassembly of section .init:

00008000 <_start>:
    8000:	e10f0000 	mrs	r0, CPSR
    8004:	e200001f 	and	r0, r0, #31
    8008:	e3a0101a 	mov	r1, #26
    800c:	e1500001 	cmp	r0, r1
    8010:	0a000004 	beq	8028 <_exitHyper>

00008014 <_bsprak>:
    8014:	eb000129 	bl	84c0 <_INIT_IVT>
    8018:	eb000146 	bl	8538 <INIT_STACK>
    801c:	e7f000f0 	udf	#0
    8020:	eb000152 	bl	8570 <main>
    8024:	eafffffe 	b	8024 <_bsprak+0x10>

00008028 <_exitHyper>:
    8028:	e59fe014 	ldr	lr, [pc, #20]	; 8044 <_exitHyper+0x1c>
    802c:	e12ef30e 	msr	ELR_hyp, lr
    8030:	e10f0000 	mrs	r0, CPSR
    8034:	e3c0001f 	bic	r0, r0, #31
    8038:	e3800013 	orr	r0, r0, #19
    803c:	e16ef300 	msr	SPSR_hyp, r0
    8040:	e160006e 	eret
    8044:	00008014 	andeq	r8, r0, r4, lsl r0

Disassembly of section .text:

00008060 <sent_string>:
    8060:	e92d4010 	push	{r4, lr}
    8064:	e1a04000 	mov	r4, r0
    8068:	e5d00000 	ldrb	r0, [r0]
    806c:	e3500000 	cmp	r0, #0
    8070:	08bd8010 	popeq	{r4, pc}
    8074:	eb0000f1 	bl	8440 <uart_transmit>
    8078:	e5f40001 	ldrb	r0, [r4, #1]!
    807c:	e3500000 	cmp	r0, #0
    8080:	1afffffb 	bne	8074 <sent_string+0x14>
    8084:	e8bd8010 	pop	{r4, pc}

00008088 <conv_to_ASCII>:
    8088:	e350000f 	cmp	r0, #15
    808c:	93083790 	movwls	r3, #34704	; 0x8790
    8090:	93403000 	movtls	r3, #0
    8094:	97d30000 	ldrbls	r0, [r3, r0]
    8098:	83a00021 	movhi	r0, #33	; 0x21
    809c:	e12fff1e 	bx	lr

000080a0 <sent_hex>:
    80a0:	e3500000 	cmp	r0, #0
    80a4:	e92d4030 	push	{r4, r5, lr}
    80a8:	e24dd00c 	sub	sp, sp, #12
    80ac:	0a000022 	beq	813c <sent_hex+0x9c>
    80b0:	e308c790 	movw	ip, #34704	; 0x8790
    80b4:	e3a04000 	mov	r4, #0
    80b8:	e340c000 	movt	ip, #0
    80bc:	e200300f 	and	r3, r0, #15
    80c0:	e2841001 	add	r1, r4, #1
    80c4:	e1b00220 	lsrs	r0, r0, #4
    80c8:	e7dc2003 	ldrb	r2, [ip, r3]
    80cc:	e28d3008 	add	r3, sp, #8
    80d0:	e6a33074 	sxtab	r3, r3, r4
    80d4:	e6ef4071 	uxtb	r4, r1
    80d8:	e5432008 	strb	r2, [r3, #-8]
    80dc:	1afffff6 	bne	80bc <sent_hex+0x1c>
    80e0:	e2444001 	sub	r4, r4, #1
    80e4:	e6ef4074 	uxtb	r4, r4
    80e8:	e3085764 	movw	r5, #34660	; 0x8764
    80ec:	e3a00030 	mov	r0, #48	; 0x30
    80f0:	e3405000 	movt	r5, #0
    80f4:	eb0000d1 	bl	8440 <uart_transmit>
    80f8:	e5f50001 	ldrb	r0, [r5, #1]!
    80fc:	e3500000 	cmp	r0, #0
    8100:	1afffffb 	bne	80f4 <sent_hex+0x54>
    8104:	e6af3074 	sxtb	r3, r4
    8108:	e3530000 	cmp	r3, #0
    810c:	ba000008 	blt	8134 <sent_hex+0x94>
    8110:	e28d2008 	add	r2, sp, #8
    8114:	e2444001 	sub	r4, r4, #1
    8118:	e0823003 	add	r3, r2, r3
    811c:	e6ef4074 	uxtb	r4, r4
    8120:	e5530008 	ldrb	r0, [r3, #-8]
    8124:	eb0000c5 	bl	8440 <uart_transmit>
    8128:	e6af3074 	sxtb	r3, r4
    812c:	e3730001 	cmn	r3, #1
    8130:	1afffff6 	bne	8110 <sent_hex+0x70>
    8134:	e28dd00c 	add	sp, sp, #12
    8138:	e8bd8030 	pop	{r4, r5, pc}
    813c:	e3a040ff 	mov	r4, #255	; 0xff
    8140:	eaffffe8 	b	80e8 <sent_hex+0x48>

00008144 <sent_dez>:
    8144:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    8148:	e3500000 	cmp	r0, #0
    814c:	e24dd010 	sub	sp, sp, #16
    8150:	e3a03000 	mov	r3, #0
    8154:	e58d3004 	str	r3, [sp, #4]
    8158:	e58d3008 	str	r3, [sp, #8]
    815c:	e1cd30bc 	strh	r3, [sp, #12]
    8160:	ba000026 	blt	8200 <sent_dez+0xbc>
    8164:	e1a02000 	mov	r2, r0
    8168:	0a000022 	beq	81f8 <sent_dez+0xb4>
    816c:	e3085790 	movw	r5, #34704	; 0x8790
    8170:	e30ccccd 	movw	ip, #52429	; 0xcccd
    8174:	e3405000 	movt	r5, #0
    8178:	e34ccccc 	movt	ip, #52428	; 0xcccc
    817c:	e3a04000 	mov	r4, #0
    8180:	e3a0800a 	mov	r8, #10
    8184:	e0876c92 	umull	r6, r7, r2, ip
    8188:	e081329c 	umull	r3, r1, ip, r2
    818c:	e1a031a7 	lsr	r3, r7, #3
    8190:	e2846001 	add	r6, r4, #1
    8194:	e0632398 	mls	r3, r8, r3, r2
    8198:	e28d2010 	add	r2, sp, #16
    819c:	e6ef3073 	uxtb	r3, r3
    81a0:	e6a2e074 	sxtab	lr, r2, r4
    81a4:	e6ef4076 	uxtb	r4, r6
    81a8:	e7d53003 	ldrb	r3, [r5, r3]
    81ac:	e1b021a1 	lsrs	r2, r1, #3
    81b0:	e54e300c 	strb	r3, [lr, #-12]
    81b4:	1afffff2 	bne	8184 <sent_dez+0x40>
    81b8:	e3500000 	cmp	r0, #0
    81bc:	e2444001 	sub	r4, r4, #1
    81c0:	e6ef4074 	uxtb	r4, r4
    81c4:	ba000011 	blt	8210 <sent_dez+0xcc>
    81c8:	e6af3074 	sxtb	r3, r4
    81cc:	e3530000 	cmp	r3, #0
    81d0:	ba000008 	blt	81f8 <sent_dez+0xb4>
    81d4:	e28d2010 	add	r2, sp, #16
    81d8:	e2444001 	sub	r4, r4, #1
    81dc:	e0823003 	add	r3, r2, r3
    81e0:	e6ef4074 	uxtb	r4, r4
    81e4:	e553000c 	ldrb	r0, [r3, #-12]
    81e8:	eb000094 	bl	8440 <uart_transmit>
    81ec:	e6af3074 	sxtb	r3, r4
    81f0:	e3730001 	cmn	r3, #1
    81f4:	1afffff6 	bne	81d4 <sent_dez+0x90>
    81f8:	e28dd010 	add	sp, sp, #16
    81fc:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
    8200:	e2602000 	rsb	r2, r0, #0
    8204:	e1520003 	cmp	r2, r3
    8208:	03a040ff 	moveq	r4, #255	; 0xff
    820c:	1affffd6 	bne	816c <sent_dez+0x28>
    8210:	e3a0002d 	mov	r0, #45	; 0x2d
    8214:	eb000089 	bl	8440 <uart_transmit>
    8218:	eaffffea 	b	81c8 <sent_dez+0x84>

0000821c <sent_udez>:
    821c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    8220:	e250c000 	subs	ip, r0, #0
    8224:	e24dd010 	sub	sp, sp, #16
    8228:	e3a01000 	mov	r1, #0
    822c:	e58d1004 	str	r1, [sp, #4]
    8230:	e58d1008 	str	r1, [sp, #8]
    8234:	e1cd10bc 	strh	r1, [sp, #12]
    8238:	0a00001d 	beq	82b4 <sent_udez+0x98>
    823c:	e3086790 	movw	r6, #34704	; 0x8790
    8240:	e30c7ccd 	movw	r7, #52429	; 0xcccd
    8244:	e3406000 	movt	r6, #0
    8248:	e34c7ccc 	movt	r7, #52428	; 0xcccc
    824c:	e3a0800a 	mov	r8, #10
    8250:	e0832c97 	umull	r2, r3, r7, ip
    8254:	e6ef4071 	uxtb	r4, r1
    8258:	e28d0010 	add	r0, sp, #16
    825c:	e1a031a3 	lsr	r3, r3, #3
    8260:	e6afe074 	sxtb	lr, r4
    8264:	e062c398 	mls	r2, r8, r3, ip
    8268:	e080500e 	add	r5, r0, lr
    826c:	e253c000 	subs	ip, r3, #0
    8270:	e6ef3072 	uxtb	r3, r2
    8274:	e2811001 	add	r1, r1, #1
    8278:	e7d60003 	ldrb	r0, [r6, r3]
    827c:	e545000c 	strb	r0, [r5, #-12]
    8280:	1afffff2 	bne	8250 <sent_udez+0x34>
    8284:	e35e0000 	cmp	lr, #0
    8288:	aa000001 	bge	8294 <sent_udez+0x78>
    828c:	ea000008 	b	82b4 <sent_udez+0x98>
    8290:	e552000c 	ldrb	r0, [r2, #-12]
    8294:	e2444001 	sub	r4, r4, #1
    8298:	eb000068 	bl	8440 <uart_transmit>
    829c:	e6ef4074 	uxtb	r4, r4
    82a0:	e28d2010 	add	r2, sp, #16
    82a4:	e6af3074 	sxtb	r3, r4
    82a8:	e0822003 	add	r2, r2, r3
    82ac:	e3530000 	cmp	r3, #0
    82b0:	aafffff6 	bge	8290 <sent_udez+0x74>
    82b4:	e28dd010 	add	sp, sp, #16
    82b8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

000082bc <kprintf>:
    82bc:	e92d000f 	push	{r0, r1, r2, r3}
    82c0:	e92d4030 	push	{r4, r5, lr}
    82c4:	e24dd00c 	sub	sp, sp, #12
    82c8:	e28d301c 	add	r3, sp, #28
    82cc:	e59d4018 	ldr	r4, [sp, #24]
    82d0:	e58d3004 	str	r3, [sp, #4]
    82d4:	e5d43000 	ldrb	r3, [r4]
    82d8:	e3530000 	cmp	r3, #0
    82dc:	1a000006 	bne	82fc <kprintf+0x40>
    82e0:	ea00002a 	b	8390 <kprintf+0xd4>
    82e4:	e1a04005 	mov	r4, r5
    82e8:	e4d40001 	ldrb	r0, [r4], #1
    82ec:	eb000053 	bl	8440 <uart_transmit>
    82f0:	e5d53001 	ldrb	r3, [r5, #1]
    82f4:	e3530000 	cmp	r3, #0
    82f8:	0a000024 	beq	8390 <kprintf+0xd4>
    82fc:	e3530025 	cmp	r3, #37	; 0x25
    8300:	11a05004 	movne	r5, r4
    8304:	1afffff6 	bne	82e4 <kprintf+0x28>
    8308:	e5d40001 	ldrb	r0, [r4, #1]
    830c:	e2403063 	sub	r3, r0, #99	; 0x63
    8310:	e3530015 	cmp	r3, #21
    8314:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
    8318:	ea00003e 	b	8418 <kprintf+0x15c>
    831c:	00008400 	andeq	r8, r0, r0, lsl #8
    8320:	00008418 	andeq	r8, r0, r8, lsl r4
    8324:	00008418 	andeq	r8, r0, r8, lsl r4
    8328:	00008418 	andeq	r8, r0, r8, lsl r4
    832c:	00008418 	andeq	r8, r0, r8, lsl r4
    8330:	00008418 	andeq	r8, r0, r8, lsl r4
    8334:	000083e8 	andeq	r8, r0, r8, ror #7
    8338:	00008418 	andeq	r8, r0, r8, lsl r4
    833c:	00008418 	andeq	r8, r0, r8, lsl r4
    8340:	00008418 	andeq	r8, r0, r8, lsl r4
    8344:	00008418 	andeq	r8, r0, r8, lsl r4
    8348:	00008418 	andeq	r8, r0, r8, lsl r4
    834c:	00008418 	andeq	r8, r0, r8, lsl r4
    8350:	00008374 	andeq	r8, r0, r4, ror r3
    8354:	00008418 	andeq	r8, r0, r8, lsl r4
    8358:	00008418 	andeq	r8, r0, r8, lsl r4
    835c:	000083b8 			; <UNDEFINED> instruction: 0x000083b8
    8360:	00008418 	andeq	r8, r0, r8, lsl r4
    8364:	000083a0 	andeq	r8, r0, r0, lsr #7
    8368:	00008418 	andeq	r8, r0, r8, lsl r4
    836c:	00008418 	andeq	r8, r0, r8, lsl r4
    8370:	00008374 	andeq	r8, r0, r4, ror r3
    8374:	e59d3004 	ldr	r3, [sp, #4]
    8378:	e5930000 	ldr	r0, [r3]
    837c:	e2833004 	add	r3, r3, #4
    8380:	e58d3004 	str	r3, [sp, #4]
    8384:	ebffff45 	bl	80a0 <sent_hex>
    8388:	e2845002 	add	r5, r4, #2
    838c:	eaffffd4 	b	82e4 <kprintf+0x28>
    8390:	e28dd00c 	add	sp, sp, #12
    8394:	e8bd4030 	pop	{r4, r5, lr}
    8398:	e28dd010 	add	sp, sp, #16
    839c:	e12fff1e 	bx	lr
    83a0:	e59d3004 	ldr	r3, [sp, #4]
    83a4:	e5930000 	ldr	r0, [r3]
    83a8:	e2833004 	add	r3, r3, #4
    83ac:	e58d3004 	str	r3, [sp, #4]
    83b0:	ebffff99 	bl	821c <sent_udez>
    83b4:	eafffff3 	b	8388 <kprintf+0xcc>
    83b8:	e59d3004 	ldr	r3, [sp, #4]
    83bc:	e5935000 	ldr	r5, [r3]
    83c0:	e2833004 	add	r3, r3, #4
    83c4:	e58d3004 	str	r3, [sp, #4]
    83c8:	e5d50000 	ldrb	r0, [r5]
    83cc:	e3500000 	cmp	r0, #0
    83d0:	0affffec 	beq	8388 <kprintf+0xcc>
    83d4:	eb000019 	bl	8440 <uart_transmit>
    83d8:	e5f50001 	ldrb	r0, [r5, #1]!
    83dc:	e3500000 	cmp	r0, #0
    83e0:	1afffffb 	bne	83d4 <kprintf+0x118>
    83e4:	eaffffe7 	b	8388 <kprintf+0xcc>
    83e8:	e59d3004 	ldr	r3, [sp, #4]
    83ec:	e5930000 	ldr	r0, [r3]
    83f0:	e2833004 	add	r3, r3, #4
    83f4:	e58d3004 	str	r3, [sp, #4]
    83f8:	ebffff51 	bl	8144 <sent_dez>
    83fc:	eaffffe1 	b	8388 <kprintf+0xcc>
    8400:	e59d3004 	ldr	r3, [sp, #4]
    8404:	e5d30000 	ldrb	r0, [r3]
    8408:	e2833004 	add	r3, r3, #4
    840c:	e58d3004 	str	r3, [sp, #4]
    8410:	eb00000a 	bl	8440 <uart_transmit>
    8414:	eaffffdb 	b	8388 <kprintf+0xcc>
    8418:	eb000008 	bl	8440 <uart_transmit>
    841c:	eaffffd9 	b	8388 <kprintf+0xcc>

00008420 <uart_receive>:
    8420:	e3a03a01 	mov	r3, #4096	; 0x1000
    8424:	e3433f20 	movt	r3, #16160	; 0x3f20
    8428:	e5932018 	ldr	r2, [r3, #24]
    842c:	e3120010 	tst	r2, #16
    8430:	05930000 	ldreq	r0, [r3]
    8434:	06ef0070 	uxtbeq	r0, r0
    8438:	13a00000 	movne	r0, #0
    843c:	e12fff1e 	bx	lr

00008440 <uart_transmit>:
    8440:	e3a01a01 	mov	r1, #4096	; 0x1000
    8444:	e3431f20 	movt	r1, #16160	; 0x3f20
    8448:	e5912018 	ldr	r2, [r1, #24]
    844c:	e3a03a01 	mov	r3, #4096	; 0x1000
    8450:	e3433f20 	movt	r3, #16160	; 0x3f20
    8454:	e3120020 	tst	r2, #32
    8458:	1afffffa 	bne	8448 <uart_transmit+0x8>
    845c:	e5830000 	str	r0, [r3]
    8460:	e12fff1e 	bx	lr

00008464 <reset>:
    8464:	e3080768 	movw	r0, #34664	; 0x8768
    8468:	e92d4008 	push	{r3, lr}
    846c:	e3400000 	movt	r0, #0
    8470:	ebffff91 	bl	82bc <kprintf>
    8474:	e308077c 	movw	r0, #34684	; 0x877c
    8478:	e3400000 	movt	r0, #0
    847c:	e8bd4008 	pop	{r3, lr}
    8480:	eaffff8d 	b	82bc <kprintf>

00008484 <undef>:
    8484:	e3080768 	movw	r0, #34664	; 0x8768
    8488:	e92d4008 	push	{r3, lr}
    848c:	e3400000 	movt	r0, #0
    8490:	ebffff89 	bl	82bc <kprintf>
    8494:	e308077c 	movw	r0, #34684	; 0x877c
    8498:	e3400000 	movt	r0, #0
    849c:	e8bd4008 	pop	{r3, lr}
    84a0:	eaffff85 	b	82bc <kprintf>
	...

000084c0 <_INIT_IVT>:
    84c0:	e59f0018 	ldr	r0, [pc, #24]	; 84e0 <_IVT>
    84c4:	ee0c0f10 	mcr	15, 0, r0, cr12, cr0, {0}
    84c8:	e12fff1e 	bx	lr
    84cc:	e320f000 	nop	{0}
    84d0:	e320f000 	nop	{0}
    84d4:	e320f000 	nop	{0}
    84d8:	e320f000 	nop	{0}
    84dc:	e320f000 	nop	{0}

000084e0 <_IVT>:
    84e0:	ea000006 	b	8500 <reset_tr>
    84e4:	ea000007 	b	8508 <undef_tr>
    84e8:	ea000008 	b	8510 <swi_tr>
    84ec:	ea000009 	b	8518 <prefab_tr>
    84f0:	ea00000a 	b	8520 <dataab_tr>
    84f4:	e320f000 	nop	{0}
    84f8:	ea00000a 	b	8528 <irq_tr>
    84fc:	ea00000b 	b	8530 <fiq_tr>

00008500 <reset_tr>:
    8500:	e92d1fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    8504:	ebffffd6 	bl	8464 <reset>

00008508 <undef_tr>:
    8508:	e92d1fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    850c:	ebffffdc 	bl	8484 <undef>

00008510 <swi_tr>:
    8510:	e98d1fff 	stmib	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    8514:	ebffffd2 	bl	8464 <reset>

00008518 <prefab_tr>:
    8518:	e98d1fff 	stmib	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    851c:	ebffffd0 	bl	8464 <reset>

00008520 <dataab_tr>:
    8520:	e98d1fff 	stmib	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    8524:	ebffffce 	bl	8464 <reset>

00008528 <irq_tr>:
    8528:	e98d1fff 	stmib	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    852c:	ebffffcc 	bl	8464 <reset>

00008530 <fiq_tr>:
    8530:	e98d1fff 	stmib	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
    8534:	ebffffca 	bl	8464 <reset>

00008538 <INIT_STACK>:
    8538:	e321f0df 	msr	CPSR_c, #223	; 0xdf
    853c:	e3a0d67b 	mov	sp, #128974848	; 0x7b00000
    8540:	e321f0db 	msr	CPSR_c, #219	; 0xdb
    8544:	e3a0d51f 	mov	sp, #130023424	; 0x7c00000
    8548:	e321f0d7 	msr	CPSR_c, #215	; 0xd7
    854c:	e3a0d67d 	mov	sp, #131072000	; 0x7d00000
    8550:	e321f0d3 	msr	CPSR_c, #211	; 0xd3
    8554:	e3a0d67e 	mov	sp, #132120576	; 0x7e00000
    8558:	e321f0d2 	msr	CPSR_c, #210	; 0xd2
    855c:	e3a0d67f 	mov	sp, #133169152	; 0x7f00000
    8560:	e321f0d1 	msr	CPSR_c, #209	; 0xd1
    8564:	e3a0d302 	mov	sp, #134217728	; 0x8000000
    8568:	e321f0d3 	msr	CPSR_c, #211	; 0xd3
    856c:	e12fff1e 	bx	lr

Disassembly of section .text.startup:

00008570 <main>:
    8570:	e3080628 	movw	r0, #34344	; 0x8628
    8574:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    8578:	e3400000 	movt	r0, #0
    857c:	e24dd00c 	sub	sp, sp, #12
    8580:	ebffff4d 	bl	82bc <kprintf>
    8584:	e3080634 	movw	r0, #34356	; 0x8634
    8588:	e3a01062 	mov	r1, #98	; 0x62
    858c:	e3400000 	movt	r0, #0
    8590:	ebffff49 	bl	82bc <kprintf>
    8594:	e3080654 	movw	r0, #34388	; 0x8654
    8598:	e3081668 	movw	r1, #34408	; 0x8668
    859c:	e3400000 	movt	r0, #0
    85a0:	e3401000 	movt	r1, #0
    85a4:	ebffff44 	bl	82bc <kprintf>
    85a8:	e3080678 	movw	r0, #34424	; 0x8678
    85ac:	e30b17f8 	movw	r1, #47096	; 0xb7f8
    85b0:	e3400000 	movt	r0, #0
    85b4:	ebffff40 	bl	82bc <kprintf>
    85b8:	e308069c 	movw	r0, #34460	; 0x869c
    85bc:	e3a01102 	mov	r1, #-2147483648	; 0x80000000
    85c0:	e3400000 	movt	r0, #0
    85c4:	ebffff3c 	bl	82bc <kprintf>
    85c8:	e30806c0 	movw	r0, #34496	; 0x86c0
    85cc:	e3e01102 	mvn	r1, #-2147483648	; 0x80000000
    85d0:	e3400000 	movt	r0, #0
    85d4:	ebffff38 	bl	82bc <kprintf>
    85d8:	e30806e4 	movw	r0, #34532	; 0x86e4
    85dc:	e3e01000 	mvn	r1, #0
    85e0:	e3400000 	movt	r0, #0
    85e4:	ebffff34 	bl	82bc <kprintf>
    85e8:	e28d1008 	add	r1, sp, #8
    85ec:	e3a03034 	mov	r3, #52	; 0x34
    85f0:	e3080708 	movw	r0, #34568	; 0x8708
    85f4:	e5613001 	strb	r3, [r1, #-1]!
    85f8:	e3400000 	movt	r0, #0
    85fc:	ebffff2e 	bl	82bc <kprintf>
    8600:	e3080730 	movw	r0, #34608	; 0x8730
    8604:	e3400000 	movt	r0, #0
    8608:	ebffff2b 	bl	82bc <kprintf>
    860c:	ebffff83 	bl	8420 <uart_receive>
    8610:	e2501000 	subs	r1, r0, #0
    8614:	0afffffc 	beq	860c <main+0x9c>
    8618:	e3080750 	movw	r0, #34640	; 0x8750
    861c:	e3400000 	movt	r0, #0
    8620:	ebffff25 	bl	82bc <kprintf>
    8624:	eafffff8 	b	860c <main+0x9c>

Disassembly of section .rodata.str1.4:

00008628 <.rodata.str1.4>:
    8628:	74736554 	ldrbtvc	r6, [r3], #-1364	; 0xfffffaac
    862c:	69676562 	stmdbvs	r7!, {r1, r5, r6, r8, sl, sp, lr}^
    8630:	000d0a6e 	andeq	r0, sp, lr, ror #20
    8634:	3a632525 	bcc	18d1ad0 <CSWTCH.20+0x18c9340>
    8638:	77206220 	strvc	r6, [r0, -r0, lsr #4]!
    863c:	20647269 	rsbcs	r7, r4, r9, ror #4
    8640:	67737561 	ldrbvs	r7, [r3, -r1, ror #10]!
    8644:	62656765 	rsbvs	r6, r5, #26476544	; 0x1940000
    8648:	203a6e65 	eorscs	r6, sl, r5, ror #28
    864c:	0d0a6325 	stceq	3, cr6, [sl, #-148]	; 0xffffff6c
    8650:	00000000 	andeq	r0, r0, r0
    8654:	3a732525 	bcc	1cd1af0 <CSWTCH.20+0x1cc9360>
    8658:	65694420 	strbvs	r4, [r9, #-1056]!	; 0xfffffbe0
    865c:	6c655720 	stclvs	7, cr5, [r5], #-128	; 0xffffff80
    8660:	73252074 			; <UNDEFINED> instruction: 0x73252074
    8664:	00000000 	andeq	r0, r0, r0
    8668:	20747369 	rsbscs	r7, r4, r9, ror #6
    866c:	c3686373 	cmngt	r8, #-872415231	; 0xcc000001
    8670:	0a216eb6 	beq	864150 <CSWTCH.20+0x85b9c0>
    8674:	0000000d 	andeq	r0, r0, sp
    8678:	3a782525 	bcc	1e11b14 <CSWTCH.20+0x1e09384>
    867c:	30373420 	eorscc	r3, r7, r0, lsr #8
    8680:	68203639 	stmdavs	r0!, {r0, r3, r4, r5, r9, sl, ip, sp}
    8684:	64617865 	strbtvs	r7, [r1], #-2149	; 0xfffff79b
    8688:	6d697a65 	vstmdbvs	r9!, {s15-s115}
    868c:	67206c61 	strvs	r6, [r0, -r1, ror #24]!
    8690:	6369656c 	cmnvs	r9, #108, 10	; 0x1b000000
    8694:	78252068 	stmdavc	r5!, {r3, r5, r6, sp}
    8698:	00000d0a 	andeq	r0, r0, sl, lsl #26
    869c:	3a692525 	bcc	1a51b38 <CSWTCH.20+0x1a493a8>
    86a0:	656c6b20 	strbvs	r6, [ip, #-2848]!	; 0xfffff4e0
    86a4:	74736e69 	ldrbtvc	r6, [r3], #-3689	; 0xfffff197
    86a8:	656e2065 	strbvs	r2, [lr, #-101]!	; 0xffffff9b
    86ac:	69746167 	ldmdbvs	r4!, {r0, r1, r2, r5, r6, r8, sp, lr}^
    86b0:	5a206576 	bpl	821c90 <CSWTCH.20+0x819500>
    86b4:	3a6c6861 	bcc	1b22840 <CSWTCH.20+0x1b1a0b0>
    86b8:	20692520 	rsbcs	r2, r9, r0, lsr #10
    86bc:	00000d0a 	andeq	r0, r0, sl, lsl #26
    86c0:	3a692525 	bcc	1a51b5c <CSWTCH.20+0x1a493cc>
    86c4:	6f726720 	svcvs	0x00726720
    86c8:	74737365 	ldrbtvc	r7, [r3], #-869	; 0xfffffc9b
    86cc:	6f702065 	svcvs	0x00702065
    86d0:	69746973 	ldmdbvs	r4!, {r0, r1, r4, r5, r6, r8, fp, sp, lr}^
    86d4:	5a206576 	bpl	821cb4 <CSWTCH.20+0x819524>
    86d8:	3a6c6861 	bcc	1b22864 <CSWTCH.20+0x1b1a0d4>
    86dc:	0a692520 	beq	1a51b64 <CSWTCH.20+0x1a493d4>
    86e0:	0000000d 	andeq	r0, r0, sp
    86e4:	3a752525 	bcc	1d51b80 <CSWTCH.20+0x1d493f0>
    86e8:	6f726720 	svcvs	0x00726720
    86ec:	74737365 	ldrbtvc	r7, [r3], #-869	; 0xfffffc9b
    86f0:	6e752065 	cdpvs	0, 7, cr2, cr5, cr5, {3}
    86f4:	6e676973 	mcrvs	9, 3, r6, cr7, cr3, {3}
    86f8:	49206465 	stmdbmi	r0!, {r0, r2, r5, r6, sl, sp, lr}
    86fc:	203a746e 	eorscs	r7, sl, lr, ror #8
    8700:	0d0a7525 	cfstr32eq	mvfx7, [sl, #-148]	; 0xffffff6c
    8704:	00000000 	andeq	r0, r0, r0
    8708:	3a702525 	bcc	1c11ba4 <CSWTCH.20+0x1c09414>
    870c:	61686320 	cmnvs	r8, r0, lsr #6
    8710:	20612072 	rsbcs	r2, r1, r2, ror r0
    8714:	69666562 	stmdbvs	r6!, {r1, r5, r6, r8, sl, sp, lr}^
    8718:	7465646e 	strbtvc	r6, [r5], #-1134	; 0xfffffb92
    871c:	63697320 	cmnvs	r9, #32, 6	; 0x80000000
    8720:	69682068 	stmdbvs	r8!, {r3, r5, r6, sp}^
    8724:	203a7265 	eorscs	r7, sl, r5, ror #4
    8728:	0d0a7025 	stceq	0, cr7, [sl, #-148]	; 0xffffff6c
    872c:	00000000 	andeq	r0, r0, r0
    8730:	7a74654a 	bvc	1d21c60 <CSWTCH.20+0x1d194d0>
    8734:	72662074 	rsbvc	r2, r6, #116	; 0x74
    8738:	20656965 	rsbcs	r6, r5, r5, ror #18
    873c:	676e6945 	strbvs	r6, [lr, -r5, asr #18]!
    8740:	20656261 	rsbcs	r6, r5, r1, ror #4
    8744:	67b6c36d 	ldrvs	ip, [r6, sp, ror #6]!
    8748:	6863696c 	stmdavs	r3!, {r2, r3, r5, r6, r8, fp, sp, lr}^
    874c:	000d0a3a 	andeq	r0, sp, sl, lsr sl
    8750:	20756f79 	rsbscs	r6, r5, r9, ror pc
    8754:	73657270 	cmnvc	r5, #112, 4
    8758:	3a646573 	bcc	1921d2c <CSWTCH.20+0x191959c>
    875c:	0a632520 	beq	18d1be4 <CSWTCH.20+0x18c9454>
    8760:	0000000d 	andeq	r0, r0, sp
    8764:	00007830 	andeq	r7, r0, r0, lsr r8
    8768:	735f3072 	cmpvc	pc, #114	; 0x72
    876c:	65747379 	ldrbvs	r7, [r4, #-889]!	; 0xfffffc87
    8770:	25203a6d 	strcs	r3, [r0, #-2669]!	; 0xfffff593
    8774:	0d0a2069 	stceq	0, cr2, [sl, #-420]	; 0xfffffe5c
    8778:	00000d0a 	andeq	r0, r0, sl, lsl #26
    877c:	5f333172 	svcpl	0x00333172
    8780:	74737973 	ldrbtvc	r7, [r3], #-2419	; 0xfffff68d
    8784:	203a6d65 	eorscs	r6, sl, r5, ror #26
    8788:	0a206925 	beq	822c24 <CSWTCH.20+0x81a494>
    878c:	000d0a0d 	andeq	r0, sp, sp, lsl #20

Disassembly of section .rodata:

00008790 <CSWTCH.20>:
    8790:	33323130 	teqcc	r2, #48, 2
    8794:	37363534 			; <UNDEFINED> instruction: 0x37363534
    8798:	42413938 	submi	r3, r1, #56, 18	; 0xe0000
    879c:	46454443 	strbmi	r4, [r5], -r3, asr #8

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <CSWTCH.20+0x10c8594>
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

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00003841 	andeq	r3, r0, r1, asr #16
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	0000002e 	andeq	r0, r0, lr, lsr #32
  10:	726f4305 	rsbvc	r4, pc, #335544320	; 0x14000000
  14:	2d786574 	cfldr64cs	mvdx6, [r8, #-464]!	; 0xfffffe30
  18:	06003741 	streq	r3, [r0], -r1, asr #14
  1c:	0841070a 	stmdaeq	r1, {r1, r3, r8, r9, sl}^
  20:	12020901 	andne	r0, r2, #16384	; 0x4000
  24:	15011404 	strne	r1, [r1, #-1028]	; 0xfffffbfc
  28:	18031701 	stmdane	r3, {r0, r8, r9, sl, ip}
  2c:	1e011a01 	vmlane.f32	s2, s2, s2
  30:	2a012202 	bcs	48840 <CSWTCH.20+0x400b0>
  34:	44022c01 	strmi	r2, [r2], #-3073	; 0xfffff3ff
  38:	Address 0x0000000000000038 is out of bounds.

