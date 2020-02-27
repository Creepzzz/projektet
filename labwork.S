  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged

  #Ändrad till våran

.macro PUSH reg
	addi $sp, $sp, -4
	sw \reg, 0($sp)
.endm

.macro POP reg
	lw \reg, 0($sp)
	addi $sp, $sp, 4
.endm
  #
  # Please add your own code below this line
  #
 .global delay
 .global hexasc
 .global time2string
 .global enable_interrupt
 .text
 
delay: 
	while:
		beq  $a0, $0, whileoff
		nop
		addi $a0, $a0, -1          #a0 = ms
		li   $t0, 0                #t0 = i
		li   $t1, 4500              #t1 = 4711
		for:
			beq  $t0, $t1, while
			nop
			addi $t0, $t0, 1
			j    for
			nop
	whileoff:
	
		jr   $ra
		nop
		
		

hexasc: 
    	andi   $a0, $a0, 0xf     #för att maska så att endast första nibble
    	slti   $t0, $a0, 10      #om a0 < 10 blir t0 = 1, annara blir t0 = 0
        beq    $0, $t0, letters  #om t0 = 0 har vi en siffra från 10-15 vilket ska va en bokstav
    	addi   $v0, $a0, 0x30    #returnerar v0 som är a0 + 0x30 (där siffror i ASCII börjar)

    	jr     $ra
    	nop
       letters: addi   $v0, $a0, 0x26    #returnerar v0 som är a0 + 0x37 (där bokstäver i ASCII börjar)
    	jr     $ra
    	nop


time2string: 
			 PUSH $s0              #lagra dessa på stacken
			 PUSH $s1
			 PUSH $ra
			 
			 move $s0, $a0           #flytta till s0 och s1
			 move $s1, $a1
			 
			 andi $a1, $a1, 0xffff   #maska för att få ut se 2 sista bytsen
             # Första siffran
             
             srl  $a0, $s1, 12
             jal  hexasc
             nop
             sb   $v0, 0($s0)
             # Andra siffran
             
             srl  $a0, $s1, 8
             jal  hexasc
             nop
             sb   $v0, 1($s0)       #nästa byte
             # Kolon
             addi $a0, $0, 0x3a
             sb   $a0, 2($s0)
             # Tredje siffran
             
             srl  $a0, $s1,4
             jal  hexasc
             nop
             sb   $v0, 3($s0)
             # Fjärde siffran
             move $a0, $s1
             jal hexasc
             nop
             sb  $v0, 4($s0)

   
             # Null-byte
             addi $t0, $0, 0
             sb   $t0, 5($s0)       #null-byte fungerar som slutsignal
      null:        
             POP $ra               #extrahera värdena från stacken
             POP $s1
             POP $s0
             
             jr $ra
             nop
             


enable_interrupt:
    ei
    jr $ra
    nop
