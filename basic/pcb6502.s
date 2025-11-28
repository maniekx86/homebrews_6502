.segment "IOHANDLER"
.org $FC00


IO_CTL = $B003
PORT_C = $B002
PORT_B = $B001

SCREEN_DATA = $8000
SCREEN_REGSEL = $8001

KBBUFPOS = $7FC0
KBREADPOS = $7FC1
KBMEM = $7FC2
KB_ISSHIFT = $7FF0
KB_ISCTRL = $7FF1
KB_NEXTRELEASE = $7FF2
KB_RETKEY = $7FF3 ; to obey the stack limitation for MONREAD 
KB_BREAK = $7FF4

SCREEN_TMPCHR = $7FFD
SCREEN_X = $7FFE
SCREEN_Y = $7FFF
; stolen from https://github.com/tedkotz/be6502/blob/master/crt/pckybd.s
asciitbl:      .byte $00               ; 00 no key pressed
               .byte $89               ; 01 F9
               .byte $87               ; 02 relocated F7
               .byte $85               ; 03 F5
               .byte $83               ; 04 F3
               .byte $81               ; 05 F1
               .byte $82               ; 06 F2
               .byte $8C               ; 07 F12
               .byte $00               ; 08
               .byte $8A               ; 09 F10
               .byte $88               ; 0A F8
               .byte $86               ; 0B F6
               .byte $84               ; 0C F4
               .byte $09               ; 0D tab
               .byte $60               ; 0E `~
               .byte $8F               ; 0F relocated Print Screen key
               .byte $03               ; 10 relocated Pause/Break key
               .byte $A0               ; 11 left alt (right alt too)
               .byte $00               ; 12 left shift
               .byte $E0               ; 13 relocated Alt release code
               .byte $00               ; 14 left ctrl (right ctrl too)
               .byte $71               ; 15 qQ
               .byte $31               ; 16 1!
               .byte $00               ; 17
               .byte $00               ; 18
               .byte $00               ; 19
               .byte $7A               ; 1A zZ
               .byte $73               ; 1B sS
               .byte $61               ; 1C aA
               .byte $77               ; 1D wW
               .byte $32               ; 1E 2@
               .byte $A1               ; 1F Windows 98 menu key (left side)
               .byte $02               ; 20 relocated ctrl-break key
               .byte $63               ; 21 cC
               .byte $78               ; 22 xX
               .byte $64               ; 23 dD
               .byte $65               ; 24 eE
               .byte $34               ; 25 4$
               .byte $33               ; 26 3#
               .byte $A2               ; 27 Windows 98 menu key (right side)
               .byte $00               ; 28
               .byte $20               ; 29 space
               .byte $76               ; 2A vV
               .byte $66               ; 2B fF
               .byte $74               ; 2C tT
               .byte $72               ; 2D rR
               .byte $35               ; 2E 5%
               .byte $A3               ; 2F Windows 98 option key (right click, right side)
               .byte $00               ; 30
               .byte $6E               ; 31 nN
               .byte $62               ; 32 bB
               .byte $68               ; 33 hH
               .byte $67               ; 34 gG
               .byte $79               ; 35 yY
               .byte $36               ; 36 6^
               .byte $00               ; 37
               .byte $00               ; 38
               .byte $00               ; 39
               .byte $6D               ; 3A mM
               .byte $6A               ; 3B jJ
               .byte $75               ; 3C uU
               .byte $37               ; 3D 7&
               .byte $38               ; 3E 8*
               .byte $00               ; 3F
               .byte $00               ; 40
               .byte $2C               ; 41 ,<
               .byte $6B               ; 42 kK
               .byte $69               ; 43 iI
               .byte $6F               ; 44 oO
               .byte $30               ; 45 0)
               .byte $39               ; 46 9(
               .byte $00               ; 47
               .byte $00               ; 48
               .byte $2E               ; 49 .>
               .byte $2F               ; 4A /?
               .byte $6C               ; 4B lL
               .byte $3B               ; 4C ;:
               .byte $70               ; 4D pP
               .byte $2D               ; 4E -_
               .byte $00               ; 4F
               .byte $00               ; 50
               .byte $00               ; 51
               .byte $27               ; 52 '"
               .byte $00               ; 53
               .byte $5B               ; 54 [{
               .byte $3D               ; 55 =+
               .byte $00               ; 56
               .byte $00               ; 57
               .byte $00               ; 58 caps
               .byte $00               ; 59 r shift
               .byte $0D               ; 5A <Enter>
               .byte $5D               ; 5B ]}
               .byte $00               ; 5C
               .byte $5C               ; 5D \|
               .byte $00               ; 5E
               .byte $00               ; 5F
               .byte $00               ; 60
               .byte $00               ; 61
               .byte $00               ; 62
               .byte $00               ; 63
               .byte $00               ; 64
               .byte $00               ; 65
               .byte $08               ; 66 bkspace
               .byte $00               ; 67
               .byte $00               ; 68
               .byte $31               ; 69 kp 1
               .byte $2f               ; 6A kp / converted from E04A in code
               .byte $34               ; 6B kp 4
               .byte $37               ; 6C kp 7
               .byte $00               ; 6D
               .byte $00               ; 6E
               .byte $00               ; 6F
               .byte $30               ; 70 kp 0
               .byte $2E               ; 71 kp .
               .byte $32               ; 72 kp 2
               .byte $35               ; 73 kp 5
               .byte $36               ; 74 kp 6
               .byte $38               ; 75 kp 8
               .byte $1B               ; 76 esc
               .byte $00               ; 77 num lock
               .byte $8B               ; 78 F11
               .byte $2B               ; 79 kp +
               .byte $33               ; 7A kp 3
               .byte $2D               ; 7B kp -
               .byte $2A               ; 7C kp *
               .byte $39               ; 7D kp 9
               .byte $8D               ; 7E scroll lock
               .byte $00               ; 7F
;
; Table for shifted scancodes
;
asciitbl_shift:
               .byte $00               ; 80
               .byte $C9               ; 81 F9
               .byte $C7               ; 82 relocated F7
               .byte $C5               ; 83 F5 (F7 actual scancode=83)
               .byte $C3               ; 84 F3
               .byte $C1               ; 85 F1
               .byte $C2               ; 86 F2
               .byte $CC               ; 87 F12
               .byte $00               ; 88
               .byte $CA               ; 89 F10
               .byte $C8               ; 8A F8
               .byte $C6               ; 8B F6
               .byte $C4               ; 8C F4
               .byte $09               ; 8D tab
               .byte $7E               ; 8E `~
               .byte $CF               ; 8F relocated Print Screen key
               .byte $03               ; 90 relocated Pause/Break key
               .byte $A0               ; 91 left alt (right alt)
               .byte $00               ; 92 left shift
               .byte $E0               ; 93 relocated Alt release code
               .byte $00               ; 94 left ctrl (and right ctrl)
               .byte $51               ; 95 qQ
               .byte $21               ; 96 1!
               .byte $00               ; 97
               .byte $00               ; 98
               .byte $00               ; 99
               .byte $5A               ; 9A zZ
               .byte $53               ; 9B sS
               .byte $41               ; 9C aA
               .byte $57               ; 9D wW
               .byte $40               ; 9E 2@
               .byte $E1               ; 9F Windows 98 menu key (left side)
               .byte $02               ; A0 relocated ctrl-break key
               .byte $43               ; A1 cC
               .byte $58               ; A2 xX
               .byte $44               ; A3 dD
               .byte $45               ; A4 eE
               .byte $24               ; A5 4$
               .byte $23               ; A6 3#
               .byte $E2               ; A7 Windows 98 menu key (right side)
               .byte $00               ; A8
               .byte $20               ; A9 space
               .byte $56               ; AA vV
               .byte $46               ; AB fF
               .byte $54               ; AC tT
               .byte $52               ; AD rR
               .byte $25               ; AE 5%
               .byte $E3               ; AF Windows 98 option key (right click, right side)
               .byte $00               ; B0
               .byte $4E               ; B1 nN
               .byte $42               ; B2 bB
               .byte $48               ; B3 hH
               .byte $47               ; B4 gG
               .byte $59               ; B5 yY
               .byte $5E               ; B6 6^
               .byte $00               ; B7
               .byte $00               ; B8
               .byte $00               ; B9
               .byte $4D               ; BA mM
               .byte $4A               ; BB jJ
               .byte $55               ; BC uU
               .byte $26               ; BD 7&
               .byte $2A               ; BE 8*
               .byte $00               ; BF
               .byte $00               ; C0
               .byte $3C               ; C1 ,<
               .byte $4B               ; C2 kK
               .byte $49               ; C3 iI
               .byte $4F               ; C4 oO
               .byte $29               ; C5 0)
               .byte $28               ; C6 9(
               .byte $00               ; C7
               .byte $00               ; C8
               .byte $3E               ; C9 .>
               .byte $3F               ; CA /?
               .byte $4C               ; CB lL
               .byte $3A               ; CC ;:
               .byte $50               ; CD pP
               .byte $5F               ; CE -_
               .byte $00               ; CF
               .byte $00               ; D0
               .byte $00               ; D1
               .byte $22               ; D2 '"
               .byte $00               ; D3
               .byte $7B               ; D4 [{
               .byte $2B               ; D5 =+
               .byte $00               ; D6
               .byte $00               ; D7
               .byte $00               ; D8 caps
               .byte $00               ; D9 r shift
               .byte $0D               ; DA <Enter>
               .byte $7D               ; DB ]}
               .byte $00               ; DC
               .byte $7C               ; DD \|
               .byte $00               ; DE
               .byte $00               ; DF
               .byte $00               ; E0
               .byte $00               ; E1
               .byte $00               ; E2
               .byte $00               ; E3
               .byte $00               ; E4
               .byte $00               ; E5
               .byte $08               ; E6 bkspace
               .byte $00               ; E7
               .byte $00               ; E8
               .byte $91               ; E9 kp 1
               .byte $2f               ; EA kp / converted from E04A in code
               .byte $94               ; EB kp 4
               .byte $97               ; EC kp 7
               .byte $00               ; ED
               .byte $00               ; EE
               .byte $00               ; EF
               .byte $90               ; F0 kp 0
               .byte $7F               ; F1 kp .
               .byte $92               ; F2 kp 2
               .byte $95               ; F3 kp 5
               .byte $96               ; F4 kp 6
               .byte $98               ; F5 kp 8
               .byte $1B               ; F6 esc
               .byte $00               ; F7 num lock
               .byte $CB               ; F8 F11
               .byte $2B               ; F9 kp +
               .byte $93               ; FA kp 3
               .byte $2D               ; FB kp -
               .byte $2A               ; FC kp *
               .byte $99               ; FD kp 9
               .byte $CD               ; FE scroll lock
; NOT USED     .byte $00               ; FF
; end

Reset:
	LDX     #STACK_TOP
	TXS
	LDA #%10001001  ; port C input, ports A and B output
    STA IO_CTL 
    LDA #%00000001 ; lit the led up
    STA PORT_B  
    LDA #$00
    STA KBBUFPOS
    STA KBREADPOS
    STA KB_ISSHIFT
    STA KB_ISCTRL
    STA KB_NEXTRELEASE
    STA SCREEN_X
    STA SCREEN_Y
    STA KB_BREAK
    
    LDA #$01 ; set cursor to 0,0
    STA SCREEN_REGSEL 
    LDA #$00
    STA SCREEN_DATA
    LDA #$02
    STA SCREEN_REGSEL 
    LDA #$00
    STA SCREEN_DATA
    
    

; Display startup message
	LDY #0
ShowStartMsg:
	LDA	StartupMessage,Y
	
	BEQ	WaitForKeypress
	JSR	MONCOUT
	INY
	BNE	ShowStartMsg
	

; Wait for a cold/warm start selection
WaitForKeypress:
	JSR	MONRDKEY
	BCC	WaitForKeypress
	
	CMP	#'w'			; compare with [W]arm start
	BEQ	WarmStart

	CMP	#'c'			; compare with [C]old start
	BNE	Reset

	JMP	COLD_START	; BASIC cold start

WarmStart:
	JMP	RESTART		; BASIC warm start

MONCOUT: ; char is in A
    STA SCREEN_TMPCHR ; save char
    
    PHA
    TXA
    PHA
    TYA
    PHA
    
    LDA SCREEN_TMPCHR
    CMP #$08 ; backspace
    BNE skip_backspace 
    
    dec SCREEN_X
    LDA #$01 ; set to X register
    STA SCREEN_REGSEL
    LDA SCREEN_X ; set X
    STA SCREEN_DATA
    LDA #$00 ; print null to clear next char
    STA SCREEN_REGSEL
    STA SCREEN_DATA
    ; again move cursor back
    LDA #$01 ; set to X register
    STA SCREEN_REGSEL
    LDA SCREEN_X ; set X
    STA SCREEN_DATA
    
    jmp skip_print
    skip_backspace:
    
    LDA #$0         ; print char (set reg 0 and print)
    STA SCREEN_REGSEL
    LDA SCREEN_TMPCHR
    STA SCREEN_DATA
    CMP #$A ; new line
    BNE skip_newline
    
    INC SCREEN_Y
    LDA SCREEN_Y
    CMP #59
    BNE skip_backY
    DEC SCREEN_Y
    skip_backY:
    LDA #$00
    STA SCREEN_X
    
    dec SCREEN_X
    skip_newline:
    
    inc SCREEN_X
    
    skip_print:
    
    PLA
    TAY
    PLA
    TAX
    PLA
    
	RTS

MONRDKEY: ; we need to return in A register a ascii key
    TYA
    PHA
    TXA
    PHA
    

    LDA KBREADPOS
	CMP KBBUFPOS
	BEQ	NoDataIn
	
	
	LDA KBREADPOS
	TAX
	LDA KBMEM, X
	STA KB_RETKEY
	INC KBREADPOS
	LDA KBREADPOS
	
	cmp #32 ; rotational buffer
    bne skipSetToZero  ; set to 0 then to cycle over

    lda #$0
    sta KBREADPOS
    
    skipSetToZero:
    
    TYA
    SEC		; Carry set if key available
	
	; get the key
	PLA
	TAX
	
	PLA
	TAY
	
	LDA KB_RETKEY

	RTS
NoDataIn:
	CLC		; Carry clear if no key pressed
    PLA
	TAX
	PLA
	TAY
	RTS

MONISCNTC:
    PHA
    
    LDA KB_BREAK
    CMP #$FF
    BNE NotCTRLC

	SEC		; Carry set if control C pressed
	RTS
NotCTRLC:
    LDA #$00
    STA KB_BREAK
    
    PLA
	CLC		; Carry clear if control C not pressed
	RTS

KBHandle:
    PHA
    TXA
    TSX
    PHA
    TYA
    PHA
    ldy PORT_C
    tya
    cmp #$E0 ; if not E0 dont skip
    bne dontSkipFuncs
    jmp skipFuncs
    
    dontSkipFuncs:
    
    cmp #$F0 
    bne next_not_Release ; keyboard_data!=$F0
    
    lda #$FF            ; set KB_NEXTRELEASE to 1 when keyboard $F0
    sta KB_NEXTRELEASE
    jmp skipFuncs ; skip when $F0
    
    next_not_Release:   ; not $F0
    
    ; update SHIFT or CTRL status based on KB_NEXTRELEASE 
    
    ; we still in Y have data
    tya
    
    cmp #$12
    beq update_shift
    
    cmp #$14
    beq update_ctrl
    
    jmp release_if_end
    
    
    update_shift:
    lda KB_NEXTRELEASE
    cmp #$FF
    beq release_shift
    ; press shift
    lda #$FF
    sta KB_ISSHIFT
    jmp end_of_shift_ctrl_check
    
    
    update_ctrl:
    lda KB_NEXTRELEASE
    cmp #$FF
    beq release_ctrl
    ; press shift
    lda #$FF
    sta KB_ISCTRL
    jmp end_of_shift_ctrl_check
    
    
    release_shift:
    lda #$00
    sta KB_ISSHIFT
    jmp end_of_shift_ctrl_check
    
    release_ctrl:
    lda #$00
    sta KB_ISCTRL
    
    end_of_shift_ctrl_check:
    
    lda #$00;           
    sta KB_NEXTRELEASE
    jmp skipFuncs
    
    release_if_end:
    
    
    lda KB_NEXTRELEASE
    cmp #$FF
    beq end_of_shift_ctrl_check  ; will ignore next byte and reset KB_NEXTRELEASE

    tya
    
    ldy KB_ISCTRL ; if control
    cpy #$FF
    BNE no_control
    ; check is $21 pressed, if yes set break
    cmp #$21
    BNE skipFuncs
    LDA #$FF
    STA KB_BREAK
    
    jmp skipFuncs
    
    no_control:
    ldy KB_ISSHIFT ; if shift
    cpy #$FF
    beq use_shift

    tay
    lda asciitbl, Y
    
    jmp shift_chk_skip
    
    use_shift:
    
    tay
    lda asciitbl_shift, Y
    
    shift_chk_skip:
    tay

    lda KBBUFPOS
    tax
    
    tya
    sta KBMEM, X
    inc KBBUFPOS
    
    ;JSR MONCOUT

    lda KBBUFPOS
    
    cmp #32
    beq setToZero  ; set to 0 then to cycle over
    jmp skipFuncs

    setToZero:
    lda #$0
    sta KBBUFPOS

    skipFuncs:
    
    
    PLA
    TAY
    PLA
    TAX
    PLA

    RTI

StartupMessage:
;	.byte	$0C,"Cold [C] or warm [W] start?",$0D,$0A,$00
	.byte	"MANIEK86.XYZ", $0A, "[c]old/[w]arm? ", $00
LOAD:
	RTS
	
SAVE:
	RTS
	
.segment "VECTS"
.org $FFFA
	.word	KBHandle		; NMI 
	.word	Reset		; RESET 
	.word	Reset		; IRQ 
