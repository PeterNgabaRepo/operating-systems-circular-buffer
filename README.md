# Operating Systems Circular Buffer Project

NASA-inspired operating systems coursework for UMBC CMSC 421.

This project implements a bounded circular buffer in C and then extends the same core idea into producer/consumer synchronization using semaphores and monitor-style condition variables. The original assignment also included Linux kernel syscall variants. This version preserves the coursework structure while removing generated binaries and adding short deterministic smoke tests for local validation.

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
- `tests/`: Short smoke tests for local validation.

## Local Validation

From this folder:

```bash
python3 tests/validate_os_candidate.py
make -f Makefile.portfolio compile
make -f Makefile.portfolio clean
```

The compile target builds short smoke binaries without running the long original coursework harnesses. The original tests are preserved in each assignment folder.

Optional runtime smoke execution is available with:

```bash
make -f Makefile.portfolio runtime-test
```

The runtime test prints labeled sections for the simple buffer, semaphore producer-consumer buffer, and monitor producer-consumer buffer. The smoke tests intentionally include a few negative-path checks before initialization, then verify the expected valid behavior.

## Notes

- The kernel syscall files are preserved as coursework artifacts but are not rebuilt in this macOS environment.
- The user-space semaphore smoke harness includes a small local counting-semaphore compatibility layer for macOS so the diagnostic counter output remains meaningful.
- This is presented as honest systems coursework, not as production aerospace software.
