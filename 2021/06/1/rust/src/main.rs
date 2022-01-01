use std::io;

fn main() {
    let mut input = String::new();

    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");

    let initial: Vec<usize> = input
        .split(",")
        .map(|s| s.trim().parse().expect("num parse error"))
        .collect();

    let mut counters = initial;

    for _ in 1..81 {
        let mut news = 0;
        for counter in counters.iter_mut() {
            if *counter > 0 {
                *counter -= 1;
            } else {
                *counter = 6;
                news += 1;
            }
        }
        counters.resize(counters.len() + news, 8);
    }

    println!("final count: {}", counters.len());
}
