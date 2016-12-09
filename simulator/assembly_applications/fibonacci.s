;
; File generated by cc65 v 2.15
;
	.fopt		compiler,"cc65 v 2.15"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.export		_fibonacci

; ---------------------------------------------------------------
; void __near__ fibonacci (int *)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_fibonacci: near

.segment	"CODE"

	jsr     pushax
	ldx     #$00
	lda     #$00
	jsr     pushax
	ldx     #$00
	lda     #$01
	jsr     pushax
	jsr     decsp4
	ldx     #$00
	lda     #$00
	ldy     #$00
	jsr     staxysp
L0004:	ldy     #$01
	jsr     ldaxysp
	cmp     #$0A
	txa
	sbc     #$00
	bvc     L000B
	eor     #$80
L000B:	asl     a
	lda     #$00
	ldx     #$00
	rol     a
	jne     L0007
	jmp     L0005
L0007:	ldy     #$01
	jsr     ldaxysp
	cmp     #$02
	txa
	sbc     #$00
	bvc     L000F
	eor     #$80
L000F:	asl     a
	lda     #$00
	ldx     #$00
	rol     a
	jeq     L000D
	ldy     #$01
	jsr     ldaxysp
	ldy     #$02
	jsr     staxysp
	ldy     #$09
	jsr     ldaxysp
	jsr     pushax
	ldy     #$03
	jsr     ldaxysp
	jsr     aslax1
	jsr     tosaddax
	jsr     pushax
	ldy     #$05
	jsr     ldaxysp
	ldy     #$00
	jsr     staxspidx
	jmp     L0006
L000D:	ldy     #$07
	jsr     ldaxysp
	jsr     pushax
	ldy     #$07
	jsr     ldaxysp
	jsr     tosaddax
	ldy     #$02
	jsr     staxysp
	ldy     #$05
	jsr     ldaxysp
	ldy     #$06
	jsr     staxysp
	ldy     #$03
	jsr     ldaxysp
	ldy     #$04
	jsr     staxysp
	ldy     #$09
	jsr     ldaxysp
	jsr     pushax
	ldy     #$03
	jsr     ldaxysp
	jsr     aslax1
	jsr     tosaddax
	jsr     pushax
	ldy     #$05
	jsr     ldaxysp
	ldy     #$00
	jsr     staxspidx
L0006:	ldy     #$01
	jsr     ldaxysp
	sta     regsave
	stx     regsave+1
	jsr     incax1
	ldy     #$00
	jsr     staxysp
	lda     regsave
	ldx     regsave+1
	jmp     L0004
L0005:	ldy     #$0A
	jsr     addysp
	rts

.endproc
