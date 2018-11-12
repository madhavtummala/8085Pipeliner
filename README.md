The main file is "compiler.cpp"

The input code is taken from the file "test.txt" and the newly generated code is written to "test_out.txt"

Instructions:

g++ compiler.cpp
./a.out

Follow the comments in the code to understand the implementation.

LOGIC:

Polynomial Time Algorithm. Intelligent Reordering of instructions to get best code by adding minimum no of NOPs - Used Dependency graph and Sorting to give the best sequence.

The program tries to reorder the existing instructions in best possible way to remove dependencies. If removing dependencies is not possible just by reordering, then it tries to insert the minimum no of NOP instructions to achieve it.

The program functionality is completely preserved even after reordering. The 'start' and 'end' contents of all the registers will be the same in both the new and old codes. We just played with the intermediate contents and optimised the code for pipelining.

These dependencies can occur only between 5th stage of an instruction and 3rd stage of another instruction. 5th stage being the updating step and 3rd stage being the execution/using step.
So, each instruction is divided based on what it updates and what it uses and a dependency graph is made accordingly.

As for the no of cycles wasted by memory delays, we calculate if the current instruction is a memory instruction and if the following instruction is a NOP/HLT. We get 6(2*3) or 4 cylces wasted respectively.

There are 5 test files given.