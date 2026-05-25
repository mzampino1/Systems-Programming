CC=             gcc
CFLAGS=         -Wall -std=gnu99 -g
TARGETS=        str_rev

all:            $(TARGETS)

#-------------------------------------------------------------------------------
# Add rule for str_rev
#-------------------------------------------------------------------------------

str_rev: str_rev.c
	$(CC) $(CFLAGS) -o $@ $<

#-------------------------------------------------------------------------------
# DO NOT MODIFY BELOW
#-------------------------------------------------------------------------------

test:
	        @$(MAKE) -sk test-all

test-all:	$(TARGETS)
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-str:	str_rev
	curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading07/str_rev.test.sh
	chmod +x str_rev.test.sh
	./str_rev.test.sh

clean:
	rm -f $(TARGETS)

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
str_rev_grade, str_rev_status = run_and_extract('make test-str')
all_success = all([str_rev_status, quiz_status])

print('Grade Summary ...')
print('  Quiz    {:4.2f} /  0.20'.format(0.2*quiz_grade))
print('  StrRev  {:4.2f} /  0.80'.format(0.8*str_rev_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.8*str_rev_grade + 0.2*quiz_grade
))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
