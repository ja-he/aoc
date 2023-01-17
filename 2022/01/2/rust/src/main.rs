use std::{
    collections::BinaryHeap,
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

/// Inv describes the inversion of something, e.g. the negation operation on a boolean:
///
/// ```
/// impl Inv for bool {
///     fn invert(&self) -> Self { !self }
/// }
/// ```
pub trait Inv {
    /// Invert the given element.
    ///
    /// ```
    /// let x: i32 = 12;
    /// println!("{}", x.invert()); // -> -12
    /// ```
    ///
    /// Ensure that `x == x.invert().invert()`.
    fn invert(&self) -> Self;
}

impl Inv for u64 {
    fn invert(&self) -> Self {
        u64::MAX - self
    }
}

/// NLargest is a rolling container for the (at most) `n` largest items which
/// are pushed through it.
pub struct NLargest<T>
where
    T: Ord,
{
    n: usize,
    h: BinaryHeap<T>,
}

impl<T> NLargest<T>
where
    T: Ord,
    T: Inv,
{
    /// Create a new `NLargest` for the given `n`.
    pub fn new(n: usize) -> Self {
        NLargest {
            n,
            h: BinaryHeap::with_capacity(n),
        }
    }

    /// Push a new item through this `NLargest`.
    ///
    /// When an item is pushed through it while it already holds `n` values, it is only retained if
    /// it is larger than the smallest previously retained value, supplanting it.
    /// (When there are not yet `n` items retained, any pushed item is retained.)
    pub fn push(&mut self, item: T) {
        if self.h.len() < self.n {
            self.h.push(item.invert());
        } else if item.cmp(&self.h.peek().unwrap().invert()).is_gt() {
            self.h.pop();
            self.h.push(item.invert());
        }
    }
}

impl<T> IntoIterator for NLargest<T>
where
    T: Ord,
    T: Inv,
{
    type Item = T;
    type IntoIter = std::vec::IntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        self.h
            .into_iter()
            .map(|x| x.invert())
            .collect::<Vec<T>>()
            .into_iter()
    }
}

fn main() -> Result<(), AocError> {
    let mut s: String = String::default();
    if std::io::stdin().read_to_string(&mut s)? == 0 {
        return Err(AocError {
            msg: "read too little?".to_string(),
        });
    };

    // declare our helper container which will have all values pushed through it and retain the `n`
    // largest, where `n` in our case is `3`
    let mut three_max_values: NLargest<u64> = NLargest::new(3);

    // push all elf payload sums through our helper container
    s.split("\n\n")
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
        .map(|elf_payload| elf_payload.sum::<u64>())
        .for_each(|item| three_max_values.push(item));

    // sum up the (3) maximal values we have retained
    let max: u64 = three_max_values.into_iter().sum();

    println!("{max}");

    Ok(())
}
