
./bs-logic/kernel:     file format elf32-littlearm


Disassembly of section .init:

00008000 <_start>:
    8000:	e10f0000 	mrs	r0, CPSR
    8004:	e200001f 	and	r0, r0, #31
    8008:	e3a0101a 	mov	r1, #26
    800c:	e1500001 	cmp	r0, r1
    8010:	0a000001 	beq	801c <_exitHyper>

00008014 <_bsprak>:
    8014:	eb00010f 	bl	8458 <main>
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

0000803c <sent_string>:
    803c:	e92d4010 	push	{r4, lr}
    8040:	e1a04000 	mov	r4, r0
    8044:	e5d00000 	ldrb	r0, [r0]
    8048:	e3500000 	cmp	r0, #0
    804c:	08bd8010 	popeq	{r4, pc}
    8050:	eb0000f7 	bl	8434 <uart_transmit>
    8054:	e5f40001 	ldrb	r0, [r4, #1]!
    8058:	e3500000 	cmp	r0, #0
    805c:	1afffffb 	bne	8050 <sent_string+0x14>
    8060:	e8bd8010 	pop	{r4, pc}

00008064 <conv_to_ASCII>:
    8064:	e350000f 	cmp	r0, #15
    8068:	93083650 	movwls	r3, #34384	; 0x8650
    806c:	93403000 	movtls	r3, #0
    8070:	97d30000 	ldrbls	r0, [r3, r0]
    8074:	83a00021 	movhi	r0, #33	; 0x21
    8078:	e12fff1e 	bx	lr

0000807c <sent_hex>:
    807c:	e3500000 	cmp	r0, #0
    8080:	e92d4030 	push	{r4, r5, lr}
    8084:	e24dd00c 	sub	sp, sp, #12
    8088:	0a000022 	beq	8118 <sent_hex+0x9c>
    808c:	e308c650 	movw	ip, #34384	; 0x8650
    8090:	e3a04000 	mov	r4, #0
    8094:	e340c000 	movt	ip, #0
    8098:	e200300f 	and	r3, r0, #15
    809c:	e2841001 	add	r1, r4, #1
    80a0:	e1b00220 	lsrs	r0, r0, #4
    80a4:	e7dc2003 	ldrb	r2, [ip, r3]
    80a8:	e28d3008 	add	r3, sp, #8
    80ac:	e6a33074 	sxtab	r3, r3, r4
    80b0:	e6ef4071 	uxtb	r4, r1
    80b4:	e5432008 	strb	r2, [r3, #-8]
    80b8:	1afffff6 	bne	8098 <sent_hex+0x1c>
    80bc:	e2444001 	sub	r4, r4, #1
    80c0:	e6ef4074 	uxtb	r4, r4
    80c4:	e308564c 	movw	r5, #34380	; 0x864c
    80c8:	e3a00030 	mov	r0, #48	; 0x30
    80cc:	e3405000 	movt	r5, #0
    80d0:	eb0000d7 	bl	8434 <uart_transmit>
    80d4:	e5f50001 	ldrb	r0, [r5, #1]!
    80d8:	e3500000 	cmp	r0, #0
    80dc:	1afffffb 	bne	80d0 <sent_hex+0x54>
    80e0:	e6af3074 	sxtb	r3, r4
    80e4:	e3530000 	cmp	r3, #0
    80e8:	ba000008 	blt	8110 <sent_hex+0x94>
    80ec:	e28d2008 	add	r2, sp, #8
    80f0:	e2444001 	sub	r4, r4, #1
    80f4:	e0823003 	add	r3, r2, r3
    80f8:	e6ef4074 	uxtb	r4, r4
    80fc:	e5530008 	ldrb	r0, [r3, #-8]
    8100:	eb0000cb 	bl	8434 <uart_transmit>
    8104:	e6af3074 	sxtb	r3, r4
    8108:	e3730001 	cmn	r3, #1
    810c:	1afffff6 	bne	80ec <sent_hex+0x70>
    8110:	e28dd00c 	add	sp, sp, #12
    8114:	e8bd8030 	pop	{r4, r5, pc}
    8118:	e3a040ff 	mov	r4, #255	; 0xff
    811c:	eaffffe8 	b	80c4 <sent_hex+0x48>

00008120 <sent_dez>:
    8120:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    8124:	e3500000 	cmp	r0, #0
    8128:	e24dd010 	sub	sp, sp, #16
    812c:	e3a03000 	mov	r3, #0
    8130:	e58d3004 	str	r3, [sp, #4]
    8134:	e58d3008 	str	r3, [sp, #8]
    8138:	e1cd30bc 	strh	r3, [sp, #12]
    813c:	ba000026 	blt	81dc <sent_dez+0xbc>
    8140:	e1a02000 	mov	r2, r0
    8144:	0a000022 	beq	81d4 <sent_dez+0xb4>
    8148:	e3085650 	movw	r5, #34384	; 0x8650
    814c:	e30ccccd 	movw	ip, #52429	; 0xcccd
    8150:	e3405000 	movt	r5, #0
    8154:	e34ccccc 	movt	ip, #52428	; 0xcccc
    8158:	e3a04000 	mov	r4, #0
    815c:	e3a0800a 	mov	r8, #10
    8160:	e0876c92 	umull	r6, r7, r2, ip
    8164:	e081329c 	umull	r3, r1, ip, r2
    8168:	e1a031a7 	lsr	r3, r7, #3
    816c:	e2846001 	add	r6, r4, #1
    8170:	e0632398 	mls	r3, r8, r3, r2
    8174:	e28d2010 	add	r2, sp, #16
    8178:	e6ef3073 	uxtb	r3, r3
    817c:	e6a2e074 	sxtab	lr, r2, r4
    8180:	e6ef4076 	uxtb	r4, r6
    8184:	e7d53003 	ldrb	r3, [r5, r3]
    8188:	e1b021a1 	lsrs	r2, r1, #3
    818c:	e54e300c 	strb	r3, [lr, #-12]
    8190:	1afffff2 	bne	8160 <sent_dez+0x40>
    8194:	e3500000 	cmp	r0, #0
    8198:	e2444001 	sub	r4, r4, #1
    819c:	e6ef4074 	uxtb	r4, r4
    81a0:	ba000011 	blt	81ec <sent_dez+0xcc>
    81a4:	e6af3074 	sxtb	r3, r4
    81a8:	e3530000 	cmp	r3, #0
    81ac:	ba000008 	blt	81d4 <sent_dez+0xb4>
    81b0:	e28d2010 	add	r2, sp, #16
    81b4:	e2444001 	sub	r4, r4, #1
    81b8:	e0823003 	add	r3, r2, r3
    81bc:	e6ef4074 	uxtb	r4, r4
    81c0:	e553000c 	ldrb	r0, [r3, #-12]
    81c4:	eb00009a 	bl	8434 <uart_transmit>
    81c8:	e6af3074 	sxtb	r3, r4
    81cc:	e3730001 	cmn	r3, #1
    81d0:	1afffff6 	bne	81b0 <sent_dez+0x90>
    81d4:	e28dd010 	add	sp, sp, #16
    81d8:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}
    81dc:	e2602000 	rsb	r2, r0, #0
    81e0:	e1520003 	cmp	r2, r3
    81e4:	03a040ff 	moveq	r4, #255	; 0xff
    81e8:	1affffd6 	bne	8148 <sent_dez+0x28>
    81ec:	e3a0002d 	mov	r0, #45	; 0x2d
    81f0:	eb00008f 	bl	8434 <uart_transmit>
    81f4:	eaffffea 	b	81a4 <sent_dez+0x84>

000081f8 <sent_udez>:
    81f8:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    81fc:	e250c000 	subs	ip, r0, #0
    8200:	e24dd010 	sub	sp, sp, #16
    8204:	e3a01000 	mov	r1, #0
    8208:	e58d1004 	str	r1, [sp, #4]
    820c:	e58d1008 	str	r1, [sp, #8]
    8210:	e1cd10bc 	strh	r1, [sp, #12]
    8214:	0a00001d 	beq	8290 <sent_udez+0x98>
    8218:	e3086650 	movw	r6, #34384	; 0x8650
    821c:	e30c7ccd 	movw	r7, #52429	; 0xcccd
    8220:	e3406000 	movt	r6, #0
    8224:	e34c7ccc 	movt	r7, #52428	; 0xcccc
    8228:	e3a0800a 	mov	r8, #10
    822c:	e0832c97 	umull	r2, r3, r7, ip
    8230:	e6ef4071 	uxtb	r4, r1
    8234:	e28d0010 	add	r0, sp, #16
    8238:	e1a031a3 	lsr	r3, r3, #3
    823c:	e6afe074 	sxtb	lr, r4
    8240:	e062c398 	mls	r2, r8, r3, ip
    8244:	e080500e 	add	r5, r0, lr
    8248:	e253c000 	subs	ip, r3, #0
    824c:	e6ef3072 	uxtb	r3, r2
    8250:	e2811001 	add	r1, r1, #1
    8254:	e7d60003 	ldrb	r0, [r6, r3]
    8258:	e545000c 	strb	r0, [r5, #-12]
    825c:	1afffff2 	bne	822c <sent_udez+0x34>
    8260:	e35e0000 	cmp	lr, #0
    8264:	aa000001 	bge	8270 <sent_udez+0x78>
    8268:	ea000008 	b	8290 <sent_udez+0x98>
    826c:	e552000c 	ldrb	r0, [r2, #-12]
    8270:	e2444001 	sub	r4, r4, #1
    8274:	eb00006e 	bl	8434 <uart_transmit>
    8278:	e6ef4074 	uxtb	r4, r4
    827c:	e28d2010 	add	r2, sp, #16
    8280:	e6af3074 	sxtb	r3, r4
    8284:	e0822003 	add	r2, r2, r3
    8288:	e3530000 	cmp	r3, #0
    828c:	aafffff6 	bge	826c <sent_udez+0x74>
    8290:	e28dd010 	add	sp, sp, #16
    8294:	e8bd81f0 	pop	{r4, r5, r6, r7, r8, pc}

00008298 <kprintf>:
    8298:	e92d000f 	push	{r0, r1, r2, r3}
    829c:	e92d4030 	push	{r4, r5, lr}
    82a0:	e24dd00c 	sub	sp, sp, #12
    82a4:	e28d301c 	add	r3, sp, #28
    82a8:	e59d4018 	ldr	r4, [sp, #24]
    82ac:	e58d3004 	str	r3, [sp, #4]
    82b0:	e5d43000 	ldrb	r3, [r4]
    82b4:	e3530000 	cmp	r3, #0
    82b8:	1a000006 	bne	82d8 <kprintf+0x40>
    82bc:	ea00002c 	b	8374 <kprintf+0xdc>
    82c0:	e1a04005 	mov	r4, r5
    82c4:	e4d40001 	ldrb	r0, [r4], #1
    82c8:	eb000059 	bl	8434 <uart_transmit>
    82cc:	e5d53001 	ldrb	r3, [r5, #1]
    82d0:	e3530000 	cmp	r3, #0
    82d4:	0a000026 	beq	8374 <kprintf+0xdc>
    82d8:	e3530025 	cmp	r3, #37	; 0x25
    82dc:	11a05004 	movne	r5, r4
    82e0:	1afffff6 	bne	82c0 <kprintf+0x28>
    82e4:	e5d40001 	ldrb	r0, [r4, #1]
    82e8:	e2403063 	sub	r3, r0, #99	; 0x63
    82ec:	e3530015 	cmp	r3, #21
    82f0:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
    82f4:	ea00001b 	b	8368 <kprintf+0xd0>
    82f8:	000083fc 	strdeq	r8, [r0], -ip
    82fc:	00008368 	andeq	r8, r0, r8, ror #6
    8300:	00008368 	andeq	r8, r0, r8, ror #6
    8304:	00008368 	andeq	r8, r0, r8, ror #6
    8308:	00008368 	andeq	r8, r0, r8, ror #6
    830c:	00008368 	andeq	r8, r0, r8, ror #6
    8310:	000083e4 	andeq	r8, r0, r4, ror #7
    8314:	00008368 	andeq	r8, r0, r8, ror #6
    8318:	00008368 	andeq	r8, r0, r8, ror #6
    831c:	00008368 	andeq	r8, r0, r8, ror #6
    8320:	00008368 	andeq	r8, r0, r8, ror #6
    8324:	00008368 	andeq	r8, r0, r8, ror #6
    8328:	00008368 	andeq	r8, r0, r8, ror #6
    832c:	00008350 	andeq	r8, r0, r0, asr r3
    8330:	00008368 	andeq	r8, r0, r8, ror #6
    8334:	00008368 	andeq	r8, r0, r8, ror #6
    8338:	000083b4 			; <UNDEFINED> instruction: 0x000083b4
    833c:	00008368 	andeq	r8, r0, r8, ror #6
    8340:	0000839c 	muleq	r0, ip, r3
    8344:	00008368 	andeq	r8, r0, r8, ror #6
    8348:	00008368 	andeq	r8, r0, r8, ror #6
    834c:	00008384 	andeq	r8, r0, r4, lsl #7
    8350:	e59d3004 	ldr	r3, [sp, #4]
    8354:	e5930000 	ldr	r0, [r3]
    8358:	e2833004 	add	r3, r3, #4
    835c:	e58d3004 	str	r3, [sp, #4]
    8360:	ebffff45 	bl	807c <sent_hex>
    8364:	e5d40001 	ldrb	r0, [r4, #1]
    8368:	eb000031 	bl	8434 <uart_transmit>
    836c:	e2845002 	add	r5, r4, #2
    8370:	eaffffd2 	b	82c0 <kprintf+0x28>
    8374:	e28dd00c 	add	sp, sp, #12
    8378:	e8bd4030 	pop	{r4, r5, lr}
    837c:	e28dd010 	add	sp, sp, #16
    8380:	e12fff1e 	bx	lr
    8384:	e59d3004 	ldr	r3, [sp, #4]
    8388:	e5930000 	ldr	r0, [r3]
    838c:	e2833004 	add	r3, r3, #4
    8390:	e58d3004 	str	r3, [sp, #4]
    8394:	ebffff38 	bl	807c <sent_hex>
    8398:	eafffff3 	b	836c <kprintf+0xd4>
    839c:	e59d3004 	ldr	r3, [sp, #4]
    83a0:	e5930000 	ldr	r0, [r3]
    83a4:	e2833004 	add	r3, r3, #4
    83a8:	e58d3004 	str	r3, [sp, #4]
    83ac:	ebffff91 	bl	81f8 <sent_udez>
    83b0:	eaffffed 	b	836c <kprintf+0xd4>
    83b4:	e59d3004 	ldr	r3, [sp, #4]
    83b8:	e5935000 	ldr	r5, [r3]
    83bc:	e2833004 	add	r3, r3, #4
    83c0:	e58d3004 	str	r3, [sp, #4]
    83c4:	e5d50000 	ldrb	r0, [r5]
    83c8:	e3500000 	cmp	r0, #0
    83cc:	0affffe6 	beq	836c <kprintf+0xd4>
    83d0:	eb000017 	bl	8434 <uart_transmit>
    83d4:	e5f50001 	ldrb	r0, [r5, #1]!
    83d8:	e3500000 	cmp	r0, #0
    83dc:	1afffffb 	bne	83d0 <kprintf+0x138>
    83e0:	eaffffe1 	b	836c <kprintf+0xd4>
    83e4:	e59d3004 	ldr	r3, [sp, #4]
    83e8:	e5930000 	ldr	r0, [r3]
    83ec:	e2833004 	add	r3, r3, #4
    83f0:	e58d3004 	str	r3, [sp, #4]
    83f4:	ebffff49 	bl	8120 <sent_dez>
    83f8:	eaffffdb 	b	836c <kprintf+0xd4>
    83fc:	e59d3004 	ldr	r3, [sp, #4]
    8400:	e5d30000 	ldrb	r0, [r3]
    8404:	e2833004 	add	r3, r3, #4
    8408:	e58d3004 	str	r3, [sp, #4]
    840c:	eb000008 	bl	8434 <uart_transmit>
    8410:	eaffffd5 	b	836c <kprintf+0xd4>

00008414 <uart_receive>:
    8414:	e3a03a01 	mov	r3, #4096	; 0x1000
    8418:	e3433f20 	movt	r3, #16160	; 0x3f20
    841c:	e5932018 	ldr	r2, [r3, #24]
    8420:	e3120010 	tst	r2, #16
    8424:	05930000 	ldreq	r0, [r3]
    8428:	06ef0070 	uxtbeq	r0, r0
    842c:	13a00000 	movne	r0, #0
    8430:	e12fff1e 	bx	lr

00008434 <uart_transmit>:
    8434:	e3a01a01 	mov	r1, #4096	; 0x1000
    8438:	e3431f20 	movt	r1, #16160	; 0x3f20
    843c:	e5912018 	ldr	r2, [r1, #24]
    8440:	e3a03a01 	mov	r3, #4096	; 0x1000
    8444:	e3433f20 	movt	r3, #16160	; 0x3f20
    8448:	e3120020 	tst	r2, #32
    844c:	1afffffa 	bne	843c <uart_transmit+0x8>
    8450:	e5830000 	str	r0, [r3]
    8454:	e12fff1e 	bx	lr

Disassembly of section .text.startup:

00008458 <main>:
    8458:	e3080510 	movw	r0, #34064	; 0x8510
    845c:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    8460:	e3400000 	movt	r0, #0
    8464:	e24dd00c 	sub	sp, sp, #12
    8468:	ebffff8a 	bl	8298 <kprintf>
    846c:	e308051c 	movw	r0, #34076	; 0x851c
    8470:	e3a01062 	mov	r1, #98	; 0x62
    8474:	e3400000 	movt	r0, #0
    8478:	ebffff86 	bl	8298 <kprintf>
    847c:	e308053c 	movw	r0, #34108	; 0x853c
    8480:	e3081550 	movw	r1, #34128	; 0x8550
    8484:	e3400000 	movt	r0, #0
    8488:	e3401000 	movt	r1, #0
    848c:	ebffff81 	bl	8298 <kprintf>
    8490:	e3080560 	movw	r0, #34144	; 0x8560
    8494:	e30b17f8 	movw	r1, #47096	; 0xb7f8
    8498:	e3400000 	movt	r0, #0
    849c:	ebffff7d 	bl	8298 <kprintf>
    84a0:	e3080584 	movw	r0, #34180	; 0x8584
    84a4:	e3a01102 	mov	r1, #-2147483648	; 0x80000000
    84a8:	e3400000 	movt	r0, #0
    84ac:	ebffff79 	bl	8298 <kprintf>
    84b0:	e30805a8 	movw	r0, #34216	; 0x85a8
    84b4:	e3e01102 	mvn	r1, #-2147483648	; 0x80000000
    84b8:	e3400000 	movt	r0, #0
    84bc:	ebffff75 	bl	8298 <kprintf>
    84c0:	e30805cc 	movw	r0, #34252	; 0x85cc
    84c4:	e3e01000 	mvn	r1, #0
    84c8:	e3400000 	movt	r0, #0
    84cc:	ebffff71 	bl	8298 <kprintf>
    84d0:	e28d1008 	add	r1, sp, #8
    84d4:	e3a03034 	mov	r3, #52	; 0x34
    84d8:	e30805f0 	movw	r0, #34288	; 0x85f0
    84dc:	e5613001 	strb	r3, [r1, #-1]!
    84e0:	e3400000 	movt	r0, #0
    84e4:	ebffff6b 	bl	8298 <kprintf>
    84e8:	e3080618 	movw	r0, #34328	; 0x8618
    84ec:	e3400000 	movt	r0, #0
    84f0:	ebffff68 	bl	8298 <kprintf>
    84f4:	ebffffc6 	bl	8414 <uart_receive>
    84f8:	e2501000 	subs	r1, r0, #0
    84fc:	0afffffc 	beq	84f4 <main+0x9c>
    8500:	e3080638 	movw	r0, #34360	; 0x8638
    8504:	e3400000 	movt	r0, #0
    8508:	ebffff62 	bl	8298 <kprintf>
    850c:	eafffff8 	b	84f4 <main+0x9c>

Disassembly of section .rodata.str1.4:

00008510 <.rodata.str1.4>:
    8510:	74736554 	ldrbtvc	r6, [r3], #-1364	; 0xfffffaac
    8514:	69676562 	stmdbvs	r7!, {r1, r5, r6, r8, sl, sp, lr}^
    8518:	000d0a6e 	andeq	r0, sp, lr, ror #20
    851c:	3a632525 	bcc	18d19b8 <CSWTCH.20+0x18c9368>
    8520:	77206220 	strvc	r6, [r0, -r0, lsr #4]!
    8524:	20647269 	rsbcs	r7, r4, r9, ror #4
    8528:	67737561 	ldrbvs	r7, [r3, -r1, ror #10]!
    852c:	62656765 	rsbvs	r6, r5, #26476544	; 0x1940000
    8530:	203a6e65 	eorscs	r6, sl, r5, ror #28
    8534:	0d0a6325 	stceq	3, cr6, [sl, #-148]	; 0xffffff6c
    8538:	00000000 	andeq	r0, r0, r0
    853c:	3a732525 	bcc	1cd19d8 <CSWTCH.20+0x1cc9388>
    8540:	65694420 	strbvs	r4, [r9, #-1056]!	; 0xfffffbe0
    8544:	6c655720 	stclvs	7, cr5, [r5], #-128	; 0xffffff80
    8548:	73252074 			; <UNDEFINED> instruction: 0x73252074
    854c:	00000000 	andeq	r0, r0, r0
    8550:	20747369 	rsbscs	r7, r4, r9, ror #6
    8554:	c3686373 	cmngt	r8, #-872415231	; 0xcc000001
    8558:	0a216eb6 	beq	864038 <CSWTCH.20+0x85b9e8>
    855c:	0000000d 	andeq	r0, r0, sp
    8560:	3a782525 	bcc	1e119fc <CSWTCH.20+0x1e093ac>
    8564:	30373420 	eorscc	r3, r7, r0, lsr #8
    8568:	68203639 	stmdavs	r0!, {r0, r3, r4, r5, r9, sl, ip, sp}
    856c:	64617865 	strbtvs	r7, [r1], #-2149	; 0xfffff79b
    8570:	6d697a65 	vstmdbvs	r9!, {s15-s115}
    8574:	67206c61 	strvs	r6, [r0, -r1, ror #24]!
    8578:	6369656c 	cmnvs	r9, #108, 10	; 0x1b000000
    857c:	78252068 	stmdavc	r5!, {r3, r5, r6, sp}
    8580:	00000d0a 	andeq	r0, r0, sl, lsl #26
    8584:	3a692525 	bcc	1a51a20 <CSWTCH.20+0x1a493d0>
    8588:	656c6b20 	strbvs	r6, [ip, #-2848]!	; 0xfffff4e0
    858c:	74736e69 	ldrbtvc	r6, [r3], #-3689	; 0xfffff197
    8590:	656e2065 	strbvs	r2, [lr, #-101]!	; 0xffffff9b
    8594:	69746167 	ldmdbvs	r4!, {r0, r1, r2, r5, r6, r8, sp, lr}^
    8598:	5a206576 	bpl	821b78 <CSWTCH.20+0x819528>
    859c:	3a6c6861 	bcc	1b22728 <CSWTCH.20+0x1b1a0d8>
    85a0:	20692520 	rsbcs	r2, r9, r0, lsr #10
    85a4:	00000d0a 	andeq	r0, r0, sl, lsl #26
    85a8:	3a692525 	bcc	1a51a44 <CSWTCH.20+0x1a493f4>
    85ac:	6f726720 	svcvs	0x00726720
    85b0:	74737365 	ldrbtvc	r7, [r3], #-869	; 0xfffffc9b
    85b4:	6f702065 	svcvs	0x00702065
    85b8:	69746973 	ldmdbvs	r4!, {r0, r1, r4, r5, r6, r8, fp, sp, lr}^
    85bc:	5a206576 	bpl	821b9c <CSWTCH.20+0x81954c>
    85c0:	3a6c6861 	bcc	1b2274c <CSWTCH.20+0x1b1a0fc>
    85c4:	0a692520 	beq	1a51a4c <CSWTCH.20+0x1a493fc>
    85c8:	0000000d 	andeq	r0, r0, sp
    85cc:	3a752525 	bcc	1d51a68 <CSWTCH.20+0x1d49418>
    85d0:	6f726720 	svcvs	0x00726720
    85d4:	74737365 	ldrbtvc	r7, [r3], #-869	; 0xfffffc9b
    85d8:	6e752065 	cdpvs	0, 7, cr2, cr5, cr5, {3}
    85dc:	6e676973 	mcrvs	9, 3, r6, cr7, cr3, {3}
    85e0:	49206465 	stmdbmi	r0!, {r0, r2, r5, r6, sl, sp, lr}
    85e4:	203a746e 	eorscs	r7, sl, lr, ror #8
    85e8:	0d0a7525 	cfstr32eq	mvfx7, [sl, #-148]	; 0xffffff6c
    85ec:	00000000 	andeq	r0, r0, r0
    85f0:	3a702525 	bcc	1c11a8c <CSWTCH.20+0x1c0943c>
    85f4:	61686320 	cmnvs	r8, r0, lsr #6
    85f8:	20612072 	rsbcs	r2, r1, r2, ror r0
    85fc:	69666562 	stmdbvs	r6!, {r1, r5, r6, r8, sl, sp, lr}^
    8600:	7465646e 	strbtvc	r6, [r5], #-1134	; 0xfffffb92
    8604:	63697320 	cmnvs	r9, #32, 6	; 0x80000000
    8608:	69682068 	stmdbvs	r8!, {r3, r5, r6, sp}^
    860c:	203a7265 	eorscs	r7, sl, r5, ror #4
    8610:	0d0a7025 	stceq	0, cr7, [sl, #-148]	; 0xffffff6c
    8614:	00000000 	andeq	r0, r0, r0
    8618:	7a74654a 	bvc	1d21b48 <CSWTCH.20+0x1d194f8>
    861c:	72662074 	rsbvc	r2, r6, #116	; 0x74
    8620:	20656965 	rsbcs	r6, r5, r5, ror #18
    8624:	676e6945 	strbvs	r6, [lr, -r5, asr #18]!
    8628:	20656261 	rsbcs	r6, r5, r1, ror #4
    862c:	67b6c36d 	ldrvs	ip, [r6, sp, ror #6]!
    8630:	6863696c 	stmdavs	r3!, {r2, r3, r5, r6, r8, fp, sp, lr}^
    8634:	000d0a3a 	andeq	r0, sp, sl, lsr sl
    8638:	20756f79 	rsbscs	r6, r5, r9, ror pc
    863c:	73657270 	cmnvc	r5, #112, 4
    8640:	3a646573 	bcc	1921c14 <CSWTCH.20+0x19195c4>
    8644:	0a632520 	beq	18d1acc <CSWTCH.20+0x18c947c>
    8648:	0000000d 	andeq	r0, r0, sp
    864c:	00007830 	andeq	r7, r0, r0, lsr r8

Disassembly of section .rodata:

00008650 <CSWTCH.20>:
    8650:	33323130 	teqcc	r2, #48, 2
    8654:	37363534 			; <UNDEFINED> instruction: 0x37363534
    8658:	42413938 	submi	r3, r1, #56, 18	; 0xe0000
    865c:	46454443 	strbmi	r4, [r5], -r3, asr #8

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <CSWTCH.20+0x10c86d4>
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
  30:	2a012202 	bcs	48840 <CSWTCH.20+0x401f0>
  34:	44022c01 	strmi	r2, [r2], #-3073	; 0xfffff3ff
  38:	Address 0x0000000000000038 is out of bounds.

