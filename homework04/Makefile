test:
	@$(MAKE) -sk test-all

test-all:	test-scripts
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-data:
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework04/cut.input
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework04/wc.input

test-scripts:	test-data
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework04/cut.test
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework04/wc.test
	@chmod +x ./*.test

test-cut:	test-scripts cut.py
	@echo Testing Cut ...
	@./cut.test -v
	@echo

test-wc: 	test-scripts wc.py
	@echo Testing WC ...
	@./wc.test -v
	@echo

test-quiz:
	../.scripts/check.py

clean:
	@rm -f *.input *.test

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

cut_grade , cut_status  = run_and_extract('./cut.test -v')
wc_grade  , wc_status   = run_and_extract('./wc.test -v')
quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')

print('Grade Summary ...')
print('  Cut     {:4.2f} /  0.40'.format(0.4*cut_grade))
print('  Wc      {:4.2f} /  0.40'.format(0.4*wc_grade))
print('  Quiz    {:4.2f} /  0.20'.format(0.2*quiz_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.4*cut_grade + 0.4*wc_grade + 0.2*quiz_grade
))
print('  Status {}'.format(
    'Success' if all([cut_grade, wc_grade, quiz_status]) else 'Failure'
))
endef

export SUMMARIZE_GRADE_PY
