# KuroScript
A simple interpreter for KuroScript.

Memory:

  KuroScript handels memory by allocating "cels" that are 4 bytes long.
  Each one is initialized with their cell number. I.e cell 0 gets 0, cell 5 gets 5.

  "_10"   <allocates 10 memory cells (40 bytes)>

  If nothing is declared at the beginning, KuroScript automatically allocates 5 cells.

Output:
  '@' begins output, and ')' ends it. Anything between the two is output.
  
  "@Hello,World)" <outputs "Hello, World">
  
  the symbols '#' and '[' can be used to output a memory cell as either a number or an ASCII character.
  
  "@ #23 )"       <displays cell 23 as a number>
  "@ [23 )"       <displays cell 23 as ASCII   >

Conditional branching
  "?2=3   @True) } @False) ~" <displays "False" because 2 is not equal to 3                                           >
  "?[3>[4 @True) } @False) ~" <displays "True" if the value of cell 3 is greater than cell 4, and "False" if it is not>

Exit:
    '.' will end the program.
