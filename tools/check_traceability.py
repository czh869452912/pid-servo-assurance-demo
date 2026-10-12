"""Check explicit bidirectional references; not a semantic or coverage verifier."""
import json
from pathlib import Path

root = Path(__file__).resolve().parent.parent
matrix = json.loads((root / '.factory/traceability.json').read_text(encoding='utf-8'))
ids = set()
for row in matrix['requirements']:
    assert row['id'] not in ids, f"duplicate: {row['id']}"
    ids.add(row['id'])
    for name in [row['source'], row['design'], row['code'], *row['tests']]:
        path = (root / name).resolve()
        assert path.is_relative_to(root), f"outside workspace: {name}"
        assert path.is_file(), f"missing: {name}"
        if name != row['code']:
            assert row['id'] in path.read_text(encoding='utf-8'), f"missing backlink: {name}/{row['id']}"
assert ids, "empty requirements matrix"
print(f'Traceability reference checks passed: {len(ids)} requirements (not test/coverage approval)')
