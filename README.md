# Advent of Code

Solving the problems given for
[Advent of Code](https://adventofcode.com/2020/) (AoC)
day by day.

Solutions may be simple and simply go for a hacky quick solution or they may be
complex and aim to minimize complexity (time or space) or exhibit very safe
coding style or anything else that I may feel like on that day.
They may also come in different languages.

# Structure

Each day has its own directory in which each part has its own directory.
So the solution for part 2 of day 8 of 2020 would be found in
`2020/08/part2/cpp` while the input for that day will be in `2020/08/input`.

Each main solution directory has a `Makefile` in it, specifying (possibly among
others) a target `solve` which will execute the solution, e.g. by compiling the
program and executing with the puzzle input on standard input.

To run the solution for part 2 of day 8, all you need to do is call
```
make -C 2020/08/part2/cpp
```
which will probably run something like `./a.out < ../../input` inside
`2020/08/part2/cpp`.
