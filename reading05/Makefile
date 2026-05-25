test:
	@$(MAKE) -sk test-all

test-all:	test-scripts
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-scripts:
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading05/odds.test
	@chmod +x ./*.test

test-odds:	test-scripts odds.py
	@echo Testing Odds ...
	@./odds.test -v

test-quiz:
	../.scripts/check.py

clean:
	@rm -f *.test

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

quiz_grade  , quiz_status = run_and_extract('../.scripts/check.py')
odds_grade, odds_status   = run_and_extract('./odds.test -v')

print('Grade Summary ...')
print('  Quiz    {:4.2f} /  0.50'.format(0.5*quiz_grade))
print('  Odds    {:4.2f} /  0.50'.format(0.5*odds_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.5*odds_grade + 0.5*quiz_grade
))
print('  Status {}'.format(
    'Success' if all([odds_status, quiz_status]) else 'Failure'
))
endef

export SUMMARIZE_GRADE_PY
