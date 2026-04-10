import pathlib
import re
import subprocess
import sys
import time

TIME_LIMIT = 10.0  # seconds per testcase
SOURCE_FILE = "2048.c"
EXE_NAME = "a.exe" if sys.platform.startswith("win") else "a"
TESTCASE_DIR = pathlib.Path("testcase")


def normalize_text(s: str) -> str:
    s = s.replace("\r\n", "\n").replace("\r", "\n")
    s = re.sub(r"[ \t](?=\n|$)", "", s)
    if s.endswith("\n"):
        s = s[:-1]
    return s


def run_case(exe_path: str, input_text: str, timeout_sec: float):
    proc = subprocess.Popen(
        [exe_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    start = time.perf_counter()
    try:
        out, err = proc.communicate(input=input_text, timeout=timeout_sec)
        elapsed = time.perf_counter() - start
        return proc.returncode, out, err, False, elapsed
    except subprocess.TimeoutExpired:
        proc.kill()
        out, err = proc.communicate()
        return None, out, err, True, timeout_sec


def collect_tasks(base_dir: pathlib.Path):
    tasks = []
    for in_path in base_dir.glob("task*.in"):
        m = re.fullmatch(r"task(\d+)\.in", in_path.name)
        if not m:
            continue
        idx = int(m.group(1))
        out_path = base_dir / f"task{idx}.out"
        if out_path.exists():
            tasks.append((idx, in_path, out_path))
    tasks.sort(key=lambda x: x[0])
    return tasks


def main():
    tasks = collect_tasks(TESTCASE_DIR)

    if not tasks:
        print("No testcases found. Expected files like testcase/task1.in / testcase/task1.out.")
        return 1

    try:
        build = subprocess.run(
            ["gcc", "-O2", "-std=c11", "-o", EXE_NAME, SOURCE_FILE],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
    except FileNotFoundError:
        print("Compile Error")
        print("gcc not found. Please install gcc and try again.")
        return 1
    if build.returncode != 0:
        print("Compile Error")
        print((build.stderr or build.stdout).strip())
        return 1

    exe_path = str((pathlib.Path(".") / EXE_NAME).resolve())
    passed = 0

    print(f"Found {len(tasks)} testcase(s) in {TESTCASE_DIR}.")
    for idx, in_path, out_path in tasks:
        input_text = in_path.read_text(encoding="utf-8", errors="ignore")
        expected = out_path.read_text(encoding="utf-8", errors="ignore")

        rc, out, err, is_tle, elapsed = run_case(exe_path, input_text, TIME_LIMIT)
        tag = f"task{idx}"

        if is_tle:
            print(f"{tag}: Time Limit Exceed ({TIME_LIMIT:.1f}s)")
            continue
        if rc is None or rc != 0:
            snippet = (err or "").strip()
            if len(snippet) > 300:
                snippet = snippet[:300] + "..."
            print(f"{tag}: Runtime Error")
            if snippet:
                print(snippet)
            continue

        got = normalize_text(out)
        exp = normalize_text(expected)
        if got == exp:
            passed += 1
            print(f"{tag}: Accepted ({elapsed*1000:.1f} ms)")
        else:
            print(f"{tag}: Wrong Answer")
            print("Expected Output:")
            print(exp if exp else "<empty>")
            print("Your Output:")
            print(got if got else "<empty>")

    total = len(tasks)
    print(f"Score: {passed}/{total}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
