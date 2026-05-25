test:
	@$(MAKE) -sk test-all

test-all:	test-scripts
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-data:
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework05/hulk.hashes
	@echo "*.hashes" > .gitignore
	@echo "*.test" >> .gitignore

test-scripts:	test-data
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework05/searx.test
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework05/hulk.test
	@chmod +x ./*.test

test-searx:	test-scripts searx.py
	@echo Testing SearX ...
	@./searx.test -v

test-hulk:	test-scripts hulk.py
	@echo Testing Hulk ...
	@./hulk.test -v

clean:
	@rm -f *.test *.hashes

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
    print()
    return float(grade), status == 'Success'

searx_grade, searx_status = run_and_extract('make test-searx')
hulk_grade , hulk_status  = run_and_extract('make test-hulk')
quiz_grade , quiz_status  = run_and_extract('../.scripts/check.py')

print('Grade Summary ...')
print('  SearX   {:4.2f} /  0.40'.format(0.4*searx_grade))
print('  Hulk    {:4.2f} /  0.40'.format(0.4*hulk_grade))
print('  Quiz    {:4.2f} /  0.20'.format(0.2*quiz_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.4*searx_grade + 0.4*hulk_grade + 0.2*quiz_grade
))
print('  Status {}'.format(
    'Success' if all([searx_status, hulk_status, quiz_status]) else 'Failure'
))

exit(0 if all([searx_status, hulk_status, quiz_status]) else 1)
endef

export SUMMARIZE_GRADE_PY
