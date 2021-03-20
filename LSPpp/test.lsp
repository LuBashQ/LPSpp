; Simple program that test the functionalities of the language
; Author: Cristian Nicolae Lupascu

MOVE 4 @7 ;move 4 into reg[7]
GEQ 5 @7
JMPC :4
PRINT @8

JMP :4
JMP :@2

MOVE 2 @2
PRINT @2

MOVE 4 @2
MOVE @2 @4

PRINT @4

ADD @2 @4

PRINT @4

HLT