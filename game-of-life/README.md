# game-of-life

run and compile with:

- make ; ./main 
  - to run tests
- make ; ./main X
  - run game of life with X rows and X columns for 10 evolutions
- make ; ./main X Y
  - run game of life with X rows, Y columns, for 10 evolutions 
- make ; ./main  X Y Z
  -  run game of life with X rows, Y columns, for Z evolutions 



Assumptions made:

- All cells react/change at same time (i.e. one whole grid evolution is instantaneous)
- Grid does **not** loop (for example a cell on the top row has no neighbour above it)
- A cell is **not** a neighbour to itself