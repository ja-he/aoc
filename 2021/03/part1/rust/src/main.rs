use std::io;
use std::vec::Vec;

fn main() {
    let mut diagnostics: Vec<u64> = Vec::new();
    let mut n_bits: u32 = 0;
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
        n_bits = input.len() as u32;

        let diagnostic: u64 = u64::from_str_radix(input, 2).unwrap();
        diagnostics.push(diagnostic);
    }

    let mut counter = 0;
    let mut gamma_bit_counters: Vec<u64> = vec![0; n_bits as usize];
    let indices: Vec<u32> = (0..n_bits).collect();
    for diagnostic in diagnostics.iter() {
        counter += 1;
        for i in &indices {
            gamma_bit_counters[*i as usize] += (diagnostic & (1 << i)) >> i;
        }
    }
    println!("got {} diagnostics", counter);

    let mut gamma: u64 = 0;
    for i in &indices {
        if gamma_bit_counters[*i as usize] > counter/2 {
            gamma |= 1 << i;
        }
    }
    assert!(n_bits < 64);
    let epsilon = gamma ^ u64::pow(2, n_bits) - 1;
    println!("gamma rate: \t{:032b} ({})", gamma, gamma);
    println!("epsilon rate: \t{:032b} ({})", epsilon, epsilon);

    println!("power usage: {}", gamma * epsilon);
}
