1. support all input methods (DONE)
2. fix collision logic (DONE)
3. render walls
4. implement clear particles, walls, all (DONE)
5. implement multithreading
6. optimization


issues:
1. for input batch method 2, start angle 0 to 360, at slower velocities < 100, resembles a square, at higher velocities > 300 resembles more of a circle
— possible explanations: discretization of motion fix: use floats for x and y positions or doubles (FIXED: turned all ints to double)
2. try adding inputs at the exact corners and see it shifts in starting positions example is (0,0) at 45degree angle and 100 velocity
