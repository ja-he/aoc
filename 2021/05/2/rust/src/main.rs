use std::collections::HashSet;
use std::fmt::{self, Display};
use std::num::ParseIntError;
use std::str::FromStr;
use std::vec::Vec;
use std::{io, ops};

#[derive(Eq, Hash, PartialEq, Copy, Clone)]
struct Point {
    x: i32,
    y: i32,
}

struct Map {
    data: Vec<Vec<u32>>,
}

impl Map {
    fn get_max_x(&self) -> usize {
        return self.data.len() - 1;
    }

    fn get_max_y(&self) -> usize {
        return self.data[0].len() - 1;
    }

    fn register(&mut self, line: &Line) {
        self.resize_for(&line.get_max_xy());
        for point in line.points().iter() {
            let x: usize = point.x.try_into().unwrap();
            let y: usize = point.y.try_into().unwrap();
            self.data[x][y] += 1;
        }
    }

    fn print(&self) {
        for j in 0..self.data[0].len() {
            for i in 0..self.data.len() {
                print!("{: >4}", self.data[i][j]);
            }
            println!();
        }
    }

    fn count_intersections(&self) -> usize {
        let mut counter = 0;
        for i in 0..self.data.len() {
            for j in 0..self.data[0].len() {
                if self.data[i][j] >= 2 {
                    counter += 1;
                }
            }
        }
        return counter;
    }

    fn resize_for(&mut self, intersection: &Point) {
        let x: usize = intersection.x.try_into().unwrap();
        let y: usize = intersection.y.try_into().unwrap();
        let max_x = if x > self.get_max_x() {
            x
        } else {
            self.get_max_x()
        };
        let max_y = if y > self.get_max_y() {
            y
        } else {
            self.get_max_y()
        };

        if max_x > self.get_max_x() {
            self.data.resize(max_x + 1, vec![0; max_y + 1]);
        }
        if max_y > self.get_max_y() {
            for i in 0..max_x + 1 {
                self.data[i].resize(max_y + 1, 0);
            }
        }
    }
}

impl Display for Point {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{},{}", self.x, self.y)
    }
}

struct Line {
    a: Point,
    b: Point,
}

impl Display for Line {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} -> {}", self.a, self.b)
    }
}

impl ops::Add<Point> for Point {
    type Output = Point;
    fn add(self, _rhs: Point) -> Point {
        return Point {
            x: self.x + _rhs.x,
            y: self.y + _rhs.y,
        };
    }
}

impl Line {
    fn get_incrementer(&self) -> Point {
        let mut incrementer = Point { x: 0, y: 0 };

        if self.a.x < self.b.x {
            incrementer.x = 1;
        }
        if self.a.y < self.b.y {
            incrementer.y = 1;
        }
        if self.a.x > self.b.x {
            incrementer.x = -1;
        }
        if self.a.y > self.b.y {
            incrementer.y = -1;
        }

        return incrementer;
    }

    fn get_max_xy(&self) -> Point {
        let x = if self.a.x > self.b.x {
            self.a.x
        } else {
            self.b.x
        };
        let y = if self.a.y > self.b.y {
            self.a.y
        } else {
            self.b.y
        };
        return Point { x, y };
    }

    fn points(&self) -> Vec<Point> {
        let delta_x = (self.a.x - self.b.x).abs();
        let delta_y = (self.a.y - self.b.y).abs();
        let cap: usize = if delta_x > delta_y { delta_x } else { delta_y }
            .try_into()
            .unwrap();
        let mut result = Vec::with_capacity(cap);

        let mut p = self.a;
        let incrementer = self.get_incrementer();
        while p != self.b {
            result.push(p);
            p = p + incrementer;
        }
        result.push(self.b);

        return result;
    }
}

impl FromStr for Line {
    type Err = ParseIntError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let coords: Vec<&str> = s.split(" -> ").collect();
        let (pair1, pair2): (Vec<&str>, Vec<&str>) = (
            coords[0].split(',').collect(),
            coords[1].split(',').collect(),
        );

        let (x1, y1) = (pair1[0], pair1[1]);
        let (x2, y2) = (pair2[0], pair2[1]);

        let x1: i32 = x1.parse::<i32>().unwrap();
        let y1: i32 = y1.parse::<i32>().unwrap();
        let x2: i32 = x2.parse::<i32>().unwrap();
        let y2: i32 = y2.parse::<i32>().unwrap();

        return Ok(Line {
            a: Point { x: x1, y: y1 },
            b: Point { x: x2, y: y2 },
        });
    }
}

fn main() {
    let mut lines: Vec<Line> = Vec::new();

    loop {
        let mut input = String::new();

        let bytes_read = io::stdin()
            .read_line(&mut input)
            .expect("Failed to read line");
        let is_eof = bytes_read == 0;
        if is_eof {
            break;
        }
        let input = input.trim();

        let line = Line::from_str(input).unwrap();

        lines.push(line)
    }

    let mut map = Map {
        data: vec![vec![0; 10]; 10],
    };

    for i in 0..lines.len() {
        println!("placing line {}", i);
        map.register(&lines[i]);
    }

    // map.print();
    println!("final: {} intersection points", map.count_intersections());
}
