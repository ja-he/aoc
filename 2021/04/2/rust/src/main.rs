use std::io;
use std::vec::Vec;

type BingoField = (i32, bool);

struct BingoBoard {
    board: [[BingoField; 5]; 5],
}

impl BingoBoard {
    fn bingo(&self) -> bool {
        for i in 0..5 {
            let n_drawn = self.board[i]
                .iter()
                .filter(|(_, drawn)| -> bool { *drawn })
                .count();
            if n_drawn == 5 {
                return true;
            }
        }
        // TODO: I'm sure there's a similar way as above to iterate through columns here?
        for j in 0..5 {
            let mut n_drawn = 0;
            for i in 0..5 {
                if self.board[i][j].1 {
                    n_drawn += 1;
                }
            }
            if n_drawn == 5 {
                return true;
            }
        }

        return false;
    }

    fn register_drawing(&mut self, num: i32) {
        for i in 0..5 {
            for j in 0..5 {
                if self.board[i][j].0 == num {
                    self.board[i][j].1 = true;
                }
            }
        }
    }

    fn reset(&mut self) {
        for i in 0..5 {
            for j in 0..5 {
                self.board[i][j].1 = false;
            }
        }
    }

    fn from_input(lines: [String; 5]) -> BingoBoard {
        let mut board: [[BingoField; 5]; 5] = [[(0, false); 5]; 5];

        let mut i = 0;
        for input_line in lines.iter() {
            let board_line: Vec<BingoField> = input_line
                .split_whitespace()
                .map(|numstr| -> (i32, bool) {
                    (
                        match numstr.parse() {
                            Ok(num) => num,
                            Err(_) => panic!("it's all gone wrong..."),
                        },
                        false,
                    )
                })
                .collect();
            if board_line.len() != 5 {
                panic!(
                    "parsed vector for input_line '{}' not of length 5!",
                    input_line
                )
            } else {
                board[i] = board_line.try_into().unwrap_or_else(|v| panic!("FRICK!"));
                i += 1;
            }
        }

        return BingoBoard { board };
    }

    fn print(&self) {
        for line in self.board.iter() {
            for (num, hit) in line.iter() {
                print!("[{} {}]", num, hit)
            }
            println!()
        }
        println!(
            "{}",
            if self.bingo() {
                "<BINGO!>"
            } else {
                "<not a bingo>"
            }
        )
    }

    fn compute_winning_score(&self, winning_draw: i32) -> i32 {
        let mut sum = 0;
        for i in 0..5 {
            for j in 0..5 {
                let (num, drawn) = self.board[i][j];
                if !drawn {
                    sum += num;
                }
            }
        }

        return sum * winning_draw;
    }
}

// returns true when eof encountered, otherwise just gobbles an empty line;
// panics if line is neither eof nor empty
fn gobble_empty_line() -> bool {
    let mut expected_empty_line = String::new();
    let bytes_read = io::stdin()
        .read_line(&mut expected_empty_line)
        .expect("Failed to read empty line after drawings");
    let is_eof = bytes_read == 0;
    if is_eof {
        return true;
    }
    if !expected_empty_line.trim().is_empty() {
        panic!("line isn't empty but '{}'", expected_empty_line);
    }
    return false;
}

fn read_bingo_board() -> BingoBoard {
    let mut bingo_board_input: [String; 5] = Default::default();
    for i in 0..5 {
        let mut line = String::new();
        let bytes_read = io::stdin()
            .read_line(&mut line)
            .expect("Failed to read line");
        let is_eof = bytes_read == 0;
        if is_eof {
            panic!("eof in bingo board!");
        }

        bingo_board_input[i] = line;
    }
    return BingoBoard::from_input(bingo_board_input);
}

fn compute_losing_board(boards: &mut Vec<BingoBoard>, drawings: &Vec<i32>) -> Option<usize> {
    let mut winners: Vec<usize> = Vec::new();

    for &drawing in drawings.iter() {
        for drawing in drawings.iter() {
            for i in 0..boards.len() {
                if winners.contains(&i) {
                    continue;
                };

                boards[i].register_drawing(*drawing);
                if boards[i].bingo() {
                    winners.push(i);
                    if winners.len() == (boards.len() - 1) {
                        for i in 0..boards.len() {
                            if !winners.contains(&i) {
                                return Some(i);
                            }
                        }
                    }
                }
            }
        }
    }
    return None;
}

fn main() {
    let mut drawings_input = String::new();
    let bytes_read = io::stdin()
        .read_line(&mut drawings_input)
        .expect("Failed to read drawings");
    let drawings: Vec<i32> = drawings_input
        .trim()
        .split(',')
        .map(|s| -> i32 {
            match s.parse() {
                Ok(n) => n,
                Err(_) => panic!("cannot parse drawing string '{}'", s),
            }
        })
        .collect();

    let mut boards: Vec<BingoBoard> = Default::default();
    loop {
        let eof_encountered = gobble_empty_line();
        if eof_encountered {
            break;
        } else {
            boards.push(read_bingo_board());
        }
    }
    println!("read in {} boards", boards.len());

    let loser = match compute_losing_board(&mut boards, &drawings) {
        Some(result) => result,
        None => panic!("no winner found?!"),
    };

    println!("loser is this board:");
    boards[loser].print();

    boards[loser].reset();
    for drawing in drawings.iter() {
        boards[loser].register_drawing(*drawing);
        if boards[loser].bingo() {
            println!(
                "hypothetical winning score of losing board: {}",
                boards[loser].compute_winning_score(*drawing)
            );
            break;
        }
    }
}
