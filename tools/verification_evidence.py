"""Capture CTest results and controlled inputs for one GitHub run attempt."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET

root = Path(__file__).resolve().parent.parent
files = {}
for name in ['CMakeLists.txt', 'src', 'include', 'tests', 'docs', 'tools', '.factory', '.github']:
    base = root / name
    for path in sorted(base.rglob('*') if base.is_dir() else [base]):
        if path.is_file() and '__pycache__' not in path.parts:
            files[path.relative_to(root).as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
tests = []
report = root / 'build/report.xml'
if report.is_file():
    for case in ET.parse(report).getroot().iter('testcase'):
        status = 'failed' if case.find('failure') is not None or case.find('error') is not None else 'skipped' if case.find('skipped') is not None else 'passed'
        tests.append({'name': case.attrib['name'], 'status': status, 'duration': float(case.attrib.get('time', '0'))})
data = {
    'schemaVersion': 1,
    'runId': os.environ.get('GITHUB_RUN_ID', 'local'),
    'attempt': int(os.environ.get('GITHUB_RUN_ATTEMPT', '1')),
    'sourceSha': subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=root, text=True).strip(),
    'workflowSha': os.environ.get('GITHUB_SHA'),
    'execution': 'not-run' if not tests else 'failed' if any(t['status'] != 'passed' for t in tests) else 'succeeded',
    'files': files, 'tests': tests,
    'coverage': 'not-collected', 'approval': 'not-approved',
}
(root / 'build').mkdir(exist_ok=True)
(root / 'build/verification.json').write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding='utf-8')
print(f"Captured {len(tests)} test observations for {data['sourceSha']}; approval remains not-approved")
