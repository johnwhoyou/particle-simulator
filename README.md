1. support all input methods (DONE)
2. fix collision logic (IN PROGRESS)
3. render walls
4. implement clear particles, walls, all
5. implement multithreading
6. optimization


issues:
1. for input batch method 2, start angle 0 to 360, at slower velocities < 100, resembles a square, at higher velocities > 300 resembles more of a circle
— possible explanations: discretization of motion fix: use floats for x and y positions or doubles
2. try adding inputs at the exact corners and see it shifts in starting positions (possibly related to #1)
