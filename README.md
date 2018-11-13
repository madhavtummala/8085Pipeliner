# 8085Pipeliner

## Assembly Language Input:
### Supported Instructions:

| Instruction | Description |
| ----------- | ----------- |
| MOV Ri Rj | Ri =[Rj] |
| LOAD Ri, X(Rj) | Ri = [[Rj]+X] |
| STORE Ri, X(Rj)| [[Rj]+X] = Ri |
| ADD Ri Rj Rk | RI = [Rj] + [Rk] |
| ADD Ri Rj #100 | RI = [Rj] + 10 |
| SUBRi Rj Rk | RI =[Rj]‐[Rk]|
| SUBRi Rj #200 | RI =[Rj]–200|
| ORRi Rj Rk | RI =[Rj] OR [Rk]|
| ORRi Rj #300 | RI =[Rj] OR 300|
| HLT | Stops execution|
| NOP | Does Nothing|

![Working](https://raw.githubusercontent.com/MadhavChoudhary/Terminal-Setup/master/Images/Compiler.png)

## Instructions:
```
g++ compiler.cpp
./a.out
```

The input code is taken from the file "`test.txt`" and the newly generated code is written to "`test_out.txt`"
Follow the comments in the code to understand the implementation.

## LOGIC:

Polynomial Time Algorithm. Intelligent Reordering of instructions to get best code by adding minimum no of NOPs - Used Dependency graph and Sorting to give the best sequence.

The program tries to reorder the existing instructions in best possible way to remove dependencies. If removing dependencies is not possible just by reordering, then it tries to insert the minimum no of NOP instructions to achieve it.

The program functionality is completely preserved even after reordering. The 'start' and 'end' contents of all the registers will be the same in both the new and old codes. We just played with the intermediate contents and optimised the code for pipelining.

These dependencies can occur only between 5th stage of an instruction and 3rd stage of another instruction. 5th stage being the updating step and 3rd stage being the execution/using step.
So, each instruction is divided based on what it updates and what it uses and a dependency graph is made accordingly.

As for the no of cycles wasted by memory delays, we calculate if the current instruction is a memory instruction and if the following instruction is a NOP/HLT. We get 6(2*3) or 4 cylces wasted respectively.

There are 5 testfiles [here](https://raw.githubusercontent.com/MadhavChoudhary/8085Pipeliner/master/Tests)
