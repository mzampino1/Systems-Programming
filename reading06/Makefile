CC=             gcc
CFLAGS=         -Wall -g -std=gnu99
TARGETS=        sort grep

all:            $(TARGETS)

sort:       sort.c
	$(CC) $(CFLAGS) -o sort sort.c

grep:       grep.c
	$(CC) $(CFLAGS) -o grep grep.c

test:
	        @$(MAKE) -sk test-all

test-all:	test-scripts $(TARGETS) 
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-scripts:	
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading06/test_sort.sh
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading06/test_grep.sh
	@chmod +x test_*.sh

test-sort:	test-scripts sort
	./test_sort.sh

test-grep:	test-scripts grep
	./test_grep.sh

test-quiz:
	../.scripts/check.py

clean:
	@rm -f $(TARGETS) test_*.sh

.SUFFIXES:

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

quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
sort_grade, sort_status = run_and_extract('make test-sort')
grep_grade, grep_status = run_and_extract('make test-grep')
all_success             = all([sort_status, grep_status, quiz_status])

print('Grade Summary ...')
print('  Quiz    {:4.2f} /  0.20'.format(0.2*quiz_grade))
print('  Sort    {:4.2f} /  0.40'.format(0.4*sort_grade))
print('  Grep    {:4.2f} /  0.40'.format(0.4*grep_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.4*sort_grade + 0.4*grep_grade + 0.2*quiz_grade
))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
