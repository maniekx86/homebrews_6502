KBAVAILABLE = $7F00
KBBUF = $7F01
INPORT = $B002 ; PORT C


; ---------------------------------------------------------------------------

.import   _stop
.export   _irq_int, _nmi_int

.segment  "CODE"


; ---------------------------------------------------------------------------
; Non-maskable interrupt (NMI) service routine

_nmi_int:                      ; Return from all NMI interrupts
    PHA
    TXA
    TSX
    PHA
    TYA
    PHA
    
    ldy INPORT

    lda KBAVAILABLE

    tax
    tya

    sta KBBUF, X
    inc KBAVAILABLE

    lda KBAVAILABLE

    cmp #32
    beq setToZero  ; set to 0 then to cycle over
    jmp skipFuncs

    setToZero:
    lda #$0
    sta KBAVAILABLE

    skipFuncs:


_nmi_end:

    PLA
    TAY
    PLA
    TAX
    PLA

    RTI

_irq_int:  

irq:       
    
    RTI                    ; Return from all IRQ interrupts

; ---------------------------------------------------------------------------
; BRK detected, stop

break:     JMP break              ; If BRK is detected, something very bad
                                  ;   has happened, so stop running
