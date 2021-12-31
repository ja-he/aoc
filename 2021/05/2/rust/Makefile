all: timed_solve

timed_solve: target/debug/main
	time target/debug/main < ../../input

solve: target/debug/main
	target/debug/main < ../../input

target/debug/main: src/main.rs
	cargo build

clean:
	cargo clean
