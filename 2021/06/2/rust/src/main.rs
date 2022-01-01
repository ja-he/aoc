use std::io;

// The crux here is to simply bunch all of the fishes with the same counter
// value together, as their behaviour is the same anyways.
// This significantly improves performance from the basic vector solution in
// part 1, which would have been completely unfit here.
fn main() {
    let mut input = String::new();

    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");

    let initial: Vec<usize> = input
        .split(",")
        .map(|s| s.trim().parse().expect("num parse error"))
        .collect();

    // initialize a vector indexed by [0,...,8] where the index is the counter
    // value and the value at that index is the number of fishes with that
    // counter value
    let mut fishes: Vec<usize> = vec![0; 9];
    // initialize the number of fishes per counter value from initial state
    for counter in initial.iter() { fishes[*counter] += 1; }

    for _ in 1..256+1 {
        let new_fishes = fishes[0];
        for counter_value in 0..9 {
            match counter_value {
                6 => fishes[counter_value] = fishes[counter_value+1] + new_fishes,
                8 => fishes[counter_value] = new_fishes,
                _ => fishes[counter_value] = fishes[counter_value+1],
            }
        }
    }

    let sum: usize = fishes.iter().sum();
    println!("final count: {}", sum);
}
