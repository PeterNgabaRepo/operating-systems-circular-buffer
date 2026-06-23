from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]

REQUIRED_FILES = [
    "README.md",
    ".gitignore",
    "Makefile.portfolio",
    "buffer/buffer.h",
    "buffer/buffer_user.c",
    "prodcon_sem/buffer_sem.h",
    "prodcon_sem/buffer_user_sem.c",
    "prodcon_mon/buffer_mon.h",
    "prodcon_mon/buffer_mon.c",
    "tests/smoke_buffer.c",
    "tests/smoke_sem.c",
    "tests/smoke_mon.c",
]

FORBIDDEN_NAMES = {".DS_Store", "build"}
FORBIDDEN_SUFFIXES = {".o", ".out", ".save"}

README_PHRASES = [
    "python3 tests/validate_os_candidate.py",
    "make -f Makefile.portfolio compile",
    "honest systems coursework",
    "kernel syscall files",
]

SIMPLE_BUFFER_INVARIANT_FILES = [
    "buffer/buffer_user.c",
    "buffer/buffer.c",
]


def main() -> None:
    errors: list[str] = []

    for rel in REQUIRED_FILES:
        if not (ROOT / rel).exists():
            errors.append(f"missing required file: {rel}")

    for path in ROOT.rglob("*"):
        if path.name in FORBIDDEN_NAMES or path.suffix in FORBIDDEN_SUFFIXES:
            errors.append(f"generated or unsafe artifact found: {path.relative_to(ROOT)}")

    readme = (ROOT / "README.md").read_text(encoding="utf-8", errors="replace")
    for phrase in README_PHRASES:
        if phrase not in readme:
            errors.append(f"README missing phrase: {phrase!r}")

    makefile = (ROOT / "Makefile.portfolio").read_text(encoding="utf-8", errors="replace")
    for target in ["smoke_buffer", "smoke_sem", "smoke_mon", "compile", "validate", "runtime-test", "clean"]:
        if target not in makefile:
            errors.append(f"Makefile.portfolio missing target/content: {target}")

    for rel in SIMPLE_BUFFER_INVARIANT_FILES:
        source = (ROOT / rel).read_text(encoding="utf-8", errors="replace")
        if "buffer.read = buffer.read->next" in source:
            errors.append(f"{rel} mutates buffer.read directly inside traversal")
        if "!buffer.read && !buffer.write" in source:
            errors.append(f"{rel} uses && for missing-pointer guard; use || to catch half-initialized state")

    sem_header = (ROOT / "prodcon_sem/buffer_sem.h").read_text(encoding="utf-8", errors="replace")
    if "long enqueue_buffer_421(const void *data);" not in sem_header:
        errors.append("prodcon_sem/buffer_sem.h should expose enqueue_buffer_421 as const void *")
    if "long dequeue_buffer_421(void *data);" not in sem_header:
        errors.append("prodcon_sem/buffer_sem.h should expose dequeue_buffer_421 as void *")

    sem_source = (ROOT / "prodcon_sem/buffer_user_sem.c").read_text(encoding="utf-8", errors="replace")
    if "memcpy(buffer.write->data, data, DATA_LENGTH)" not in sem_source:
        errors.append("prodcon_sem/buffer_user_sem.c should copy DATA_LENGTH bytes on enqueue")
    if "memcpy(data, buffer.read->data, DATA_LENGTH)" not in sem_source:
        errors.append("prodcon_sem/buffer_user_sem.c should copy DATA_LENGTH bytes on dequeue")
    for phrase in ["sem421_init", "sem421_wait", "sem421_post", "sem421_getvalue"]:
        if phrase not in sem_source:
            errors.append(f"prodcon_sem/buffer_user_sem.c missing local semaphore wrapper: {phrase}")

    sem_smoke = (ROOT / "tests/smoke_sem.c").read_text(encoding="utf-8", errors="replace")
    if "memcmp(data, expected, DATA_LENGTH)" not in sem_smoke:
        errors.append("tests/smoke_sem.c should verify byte-for-byte semaphore buffer output")
    for phrase in ["WRAP_EXTRA", "SIZE_OF_BUFFER + WRAP_EXTRA", "pre-wrap dequeued data mismatch", "post-wrap dequeued data mismatch"]:
        if phrase not in sem_smoke:
            errors.append(f"tests/smoke_sem.c missing full/wraparound validation phrase: {phrase}")

    if errors:
        print("os candidate validation failed")
        for error in errors:
            print(f"- {error}")
        raise SystemExit(1)

    print("os candidate validation passed")


if __name__ == "__main__":
    main()
