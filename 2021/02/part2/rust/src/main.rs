use std::io;
use std::str::FromStr;
use std::vec::Vec;
use strum_macros::Display;
use strum_macros::EnumString;

struct Sub {
    aim: i32,
    pos: Position,
}

impl Sub {
    fn apply(&mut self, instruction: &Instruction) {
        match instruction.direction {
            Direction::Up => self.aim -= instruction.magnitude,
            Direction::Down => self.aim += instruction.magnitude,
            Direction::Forward => {
                self.pos.horiz += instruction.magnitude;
                self.pos.depth += instruction.magnitude * self.aim;
            },
        }
    }
}

struct Position {
    horiz: i32,
    depth: i32,
}

struct Instruction {
    direction: Direction,
    magnitude: i32,
}

#[derive(Display, EnumString)]
#[strum(serialize_all = "snake_case")]
enum Direction {
    Up,
    Down,
    Forward,
}

fn main() {
    // parse movements into vector
    let mut movements: Vec<Instruction> = Vec::new();
    loop {
        let mut input = String::new();

        let bytes_read = io::stdin()
            .read_line(&mut input)
            .expect("Failed to read line");
        let is_eof = bytes_read == 0;
        if is_eof {
            break;
        }

        let mut tokens_iter = input.trim().split_whitespace();
        let direction = tokens_iter.next().unwrap();
        let magnitude = tokens_iter.next().unwrap();

        let direction = Direction::from_str(direction).unwrap();
        let magnitude: i32 = match magnitude.parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        let instruction: Instruction = Instruction {
            direction,
            magnitude,
        };

        movements.push(instruction)
    }

    // apply movements to sub
    let mut sub: Sub = Sub {
        aim: 0,
        pos: Position { horiz: 0, depth: 0 },
    };
    for instruction in movements.iter() {
        sub.apply(instruction);
    }

    // emit result
    println!("final pos: (horiz:{},depth:{})", sub.pos.horiz, sub.pos.depth);
    println!("multiplied: {}", (sub.pos.horiz * sub.pos.depth));
}
