use std::io;

fn main() {
    let mut prev_elevation: i32 = -1;
    let mut counter: u32 = 0;
    loop {
        let mut elevation = String::new();

        let bytes_read = io::stdin()
            .read_line(&mut elevation)
            .expect("Failed to read line");
        let is_eof = bytes_read == 0;
        if is_eof {
            break;
        }

        let elevation: i32 = match elevation.trim().parse() {
            Ok(num) => num,
            Err(_) => break,
        };

        let depth_increase = elevation > prev_elevation;
        let first_measurement = prev_elevation < 0;
        if depth_increase && !first_measurement {
            counter += 1;
        }
        prev_elevation = elevation;
    }
    println!("N of depth increases: {}", counter);
}
