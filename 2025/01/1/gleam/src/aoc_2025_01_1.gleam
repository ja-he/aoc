import gleam/int
import gleam/result
import gleam/string
import gleam/yielder
import stdin

type Direction {
  Left
  Right
}

type Instruction {
  Instruction(dir: Direction, count: Int)
}

type ParserError {
  InvalidDirection
  InvalidCount
  Other
}

fn parse_for_direction(c1: String) -> Result(Direction, ParserError) {
  case string.first(c1) {
    Ok("L") -> Ok(Left)
    Ok("R") -> Ok(Right)
    Ok(_) -> Error(InvalidDirection)
    Error(_) -> Error(Other)
  }
}

fn parse_line(line: String) -> Result(Instruction, ParserError) {
  let rest_of_line = string.slice(line, 1, string.length(line) - 1)
  let direction_result =
    string.first(line)
    |> result.map_error(fn(_) { Other })
    |> result.try(parse_for_direction)

  let count_result = int.parse(rest_of_line)

  case direction_result, count_result {
    Ok(d), Ok(c) -> Ok(Instruction(dir: d, count: c))
    Error(InvalidDirection), _ -> Error(InvalidDirection)
    Error(_), _ -> Error(InvalidDirection)
    _, Error(Nil) -> Error(InvalidCount)
  }
}

pub fn main() -> Nil {
  let instructions =
    stdin.read_lines()
    |> yielder.map(string.trim)
    |> yielder.map(parse_line)
    |> yielder.map(fn(r) {
      let assert Ok(instr) = r as "Instruction should be Ok"
      instr
    })

  let times_stopped_at_exactly_zero =
    instructions
    |> yielder.fold(#(50, 0), fn(p: #(Int, Int), instruction: Instruction) {
      let remaining_clicks = instruction.count % 100
      let next_pos = case instruction.dir {
        Left -> p.0 - remaining_clicks
        Right -> p.0 + remaining_clicks
      }
      let next_pos = case next_pos < 0 {
        True -> next_pos + 100
        False -> next_pos
      }
      let next_pos = case next_pos >= 100 {
        True -> next_pos - 100
        False -> next_pos
      }

      let extra_zero_stops = case next_pos == 0 {
        True -> 1
        False -> 0
      }

      #(next_pos, p.1 + extra_zero_stops)
    })
    |> fn(p: #(Int, Int)) { p.1 }

  echo times_stopped_at_exactly_zero

  Nil
}
