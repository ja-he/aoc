use std::{
    fmt::Display,
    io::{self, Read},
};

#[derive(Debug, Clone)]
struct AocError {
    pub msg: String,
}

impl From<io::Error> for AocError {
    fn from(value: io::Error) -> Self {
        Self {
            msg: value.to_string(),
        }
    }
}

impl Display for AocError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.msg)
    }
}

fn main() -> Result<(), AocError> {
    let mut s: String = String::default();
    if std::io::stdin().read_to_string(&mut s)? == 0 {
        return Err(AocError {
            msg: "read too little?".to_string(),
        });
    };

    let max: u64 = s
        .split("\n\n")
        .map(|block| {
            block.split('\n').filter_map(|value| {
                if value.is_empty() {
                    return None;
                }
                match value.parse::<u64>() {
                    Ok(v) => Some(v),
                    Err(e) => {
                        panic!("could not parse '{value}' ({e}); assuming that's an error.");
                    }
                }
            })
        })
        .map(|elf_payload| elf_payload.sum())
        .max()
        .ok_or(AocError {
            msg: "unexpectedly no max found (empty list)".to_string(),
        })?;

    println!("{max}");

    Ok(())
}
