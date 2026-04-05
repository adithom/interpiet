## Interpiet

a minimal interpreter for the esoteric programming language piet written in c.

logic flow goes like this
ppm image -> a struct with a grid of all the pixels -> codelgrid -> floodfill that uses bfs to create a blockmap -> parser -> instruction lookup -> piet working stack
