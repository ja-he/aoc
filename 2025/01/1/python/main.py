from enum import Enum
from typing import TypeAlias
import sys

Position: TypeAlias = int
Offset: TypeAlias = int
class Direction(Enum):
    LEFT = 'L'
    RIGHT = 'R'
Movement: TypeAlias = tuple[Direction, Offset]

def compute_solution(initial_position: Position, rotary_range: Position, steps: list[Movement]) -> tuple[Position, int, int]:
    position = initial_position
    zero_passes = 0
    n_times_at_zero = 0
    for (dir, ofs) in steps:
        zero_passes += (ofs // rotary_range)
        ofs_adjusted = ofs % rotary_range
        if dir == Direction.LEFT:
            position -= ofs_adjusted
        else:
            position += ofs_adjusted
        if position < 0:
            zero_passes += 1
            position += rotary_range
        if position >= rotary_range:
            zero_passes += 1
            position -= rotary_range
        if position == 0:
            n_times_at_zero += 1
    return position, zero_passes, n_times_at_zero

def parse_line(line: str) -> Movement:
    dir_char = line[0]
    dir = Direction(dir_char)
    return (dir, int(line[1:]))

def main():
    steps = [parse_line(line) for line in sys.stdin]
    position, zero_passes, n_times_at_zero = compute_solution(initial_position=50, rotary_range=100, steps=steps)
    print(f"pos={position} zero_passes={zero_passes} n_times_at_zero={n_times_at_zero}")


if __name__ == "__main__":
    main()
