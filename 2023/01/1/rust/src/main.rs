use std::{
    fmt::Display,
    io::{self, Read},
    num::ParseIntError,
    string::FromUtf8Error,
};

#[derive(Debug, Clone)]
struct AocError {
    pub msg: String,
}

impl From<io::Error> for AocError {
    fn from(value: io::Error) -> Self {
        Self {
            msg: "from-utf8-error:".to_owned() + &value.to_string(),
        }
    }
}

impl From<FromUtf8Error> for AocError {
    fn from(value: FromUtf8Error) -> Self {
        Self {
            msg: "from-utf8-error:".to_owned() + &value.to_string(),
        }
    }
}

impl From<ParseIntError> for AocError {
    fn from(value: ParseIntError) -> Self {
        Self {
            msg: "parse-int-error:".to_owned() + &value.to_string(),
        }
    }
}

impl Display for AocError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.msg)
    }
}

fn get_calibration_value_indices(s: &[u8]) -> Result<(usize, usize), AocError> {
    let mut first: Option<usize> = None;
    let mut last: Option<usize> = None;

    for i in 0..s.len() {
        if s[i].is_ascii_digit() {
            first = Some(i);
            break;
        }
    }
    if first.is_none() {
        return Err(AocError {
            msg: "no digits found".to_string(),
        });
    }

    for i in (first.unwrap()..s.len()).rev() {
        if s[i].is_ascii_digit() {
            last = Some(i);
            break;
        }
    }
    if last.is_none() {
        // convert the &[u8] to a string again
        let debug_str = String::from_utf8(s.to_vec())?;
        return Err(AocError {
            msg: "no second digit found in line '".to_string() + &debug_str + "'",
        });
    }

    Ok((first.unwrap(), last.unwrap()))
}

fn main() -> Result<(), AocError> {
    let mut s: String = String::default();
    if std::io::stdin().read_to_string(&mut s)? == 0 {
        return Err(AocError {
            msg: "read too little?".to_string(),
        });
    };

    let sum: usize = s
        .lines()
        .map(|s| -> Result<usize, AocError> {
            let b = s.as_bytes();
            let indices = get_calibration_value_indices(b)?;

            // declare a 2-element array of u8
            let digits: [u8; 2] = [b[indices.0], b[indices.1]];

            Ok(String::from_utf8(digits.to_vec())?.parse::<usize>()?)
        })
        .collect::<Result<Vec<usize>, AocError>>()?
        .iter()
        .sum();

    println!("{sum}");

    Ok(())
}
