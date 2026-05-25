test:
	@$(MAKE) -sk test-all

test-all:	test-scripts
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-scripts:
	curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading04/test_exists.sh
	chmod +x test_*.sh

test-exists:
	./test_exists.sh

test-quiz:
	../.scripts/check.py

# Embedded Grade Summary Python Script

define SUMMARIZE_GRADE_PY
import os

def run_and_extract(cmd):
    grade  = 0.0
    status = 'Failure'
    for line in os.popen(cmd):
        if 'Grade' in line:  grade  = line.split()[1]
        if 'Status' in line: status = line.split()[1]
        print(line, end='')
    return float(grade), status == 'Success'

exists_grade, exists_status = run_and_extract('./test_exists.sh')
quiz_grade  , quiz_status   = run_and_extract('../.scripts/check.py')

print('Grade Summary ...')
print('  Quiz    {:4.2f} /  0.50'.format(0.5*quiz_grade))
print('  Exists  {:4.2f} /  0.50'.format(0.5*exists_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.5*exists_grade + 0.5*quiz_grade
))
print('  Status {}'.format(
    'Success' if all([exists_status, quiz_status]) else 'Failure'
))
endef

export SUMMARIZE_GRADE_PY
