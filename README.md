# LSSC
A **L**ightweight **S**udoku **S**olver using **C**

LSSC uses a backtracking algorithm approach to solve Sudoku puzzles.
A starting configuration is read in (in the form of a .txt file)
which is used to create the board. The starting board is output to the 
user, then the rest of the board is solved and output to the user in 
completion. All outputs are provided as command line prints.

The current implementation is only around 200 lines of code and has
logic built in to detect impossible puzzles. This quick project is
a fun exercise in file IO and pointer arithmetic and is a good example
of procedural programming.

# Compile/Run Instructions
Compile using gcc \
`gcc solver.c -o solver` \
Run from CL \
`./solver`

# Next Steps
- Implement a stochastic solving algorithm
- Add non-singular solution detection

Don't hesitate to reach out with feedback and questions!
