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
        original_pos = position
        extra_zero_passes = (ofs // rotary_range)
        ofs_adjusted = ofs % rotary_range
        delta = -ofs_adjusted if dir == Direction.LEFT else ofs_adjusted
        position += delta
        if position < 0:
            if original_pos != 0:
                extra_zero_passes += 1
            position += rotary_range
        if position >= rotary_range:
            extra_zero_passes += 1
            position -= rotary_range
        if position == 0 and dir == Direction.LEFT and original_pos > 0:
            extra_zero_passes += 1
        if position == 0:
            n_times_at_zero += 1
        print(f"move {delta} to {position} passing zero {extra_zero_passes} times")
        zero_passes += extra_zero_passes
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
