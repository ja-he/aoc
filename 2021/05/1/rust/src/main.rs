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

    fn points(&self) -> HashSet<Point> {
        let mut result = HashSet::new();

        let mut p = self.a;
        let incrementer = self.get_incrementer();
        while p != self.b {
            result.insert(p);
            p = p + incrementer;
        }
        result.insert(self.b);

        return result;
    }
}

fn intersection(a: &Line, b: &Line) -> HashSet<Point> {
    if !(a.a.x == a.b.x || a.a.y == a.b.y) || !(b.a.x == b.b.x || b.a.y == b.b.y) {
        return HashSet::new();
    }

    return a.points().intersection(&b.points()).cloned().collect();
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

    for line in lines.iter() {
        println!("{}", line);
    }

    let mut intersections: HashSet<Point> = HashSet::new();

    let l1: Line = Line{
        a: Point{x:0,y:9},
        b: Point{x:2,y:9},
    };
    let l2: Line = Line{
        a: Point{x:0,y:9},
        b: Point{x:5,y:9},
    };
    for point in intersection(&l1, &l2).iter() {
        println!("{}", point)
    }
    println!("----");


    for i in 0..lines.len() {
        print!("doing line {}", i);
        for j in (i + 1)..lines.len() {
            intersections = intersections.union(&intersection(&lines[i], &lines[j])).cloned().collect();
        }
        println!(" -> {} intersection points", intersections.len());
    }
}
