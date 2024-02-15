1. support all input methods (DONE)
2. fix collision logic (DONE)
3. render walls (DONE)
4. implement clear particles, walls, all (DONE)
5. implement multithreading (DONE) //multithreaded branch
6. optimization / improve multithreading


issues:
1. (FIXED) for input batch method 2, start angle 0 to 360, at slower velocities < 100, resembles a square, at higher velocities > 300 resembles more of a circle
— possible explanations: discretization of motion fix: use floats for x and y positions or doubles (SOLUTION: turned all ints to double)
2. (expected behavior?) try adding inputs at the exact corners and see it shifts in starting positions example is (0,0) at 45degree angle and 100 velocity
3. (FIXED) user generated wall with arbitrary angle calculation problem. (SOLUTION: used atan2 to calculate angle)
4. (FIXED) user-gen wall with arbitrary angles or diagonal walls have tunneling problem.

extra features:
1. add a value randomizer for wall generation (DONE)
2. add a value randomizer for particle generation (DONE)
3. add a particle counter as display (DONE)
4. input validation
