# green is what is missing, white is what's correct, red is what you wrote extra
import sys
from tempfile import NamedTemporaryFile as TF
from subprocess import check_output, STDOUT
import os

DIFF=os.environ.get('DIFF', 'git --no-pager diff --word-diff=color --word-diff-regex=. ')


def report_error(cond, correct, output, kind):
    if cond:
        print('PASS')
        return True
    with TF('w') as cf, TF('w') as of:
        cf.write(correct)
        cf.flush()
        if kind == 'all':
            of.write(output)
        elif kind == 'start':
            of.write(output[:len(correct)])
        elif kind == 'end':
            of.write(output[-len(correct)-1:])

        of.flush()
        os.system(f'{DIFF} {of.name} {cf.name}')
        print('FAIL')
        return False

if len(sys.argv) != 2:
    print('usage: python3 test.py ex.txt', file=sys.stderr)
    raise SystemExit(1)
test_file = sys.argv[1]
tests = [a.lstrip() for a in ''.join(open(test_file)).split('---')]
for test in tests[int(os.environ.get('START', 0)):]:
        i, _, do = test.partition('\n')
        do = do.replace('\r', '')
        print(i)
        assert i.startswith('$'), f"the first character of a test case must be $, got {i}"
        ro = check_output(i.lstrip('$ '), shell=True, stderr=STDOUT, universal_newlines=True, encoding='utf-8')
        if '...' in do:
            start, end = do.split('...')
            if report_error(ro.startswith(start), start, ro, 'start'):
                report_error(ro.endswith(end), end, ro, 'end')
        else:
            report_error(ro == do, do, ro, 'all')
