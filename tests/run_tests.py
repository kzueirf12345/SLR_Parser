import subprocess
from pathlib import Path

BASE_DIR = Path(__file__).parent.parent
PARSER = BASE_DIR / "build" / "SLRParser"
TESTS_DIR = BASE_DIR / "tests"
REPORTS_DIR = BASE_DIR / "tests" / "reports"

TEST_DIRS = ["valid", "invalid"]

for dir_name in TEST_DIRS:
    (REPORTS_DIR / dir_name).mkdir(parents=True, exist_ok=True)

for dir_name in TEST_DIRS:
    test_dir = TESTS_DIR / dir_name
    report_dir = REPORTS_DIR / dir_name
    
    if not test_dir.exists():
        continue
    
    for test_file in test_dir.glob("*.popin"):
        output_file = report_dir / (test_file.stem + ".popout")
        
        cmd = [
            str(PARSER),
            "-i", str(test_file),
            "-v",
            "-o", str(output_file)
        ]
        
        print(f"Running: {test_file.name} -> {output_file.name}")
        subprocess.run(cmd)

print("Done!")