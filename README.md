# Operating Systems Circular Buffer Project

NASA-inspired operating systems coursework for UMBC CMSC 421.

This project implements a bounded circular buffer in C and then extends the same core idea into producer/consumer synchronization using semaphores and monitor-style condition variables. The original assignment also included Linux kernel syscall variants. This cleaned portfolio candidate preserves the coursework structure while removing generated binaries and adding short deterministic smoke tests for local validation.

## What It Demonstrates

- Circular buffer read/write pointer movement
- Bounded-buffer full/empty behavior
- Producer/consumer synchronization
- POSIX semaphore coordination
- Monitor-style condition variables
- User-space and kernel-space implementation patterns
- Systems-level debugging and validation

## Structure

- `buffer/`: Part 1 circular buffer implementation and user-space test.
- `prodcon_sem/`: Part 2 producer/consumer bounded buffer with semaphores.
- `prodcon_mon/`: Part 3 producer/consumer bounded buffer with monitor-style condition variables.
- `tests/`: Short smoke tests for portfolio/CI-style validation.

## Local Validation

From this folder:

```bash
python3 tests/validate_os_candidate.py
make -f Makefile.portfolio compile
make -f Makefile.portfolio clean
```

The portfolio compile target builds short smoke binaries without running the long original coursework harnesses. The original tests are preserved in each assignment folder.

Optional runtime smoke execution is available with:

```bash
make -f Makefile.portfolio runtime-test
```

In this Codex sandbox on macOS, the `smoke_buffer` runtime binary can hang even though the source compiles. Keep runtime execution as a review gate for a normal local terminal or Linux environment before publication.

## Notes

- The kernel syscall files are preserved as coursework artifacts but are not rebuilt in this macOS environment.
- The semaphore implementation uses POSIX APIs that are deprecated on macOS, so macOS builds may show warnings even when the code compiles and runs.
- This is presented as honest systems coursework, not as production aerospace software.
